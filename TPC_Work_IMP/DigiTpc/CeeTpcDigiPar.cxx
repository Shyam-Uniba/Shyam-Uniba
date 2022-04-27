// /////////////////////////////////////////////////////////////
// // CeeTpcDigiPar
// //
// // Container class for Digitisation parameters
// // class is inherited from FairParGenericSet
// //
// /////////////////////////////////////////////////////////////

#include "CeeTpcDigiPar.h"
#include <iostream>
#include <fstream>
#include <exception>
#include "TSystem.h"
#include "CeeTpcGas.h"
#include "CeeTpcGem.h"
#include "CeeTpcPadShapePool.h"
#include "CeeTpcPadPlane.h"
#include "CeeTpcFrontend.h"
using namespace std;
ClassImp(CeeTpcDigiPar)

CeeTpcDigiPar::CeeTpcDigiPar(const char* name,const char* title,
			     const char* context)
  : FairParGenericSet(name,title,context),
    fgas(NULL), fgem(NULL), fpadplane(NULL), fFE(NULL), fpadshapepool(NULL)
{
	clear();
}

CeeTpcDigiPar::~CeeTpcDigiPar()
{
  if(fgas!=NULL)delete fgas;
  if(fgem!=NULL)delete fgem;
  if(fpadplane!=NULL)delete fpadplane;
  if(fpadshapepool!=NULL)delete fpadshapepool;
  if(fFE!=NULL)delete fFE;

}


void CeeTpcDigiPar::putParams(FairParamList* list)
{
	if(!list) return;
	list->add("CeeTpcGasFile",ftpcGasFile);
	list->add("EField",fEField);
	list->add("AttachFlag",fattach);
	list->add("LogitudinaldiffusionFlag",fdiffuseL);
	list->add("TransversediffusionFlag",fdiffuseT);
	list->add("DriftDistortionFlag",fdistort);
	list->add("yGem",fyGem);
	list->add("yMax",fyMax);
	list->add("Gain",fgain);
	list->add("Supression", fsupression);
	list->add("Spread_xz",fspread_xz);
	list->add("Spread_time",fspread_time);
	list->add("Mean_time_GEM",fmean_time);
	list->add("MinSignalAmp",fminSignalAmp);
	list->add("rMin",frmin);
	list->add("rMax",frmax);
	list->add("PadPlaneFile",fpadPlaneFile);
	list->add("PadShapeFile",fpadShapes);
	list->add("PadShapeRange",fshaperange);
	list->add("PadShapeStep",fshapestep);
	list->add("PadShapeIntStep",fintstep);
	list->add("ADCThreshold",fadcthres);
	list->add("ADCMax",fadcmax);
	list->add("ADCBits",fadcbits);
	list->add("SamplingFreq",fsamplefreq);
	list->add("T0_wallclock",ft0);
	list->add("TimeBits",ftimebits);
	list->add("PSAThreshold",fpsathres);
	list->add("Shaper_tDiff",ftdiff);
	list->add("Shaper_tInt",ftint);
	list->add("Shaper_tSig",ftsig);
	list->add("TOTPSA",fpsa);
	list->add("Maxtimebins",fntimebins);
	list->add("TimeStep",ftimestep);
	list->add("GAUSSIANNOISE",fgaussianNoise);
	list->add("GAUSSIANNOISEAMP",fgaussianNoiseAmp);
	list->add("Rate",frate);
}

