//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Implementation of class CeeTpcElectronicsTask
//      see CeeTpcElectronicsTask.hh for details
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
#include "CeeTpcElectronicsTask.h"

// C/C++ Headers ----------------------
#include <algorithm>
#include <iostream>
#include "assert.h"

#include "TGraph.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TApplication.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TF1.h"

// Collaborating Class Headers --------
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "CeeTpcDigiPar.h"
#include "TClonesArray.h"
#include "CeeTpcSignal.h"
#include "CeeTpcPadPlane.h"
#include "CeeTpcDigi.h"
#include "CeeTpcFrontend.h"
#include "CeeDetectorList.h"
#include "McId.h"
#include "QAPlotCollection.h"

// C/C++ Headers ----------------------
#include <TMath.h>
#include <TRandom.h>


CeeTpcElectronicsTask::CeeTpcElectronicsTask()
  : FairTask("TPC Electronics response"), fpersistence(kFALSE),finitialized(kFALSE), fCount(0),
	fdebug(kFALSE), fbinSignalStart(1), fbinSignalEnd(1), fSigBinNumber(0), fADCTheshold(5),
	fmaxmctrackId(1000),fPedestal(0),fNoiseTimeBins(1), fSignalStart(0.), fSignalStop(0.),fMaxPadsinRow(240.)
 {
  fsignalBranchName = "CeeTpcSignal";
  fSignalfile = TFile::Open("../tpc/DigiTpc/signal.root");
  fNoisefile = TFile::Open("../tpc/DigiTpc/noise_hist.root");
  hSig = (TH2D*)fSignalfile->Get("hsignal");
  hNoise = (TH1D*)fNoisefile->Get("noise");
 }


CeeTpcElectronicsTask::~CeeTpcElectronicsTask()
{
}


void
CeeTpcElectronicsTask::SetParContainers() {

  std::cout<<"CeeTpcElectronicsTask::SetParContainers"<<std::endl;
  std::cout.flush();

  // Get run and runtime database
  FairRun* run = FairRun::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* db = run->GetRuntimeDb();
  if ( ! db ) Fatal("SetParContainers", "No runtime database");

  // Get CeeTpc digitisation parameter container
  fpar= (CeeTpcDigiPar*) db->getContainer("CeeTpcDigiPar");
  if (! fpar ) Fatal("SetParContainers", "CeeTpcDigiPar not found");
}


InitStatus
CeeTpcElectronicsTask::Init()
{
	
	finitialized=kFALSE;
  //Get ROOT Manager
  FairRootManager* ioman= FairRootManager::Instance();

  if(ioman==0)
    {
      Error("CeeTpcElectronicsTask::Init","RootManager not instantiated!");
      return kERROR;
    }
  
  // Get input collection
  fsignalArray=(TClonesArray*) ioman->GetObject(fsignalBranchName);
  
  if(fsignalArray==0)
    {
      Error("CeeTpcElectronicsTask::Init","Signal-array not found!");
      return kERROR;
    }
  
  // create and register output array
  fdigiArray = new TClonesArray("CeeTpcDigi"); 
  ioman->Register("CeeTpcDigi","CeeTpc",fdigiArray,fpersistence);

  // Histogram for ADC calculation
  hsignal_timebin = new TH2D("hsignal_timebin","Time Bin and MC Track Id",200,0,200.,1000,0,1000.);
  htimebin_proj = new TH1D("htimebin_proj","1D Time Bin Projection",200,0.,200.);
  htrack_proj = new TH1D("htrack_proj","1D Track Projection",1000,0.,1000.);

  // SAMPA Response signal binning
  fbinSignalStart = hSig->FindBin(fSignalStart); // signal starts at -1.8
  fbinSignalEnd = hSig->FindBin(fSignalStop); // signal end at 4.0
  fSigBinNumber = hSig->GetNbinsX();

  // Noise histogram find pedestal
  fNoiseTimeBins=hNoise->GetNbinsX();
  hNoise->Fit("pol0");
  TF1 *func = (TF1*)hNoise->GetListOfFunctions()->FindObject("pol0");
  fPedestal = func->GetParameter(0);

  //TODO: Get this from Database!
  fpadPlane= fpar->getPadPlane();
  fMaxPadsinRow = fpadPlane->MaxPadX();
  ffrontend= fpar->getFrontend();
  fntimebins = fpar->getNtimebins();
  ftimestep = fpar->getTimeStep();
  finitialized=kTRUE;
  return kSUCCESS;
}


