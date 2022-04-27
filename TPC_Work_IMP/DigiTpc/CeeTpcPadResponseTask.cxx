//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Implementation of class CeeTpcPadResponseTask
//      see CeeTpcPadResponseTask.hh for details
//
// Environment:
//      Software developed for the PANDA Detector at FAIR.
//
// Author List:
//      Sebastian Neubert    TUM            (original author)
//
//
//-----------------------------------------------------------

// Panda Headers ----------------------

// This Class' Header ------------------
#include "CeeTpcPadResponseTask.h"
#include <fstream>
// C/C++ Headers ----------------------


// Collaborating Class Headers --------
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "CeeTpcDigiPar.h"
#include "TClonesArray.h"
#include "TRandom.h"
#include "CeeTpcSignal.h"
#include "CeeTpcAvalanche.h"
#include "CeeTpcPad.h"
#include "CeeTpcPadPlane.h"
#include "CeeTpcGem.h"
#include "CeeDetectorList.h"
#include "QAPlotCollection.h"

#include <iostream>

// Class Member definitions -----------


CeeTpcPadResponseTask::CeeTpcPadResponseTask()
  : FairTask("TPC PadResponse"), fpersistence(kFALSE), fminSignalAmp(0), fMeanGemTime(0),
	fSigmaGemTime(0), fselected(false), finitialized(kFALSE), fqa(NULL), fdebug(kFALSE)
    //TODO: parameter management
{
  favalancheBranchName = "CeeTpcAvalanche";
}


CeeTpcPadResponseTask::~CeeTpcPadResponseTask()
{}

void
CeeTpcPadResponseTask::SetParContainers() {

  std::cout<<"CeeTpcPadResponseTask::SetParContainers"<<std::endl;
  std::cout.flush();

  // Get run and runtime database
  FairRun* run = FairRun::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* db = run->GetRuntimeDb();
  if ( ! db ) Fatal("SetParContainers", "No runtime database");

  // Get CeeTpc digitisation parameter container
  fpar= (CeeTpcDigiPar*) db->getContainer("CeeTpcDigiPar");
  if (! fpar ) Fatal("SetParContainers", "CeeTpc not found");
}




InitStatus CeeTpcPadResponseTask::Init()
{
  finitialized=false;

  //Get ROOT Manager
  FairRootManager* ioman= FairRootManager::Instance();

  if(ioman==0)
    {
      Error("CeeTpcPadResponseTask::Init","RootManager not instantiated!");
      return kERROR;
    }

  // Get input collection
  favalancheArray=(TClonesArray*) ioman->GetObject(favalancheBranchName);
  
  if(favalancheArray==0)
    {
      Error("CeeTpcPadResponseTask::Init","Avalanche-array not found!");
      return kERROR;
    }
  
  // create and register output array
  fsignalArray = new TClonesArray("CeeTpcSignal"); 
  ioman->Register("CeeTpcSignal","CeeTpc",fsignalArray,fpersistence);

  fpadPlane= fpar->getPadPlane();
  fgem=fpar->getGem();
  fminSignalAmp=fpar->getMinSigAmp();
  fMeanGemTime = fpar->getMeanTimeInGEM();
  fSigmaGemTime = fpar->getSigmaTimeInGEM();
  feventcounter=0;

  if(fdebug){ // Create TPC pad file
  std::ofstream padplane("padplane_cee_tpc.dat");
  fpadPlane->WriteToStream(padplane);
  padplane.close();
  }
  std::cout<<"CeeTpcPadPlane Parameters:"<<std::endl;
  std::cout<<*fpadPlane<<std::endl;
  std::cout<<"CeeTpcGem Parameters:"<<std::endl;
  std::cout<<*fgem<<std::endl;

  //Create Histograms for recording spread of Avalanche-centre to hit Pads
  if(fqa!=NULL){
	  fxVariation = fqa->getTH1D("xSpread","Hit Pad x-coordinates around Avalanche centre", 
			 200, -2.5, 2.5);
	  fzVariation = fqa->getTH1D("zSpread", 
			 "Hit Pad z-coordinates around Avalanche centre", 
			 200, -2.5, 2.5);
	  f2DHisto = fqa->getTH2D("xzSpread","xSpread vs zSpread",
		      200,-2.5,2.5,200,-2.5,2.5);
  }
  fselected=fsecids.size()>0;

  finitialized=true;
  return kSUCCESS;
}