Bool_t CeeTpcDigiPar::getParams(FairParamList* list)
{
 	if (!list) {std::cout<<"list not found"<<std::endl; return kFALSE;}
	if(!list->fill("EField",&fEField)) 
	  {std::cout<<"par: EField not found"<<std::endl; return kFALSE;}
	if(!list->fill("CeeTpcGasFile",&ftpcGasFile)) 
	  {std::cout<<"par: CeeTpcGasFile not found"<<std::endl; return kFALSE;}
	if(!list->fill("AttachFlag",&fattach)) 
	  {std::cout<<"par: AttachFlag not found"<<std::endl; return kFALSE;}
	if(!list->fill("LogitudinaldiffusionFlag",&fdiffuseL)) 
	  {std::cout<<"par: LogitudinaldiffusionFlag not found"<<std::endl; return kFALSE;}
	if(!list->fill("TransversediffusionFlag",&fdiffuseT)) 
	  {std::cout<<"par: TransversediffusionFlag not found"<<std::endl; return kFALSE;}
	if(!list->fill("DriftDistortionFlag",&fdistort)) 
	  {std::cout<<"par: DriftDistortionFlag not found"<<std::endl; return kFALSE;}
	if(!list->fill("yGem",&fyGem))
	  {std::cout<<"par: yGem not found"<<std::endl; return kFALSE;}
	if(!list->fill("yMax",&fyMax))
	  {std::cout<<"par: yMax not found"<<std::endl; return kFALSE;}
	if(!list->fill("Gain",&fgain)) 
	  {std::cout<<"par: Gain not found"<<std::endl; return kFALSE;}
	if(!list->fill("Supression",&fsupression)) 
	  {std::cout<<"par: Supression not found"<<std::endl; return kFALSE;}
	if(!list->fill("Spread_xz",&fspread_xz))
	  {std::cout<<"par: Spread XZ not found"<<std::endl; return kFALSE;}
	if(!list->fill("Spread_time",&fspread_time))
	  {std::cout<<"par: Spread Time not found"<<std::endl; return kFALSE;}
	if(!list->fill("Mean_time_GEM",&fmean_time))
	  {std::cout<<"par:GEM Mean Time not found"<<std::endl; return kFALSE;}
	if(!list->fill("MinSignalAmp",&fminSignalAmp)) 
	  {std::cout<<"par: MinSignalAmp not found"<<std::endl; return kFALSE;}
	if(!list->fill("rMin",&frmin)) 
	  {std::cout<<"par: rMin not found"<<std::endl; return kFALSE;}
	if(!list->fill("rMax",&frmax)) 
	  {std::cout<<"par: rMax not found"<<std::endl; return kFALSE;}
	if(!list->fill("PadPlaneFile",&fpadPlaneFile)) 
	  {std::cout<<"par: PadPlaneFile not found"<<std::endl; return kFALSE;}
	if(!list->fill("PadShapeFile",&fpadShapes)) 
	  {std::cout<<"par: PadShapeFile not found"<<std::endl; return kFALSE;}
	if(!list->fill("PadShapeRange",&fshaperange)) 
	  {std::cout<<"par: PadShapeRange not found"<<std::endl; return kFALSE;}
	if(!list->fill("PadShapeStep",&fshapestep)) 
	  {std::cout<<"par: PadShapeStep not found"<<std::endl; return kFALSE;}
	if(!list->fill("PadShapeIntStep",&fintstep)) 
	  {std::cout<<"par: PadShapeIntStep not found"<<std::endl; return kFALSE;}
	if(!list->fill("ADCThreshold",&fadcthres)) 
	  {std::cout<<"par: ADCThreshold not found"<<std::endl; return kFALSE;}
	if(!list->fill("ADCMax",&fadcmax)) 
	  {std::cout<<"par: ADCMax not found"<<std::endl; return kFALSE;}
	if(!list->fill("ADCBits",&fadcbits)) 
	  {std::cout<<"par: ADCBits not found"<<std::endl; return kFALSE;}
	if(!list->fill("SamplingFreq",&fsamplefreq)) 
	  {std::cout<<"par: SamplingFreq not found"<<std::endl; return kFALSE;}
	if(!list->fill("T0_wallclock",&ft0)) 
	  {std::cout<<"par: T0_wallclock not found"<<std::endl; return kFALSE;}
	if(!list->fill("TimeBits",&ftimebits)) 
	  {std::cout<<"par: TimeBits not found"<<std::endl; return kFALSE;}
	if(!list->fill("PSAThreshold",&fpsathres)) 
	  {std::cout<<"par: PSAThreshold not found"<<std::endl; return kFALSE;}
	if(!list->fill("Shaper_tDiff",&ftdiff)) 
	  {std::cout<<"par: Shaper_tDiff not found"<<std::endl; return kFALSE;}
	if(!list->fill("Shaper_tInt",&ftint)) 
	  {std::cout<<"par: Shaper_tInt not found"<<std::endl; return kFALSE;}
	if(!list->fill("Shaper_tSig",&ftsig)) 
	  {std::cout<<"par: Shaper_tSig not found"<<std::endl; return kFALSE;}
	if(!list->fill("TOTPSA",&fpsa)) 
	  {std::cout<<"par: TOTPSA not found"<<std::endl; return kFALSE;}
	if(!list->fill("Maxtimebins",&fntimebins))
	  {std::cout<<"par: Maxtimebins not found"<<std::endl; return kFALSE;}
	if(!list->fill("TimeStep",&ftimestep))
	  {std::cout<<"par: TimeStep not found"<<std::endl; return kFALSE;}
	if(!list->fill("GAUSSIANNOISE",&fgaussianNoise)) 
	  {std::cout<<"par: GAUSSIANNOISE not found"<<std::endl; return kFALSE;}
	if(!list->fill("GAUSSIANNOISEAMP",&fgaussianNoiseAmp)) 
	  {std::cout<<"par: GAUSSIANNOISEAMP not found"<<std::endl; return kFALSE;}
	if(!list->fill("Rate",&frate)) 
	  {std::cout<<"par: Rate not found"<<std::endl; return kFALSE;}  
	
	// read strings
	ftpcGasFileName=readString(ftpcGasFile);
	fpadPlaneFileName=readString(fpadPlaneFile);
	fpadShapesName=readString(fpadShapes);
	

	// update interfaces!
	// Todo: implement chaching!
	try{
	if(fgas!=NULL){
	  delete fgas;
	  getGas();
	}
	if(fgem!=NULL){
	  delete fgem;
	  getGem();
	}
	
	if(fpadplane!=NULL){
	  delete fpadplane;
	  if(fpadshapepool!=NULL){
	    delete fpadshapepool;
	    // padshapes should be reloaded by padplane
	  }
	  getPadPlane();
	}

	if(fFE!=NULL){
	  delete fFE;
	  getFrontend();
	}
	}
	catch (std::exception& ex){
	  std::cout<<ex.what()<<std::endl;
	  throw ex;
	}

	return kTRUE;
}

