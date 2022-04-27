#ifndef CEETPCDIGIPAR_H
#define CEETPCDIGIPAR_H

#include "FairParGenericSet.h"
#include "FairParamList.h"
#include "TString.h"

// interfaces to the CeeTpcCode:
class CeeTpcGas;
class CeeTpcGem;
class CeeTpcPadPlane;
class CeeTpcFrontend;
class CeeTpcPadShapePool;

class CeeTpcDigiPar : public FairParGenericSet
{
public :
  
  CeeTpcDigiPar (const char* name="CeeTpcPars",
	      const char* title="CeeTpc digi parameter",
	      const char* context="TestDefaultContext");

  ~CeeTpcDigiPar();
  void clear(){};
  void putParams(FairParamList* list);
  Bool_t getParams(FairParamList* list);

  // Accessors (partially catched!)
  double getYGem()const {return fyGem;}
  double getYMax() const {return fyMax;}
  double getTDiff() const {return ftdiff;}
  double getTInt() const {return ftint;}
  double getTSig() const {return ftsig;}
  double getRMin() const {return frmin;}
  double getRMax() const {return frmax;}
  double getMeanTimeInGEM() const {return fmean_time;}
  double getSigmaTimeInGEM() const {return fspread_time;}
  bool getAttach() const {return fattach>0;}
  bool getDiffuseL() const {return fdiffuseL>0;}
  bool getDiffuseT() const {return fdiffuseT>0;}
  double getGain() const {return fgain;}
  double getSupression() const {return fsupression;}
  double getRate() const {return frate;}
  double getMinSigAmp() const {return fminSignalAmp;}
  int getPSA() const {return fpsa;}
  int getNtimebins() const {return fntimebins;}
  int getTimeStep() const {return ftimestep;}
  const CeeTpcGas* getGas();
  const CeeTpcGem* getGem();
  CeeTpcPadPlane* getPadPlane();
  const CeeTpcPadShapePool* getPadShapes();
  const CeeTpcFrontend* getFrontend();

  bool getGaussianNoise() const {return fgaussianNoise != 0;}
  int getGaussianNoiseAmp() const {return fgaussianNoiseAmp;}

private :
  Int_t ftpcGasFile;
  TString ftpcGasFileName;
  Double_t fEField;
  Int_t fattach;
  Int_t fdiffuseL;
  Int_t fdiffuseT;
  Int_t fdistort;
  Double_t fyGem; // get from Geom!
  Double_t fyMax; // get from Geom!

  Double_t fgain;
  Double_t fsupression;
  Double_t fspread_xz; // Spread in XZ direction due to GEM
  Double_t fspread_time; //Spread in time direction due to GEM
  Double_t fmean_time; // Mean time in GEM
  Double_t frate;

  Double_t fminSignalAmp; // cut on signal amplitude
  Double_t frmin, frmax; // cuts on the TPC volume in r should come from Geom!
  Int_t fpadPlaneFile;
  TString fpadPlaneFileName;
  Int_t fpadShapes;
  TString fpadShapesName;
  Double_t fshaperange; // range for padresponse lookuptable calculation
  Double_t fshapestep;  // LOT entry steps
  Double_t fintstep;    // integration step for LOT point calculation

  int fadcthres; // adc threshold
  int fadcmax; // adc dynamic range maximum
  int fadcbits; // adc # bits
  double fsamplefreq; // sampling frequency
  double ft0; // t0 of time stamps
  int ftimebits; // bits for timestamps
  int fpsathres; // threshold for pulseshape analysis
  double ftdiff; // pulseshape: preamp differentiation time
  double ftint; // pulseshape: preamp integration time
  double ftsig; // pulseshape: signal risetime
  
  int fpsa;	//0 -> Simple PSA, 1 -> TOTPSA
  int fntimebins; // maximum time bins
  int ftimestep; // timebin step
  int fgaussianNoise;
  int fgaussianNoiseAmp;

  // interface pointers (will be created on the fly):
  CeeTpcGas* fgas; //!
  CeeTpcGem* fgem; //!
  CeeTpcPadPlane* fpadplane; //!
  CeeTpcFrontend* fFE; //! // Front End
  CeeTpcPadShapePool* fpadshapepool; //!

 
  TString readString(int i);

  ClassDef(CeeTpcDigiPar,1);
};

#endif