void
CeeTpcPadResponseTask::Exec(Option_t* opt)
{
  std::cout<< "CeeTpcPadResponseTask::Exec" << std::endl;
  // Reset output Array
  if(fsignalArray==0) Fatal("CeeTpcPadResponse::Exec","No SignalArray");
  fsignalArray->Delete(); // Note: signals will be cleand up in the adc!
  int nValidHits=0;
  Int_t na=favalancheArray->GetEntriesFast();
  for(Int_t ia=0;ia<na;++ia){
    CeeTpcAvalanche* Aval=(CeeTpcAvalanche*)favalancheArray->At(ia);
    if (fdebug){
    std::cout<<std::endl;
    std::cout<<"\033[1;34m Avalanche No. \033[0m"<<ia<<std::endl;
    }
    std::vector<CeeTpcPad*> hitPads;
    double xAv=Aval->x();
    double zAv=Aval->z();
    double smeared_time = Aval->t()+gRandom->Gaus(fMeanGemTime,fSigmaGemTime);

    if(xAv<-60.|| xAv>60. ||zAv<-45. || zAv>45.) {
    if(fdebug){
     Warning("Exec","Avalanche outside of allowed region! X=%f, Z=%f",xAv,zAv);
    }
     continue;
    }
    fpadPlane->GetPadList(xAv,zAv,fgem->spread_xz()*3, hitPads); // radius of pad search = fgem->spread_xz()*3
    // Build Signals
    int nHits=hitPads.size();
    if(fdebug){
    std::cout<<"\033[1;34m (XAv, ZAv): \033[0m ("<<xAv<<","<<zAv<<")"<<std::endl;
    std::cout<<"\033[1;34m Pad Hits. \033[0m"<<nHits<<std::endl;
    std::cout<<std::endl;
    }
    for(int iHit=0; iHit<nHits; ++iHit){
      CeeTpcPad* pad=hitPads[iHit];
      if(fselected){
	// find sectorid
	 std::map<unsigned int,bool>::iterator it=fsecids.find(pad->sectorId());
   	  if(it==fsecids.end())continue;
      }
      //--------Accessing the Neighbours of a pad-------------
      if(fdebug){
      std::cout<<"(Xpad, Zpad): ("<<pad->x()<<","<<pad->z()<<")"<<std::endl;
      std::cout<<"Region: "<<fpadPlane->Indexer(pad)<<"   Pad Id: "<<pad->id()<<std::endl;
      CeeTpcPad* pad1=fpadPlane->GetPad(pad->id());
      unsigned int nn=pad1->nNeighbours(); // Total neighbours
      std::cout<<"Neighbouring Pads:";
      for(unsigned int in=0;in<nn;++in)  std::cout<<"\t"<<pad1->getNeighbour(in);
      std::cout<<std::endl;
      std::cout<<std::endl;
      }
      double Amp=Aval->amp()*pad->GetValue(xAv,zAv);
      if(fdebug){
      std::cout<<"Amplitute: "<<Amp<<std::endl;
      }
      if(fpar->getGaussianNoise()){
	  Amp+=gRandom->Gaus(0.,fpar->getGaussianNoiseAmp());
      }
      //Fill Histograms with these Values
      FillHistograms(xAv, zAv, pad->x(), pad->z() );      

      // cut on amplitude
      if(Amp>=fminSignalAmp){
        CeeTpcSignal* sig=new((*fsignalArray)[nValidHits]) CeeTpcSignal(feventcounter,smeared_time,Amp,
        		                                           pad->id(),fpadPlane->Indexer(pad),0);
        sig->setmcTrackId(Aval->mcTrackId());
        sig->setmcMotherId(Aval->mcMotherId());
        sig->setmcEventId(feventcounter);
      //  sig->SetLink(FairLink(favalancheBranchName, ia));
        ++nValidHits;
      }
      else {
	 //std::cout<<"PadAmp too small! Amp="<<Amp<<"<"<<fminSignalAmp<<std::endl;
      }
    } // end loop over hit pads

  } // end loop over avalanches

  ++feventcounter;
  std::cout<<fsignalArray->GetEntriesFast()<<" Signals created"<<std::endl;

  return;
}

void CeeTpcPadResponseTask::FillHistograms(double xAv, double zAv,
					double xPad, double zPad) {
  if(fqa!=NULL){
	  fxVariation->Fill(xAv - xPad);
	  fzVariation->Fill(zAv - zPad);
	  f2DHisto->Fill(xAv - xPad, zAv - zPad);
  }
}

//!Method WriteHistograms() has to be called once in the runDigi.C macro!
//! This method is deprecated - replaced by the QAPlotcollection!
void CeeTpcPadResponseTask::WriteHistograms() {

  if(!finitialized || fqa==NULL)return;
  TFile* file = FairRootManager::Instance()->GetOutFile();
  file->mkdir("CeeTpcPadResponse");
  file->cd("CeeTpcPadResponse");
  fxVariation->Write();
  fzVariation->Write();
  f2DHisto->Write();
  delete fxVariation;
  delete fzVariation;
  delete f2DHisto;
  //file->Close(); // If you close the file you can not make directory to it (Shyam).

}


ClassImp(CeeTpcPadResponseTask)