const CeeTpcGas* 
CeeTpcDigiPar::getGas(){
  if(fgas==NULL){
    // load gasfile
    fgas=new CeeTpcGas(ftpcGasFileName.Data(),fEField);
  }
  return fgas;
}

const CeeTpcGem* 
CeeTpcDigiPar::getGem(){
  if(fgem==NULL){
    fgem=new CeeTpcGem(fgain,fspread_xz,fspread_time);
  }
  return fgem;
}

CeeTpcPadPlane* 
CeeTpcDigiPar::getPadPlane(){
  if(fpadplane==NULL){
    fpadplane=new CeeTpcPadPlane(fpadPlaneFileName.Data(),getPadShapes());
	if(!fpadplane)std::cout<<"CeeTpcDigiPar::getPadPlane: Pad plane not found"<<std::endl;
  }
  return fpadplane;
}

const CeeTpcPadShapePool* 
CeeTpcDigiPar::getPadShapes(){
  if(fpadshapepool==NULL){
    fpadshapepool=new CeeTpcPadShapePool(fpadShapesName.Data(),*getGem(),
				   fshaperange,fshapestep,fintstep);
  }
  return fpadshapepool;
}

const CeeTpcFrontend* 
CeeTpcDigiPar::getFrontend(){
  if(fFE==NULL){
    fFE=new CeeTpcFrontend(ftint,ftdiff,ftsig,
			      fadcthres,fadcmax,fadcbits,
			      fsamplefreq,ft0,ftimebits,
			      fpsathres);
  }
  return fFE;
}

TString CeeTpcDigiPar::readString(int i){
  ifstream infile;
  TString sysFile = gSystem->Getenv("VMCWORKDIR"); // returns /home/shyam/GSI/CeeRoot
  TString parFile = sysFile+"/tpc/DigiTpc/tpc_files.par";
  infile.open(parFile.Data());
  //  infile.open("tpc/tpc.files.par");
  if(!infile){
    std::cout<<"File " << parFile.Data() << " not found!"<<std::endl;
    throw;
  }
  char buffer[256];
  for(int j=0;j<i+1;++j){
    if(infile.eof()){
      std::cout<<"Did not find string#"<<i<<"in file " << parFile.Data() <<std::endl;
      throw;
    }
    infile.getline(buffer,256);
  }
  infile.close();
  return TString(sysFile+"/"+buffer);
}

