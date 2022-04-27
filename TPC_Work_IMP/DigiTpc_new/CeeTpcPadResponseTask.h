//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      CeeTpc PadResponse reads in MCPoints and produces primary electrons
//
//
// Environment:
//      Software developed for the PANDA Detector at FAIR.
//
// Author List:
//      Sebastian Neubert    TUM            (original author)
//
//
//-----------------------------------------------------------

#ifndef CEETPCPADRESPONSETASK_HH
#define CEETPCPADRESPONSETASK_HH

// Base Class Headers ----------------
#include "FairTask.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"

// Collaborating Class Headers -------
#include <map>

// Collaborating Class Declarations --
class TClonesArray;
class CeeTpcPadPlane;
class CeeTpcPad;
class CeeTpcGem;
class CeeTpcDigiPar;

class CeeTpcPadResponseTask : public FairTask {
public:

  // Constructors/Destructors ---------
  CeeTpcPadResponseTask();
  ~CeeTpcPadResponseTask();

  // Operators
  

  // Accessors -----------------------


  // Modifiers -----------------------
   void SetAvalancheBranchName(const TString& name) {favalancheBranchName=name;}
   void SetPersistence(Bool_t opt=kTRUE) {fpersistence=opt;}
   void SelectSector(unsigned int id){fsecids[id]=true;}
   void SetDebug(Bool_t debug){fdebug=debug;}

  // Operations ----------------------
  
  virtual InitStatus Init();

  virtual void Exec(Option_t* opt);

  virtual void SetParContainers();
  double GetInducedCharge(CeeTpcPad* p, const double Xav,
		    const double Zav, const double sigma) const;
  double GausIntegrate(const float a, const float b) const;

private:

  // Private Data Members ------------
  TString fPadPlaneFileName;
  TString favalancheBranchName;
  TClonesArray* favalancheArray;
  TClonesArray* fsignalArray;

  Bool_t fpersistence;
  Double_t fminSignalAmp;
  Double_t fMeanGemTime; // Mean time in the GEM read from tpc.par file
  Double_t fSigmaGemTime;// Sigma time in the GEM read from tpc.par file
  Bool_t fdebug;
  const CeeTpcPadPlane* fpadPlane;
  const CeeTpcGem* fgem;
  TF1 *fcloudShapeX, * fcloudShapeZ; // Cloud shape is Assumed to be Gaussain in 2D
  double* table; // Table for Gaussian Integral
  double fmin; // Min value of interpolation table -50.
  double fmax; // Max value of interpolation table 50.
  Int_t fmaxentries; // Maximum number of entries
  double fstepsize; // stepsize of Gussian Integration = maxrange/nsteps = 100/100000;
  double fsigma; // sigma of GEM
  
  CeeTpcDigiPar* fpar;  

  Int_t feventcounter;

  std::map<unsigned int,bool> fsecids;
  bool fselected; // true if only selected sectors are processed

  Bool_t finitialized;
  
  // Private Methods -----------------

public:
  ClassDef(CeeTpcPadResponseTask,1)

};

#endif

