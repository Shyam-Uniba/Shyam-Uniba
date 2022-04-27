//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      TPC digitization: avalanche 
//      Models a cloud of charge produced by a gas amplification
//      process in the TPC-GEMs
//
// Environment:
//      Software developed for the PANDA Detector at FAIR.
//
// Author List:
//      Sebastian Neubert    TUM            (original author)
//      Cristoforo Simonetto TUM
//
//
//-----------------------------------------------------------

#ifndef CEETPCAVALANCHE_HH
#define CEETPCAVALANCHE_HH

// Base Class Headers ----------------
#include "FairMultiLinkedData.h"

// Collaborating Class Headers -------
#include <ostream>

// Collaborating Class Declarations --
class CeeTpcDriftedElectron;

class CeeTpcAvalanche : public FairMultiLinkedData {
public:

  // Constructors/Destructors ---------
  CeeTpcAvalanche();
  CeeTpcAvalanche(const double X, 
	       const double Z,
	       const double T,
	       const double Amp,
	       CeeTpcDriftedElectron* tpcDElectronp);

  ~CeeTpcAvalanche(){;}

  // Operators
  friend bool operator== (const CeeTpcAvalanche&, const CeeTpcAvalanche&);
  friend bool operator<  (const CeeTpcAvalanche& lhs,const CeeTpcAvalanche& rhs)
  {return lhs.ft < rhs.ft;}

  friend std::ostream& operator<< (std::ostream& s, const CeeTpcAvalanche& me);


  // Accessors -----------------------
  double x() const {return fx;}
  double z() const {return fz;}
  double t() const {return ft;}
  double amp() const {return famp;}
  Int_t mcTrackId() const; 
  Int_t mcMotherId() const;


  CeeTpcDriftedElectron* mother() const {return fmother;}
  
  // Modifiers -----------------------
  void addAmp(double a){famp+=a;}

private:

  // Private Data Members ------------
  double fx;
  double fz;
  double ft;
  double famp;
  CeeTpcDriftedElectron* fmother;
  
  // Private Methods -----------------

public:
  ClassDef(CeeTpcAvalanche,1)

};

#endif