void
CeeTpcElectronicsTask::Exec(Option_t* opt)
{
  std::cout<<"CeeTpcElectronicsTask::Exec "<<std::endl;
  std::cout<<std::endl;
  std::cout<<"Event: "<<fCount<<std::endl;
  // Reset output Array
  if(fdigiArray==0) Fatal("CeeTpcElectronics::Exec)","No DigiArray");
  fdigiArray->Delete();

  int region = 0, timebin = 0, mctrackId = 0, id = 0, is = 0;
  float scalefactor = (1.6384e-3)/569.4; // 100 fc = 1024 ADC
  TRandom *r = new TRandom();
  r->SetSeed(700000000000);
  Int_t iNoiseStart=0;

  Int_t ns=fsignalArray->GetEntriesFast();

  //partition data according to pads
  std::cout<<"Building up padmap ...";
  std::map<unsigned int,std::vector<CeeTpcSignal*> > padmap;
  std::map<unsigned int, std::vector<int> > sigmap;
  std::map<unsigned int, std::vector<int> >::iterator it;

  for(Int_t i=0;i<ns;++i){
	CeeTpcSignal* signal=(CeeTpcSignal*)fsignalArray->At(i);
    unsigned int padid=signal->padId();
    padmap[padid].push_back(signal);
    sigmap[padid].push_back(i);
  }
  std::cout<<"finished. "<<padmap.size()<<" pads hit"<<std::endl;
  int size = padmap.size();

  // Filling 2-D histogram of TimeBin vs MC Track Id
  std::map<unsigned int,std::vector<CeeTpcSignal*> >::iterator padIt;
  for (padIt = padmap.begin(); padIt!=padmap.end(); padIt++){
	 // if (is>0) continue;
	  if (fdebug) std::cout<<padIt->first<<"==>";
	  for( auto pad_iter = padIt->second.begin(); pad_iter != padIt->second.end(); ++pad_iter ){

      if (fdebug) std::cout << (*pad_iter)->amp() <<"  " ;
	  float amp = (*pad_iter)->amp()*scalefactor; // converting electron in ADC
	  mctrackId = (*pad_iter)->mcTrackId();
	  if (mctrackId>fmaxmctrackId) Warning("CeeTpcElectronicsTask::Exec","MCTrackId out of range=%d",mctrackId);
      float drift_timebin = (((*pad_iter)->t())/ftimestep);
	  for (int iTimebin = drift_timebin; iTimebin<=250; iTimebin++){ //
	  int t1bin = (iTimebin-drift_timebin)*250;
	  int t2bin = t1bin + 249;
	  if (t1bin<=0) t1bin = 1;
	  if (t2bin<=0) t2bin = 1;
	  if(t1bin>=fSigBinNumber) t1bin = fSigBinNumber;
	  if(t2bin>=fSigBinNumber) t2bin = fSigBinNumber;
	  if (t1bin==t2bin) continue;
	  if(t2bin<fbinSignalStart || t1bin>fbinSignalEnd) continue;
	  float adcforthisbin = hSig->Integral(t1bin,t2bin,"width");
	  adcforthisbin = adcforthisbin*amp;
	  hsignal_timebin->Fill(iTimebin+0.000001,mctrackId+0.000001,adcforthisbin);
	  }
	  }
	  htimebin_proj = (TH1D*) hsignal_timebin->ProjectionX("hsignal_timebin_px",1,1000,"");
	  for (int tbin=1; tbin<=250;tbin++){
	  iNoiseStart =(int) r->Uniform()*1024;
	  int adc = floor(htimebin_proj->GetBinContent(tbin)+(hNoise->GetBinContent((tbin+iNoiseStart)%fNoiseTimeBins+1)-fPedestal));
	  if (adc>=1024) adc = 1023; // saturation of ADC values
	  if (adc >=fADCTheshold) {
	  htrack_proj = (TH1D*) hsignal_timebin->ProjectionY("htrack_proj_py",tbin,tbin,"");
	  int mcId = htrack_proj->GetMaximumBin()-1;
	  CeeTpcDigi* digi = new((*fdigiArray)[is]) CeeTpcDigi(padIt->first,is, 0.,region,tbin,adc,mcId);
	  digi->SetRow((padIt->first)/fMaxPadsinRow);
	  is++;
	  }
	  if (fdebug) std::cout<<"\n"<<std::endl;
  }
	    htimebin_proj->Reset("");
	    htrack_proj->Reset("");
	    hsignal_timebin->Reset("");; // Clear ADC calculation histogram
  } // Pad loop


  std::cout<<std::endl<<fdigiArray->GetEntriesFast()<<" Digis created"<<std::endl;
  fCount++;

  return;
}


void CeeTpcElectronicsTask::WriteHistograms() {
  if(!finitialized)return;
  TFile* file = FairRootManager::Instance()->GetOutFile();
  file->mkdir("CeeTpcElectronicResponse");
  file->cd("CeeTpcElectronicResponse");
  hsignal_timebin->GetXaxis()->SetTitle("TimeBin");
  hsignal_timebin->GetYaxis()->SetTitle("MCTrackId");
  htimebin_proj->GetXaxis()->SetTitle("TimeBin");
  htimebin_proj->GetYaxis()->SetTitle("ADC");
  htrack_proj->GetXaxis()->SetTitle("MCTrackId");
  htrack_proj->GetYaxis()->SetTitle("ADC");
  hsignal_timebin->Write();
  htimebin_proj->Write();
  htrack_proj->Write();
  delete htrack_proj;
  delete hsignal_timebin;
  delete htimebin_proj;
  file->Close(); // If you close the file you can not make directory to it (Shyam).

}

ClassImp(CeeTpcElectronicsTask)
