//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Data class to carry information of drifted electrons
//      in CeeTpc digitization. Defined at first GEM.
//
// Environment:
//      Software developed for the PANDA Detector at FAIR.
//
// Author List:
//      Sebastian Neubert    TUM            (original author)
//      Cristoforo Simonetto TUM  
//
//-----------------------------------------------------------

#ifndef TPCDRIFTEDELECTRON_HH
#define TPCDRIFTEDELECTRON_HH

// Base Class Headers ----------------
#include "FairMultiLinkedData.h"
#include "CeeTpcPoint.h"
// Collaborating Class Headers -------


// Collaborating Class Declarations --


class CeeTpcDriftedElectron : public FairMultiLinkedData {
public:

  // Constructors/Destructors ---------
  CeeTpcDriftedElectron();
  CeeTpcDriftedElectron(const double X, 
		     const double Z,
		     const double T,
		     CeeTpcPoint* tpcPClusterp);
  ~CeeTpcDriftedElectron(){;}

  // Operators
  friend bool operator== (const CeeTpcDriftedElectron& lhs,
			  const CeeTpcDriftedElectron& rhs);

  friend bool operator<  (const CeeTpcDriftedElectron& lhs,
			  const CeeTpcDriftedElectron& rhs){return lhs.ft < rhs.ft;}

  // Accessors -----------------------
  double x() const {return fx;}
  double z() const {return fz;}
  double t() const {return ft;}
  Int_t mcTrackId() const;
  Int_t mcMotherId() const;


  CeeTpcPoint* tpcPClusterp() const {return ftpcPClusterp;}
  
  void addtox(double dx) {fx+=dx;}
  void addtoz(double dz) {fz+=dz;}
  void addtot(double dt) {ft+=dt;}

private:

  // Private Data Members ------------
  double fx;
  double fz;
  double ft;
  CeeTpcPoint* ftpcPClusterp; //->
    
  // Private Methods -----------------

public:
  ClassDef(CeeTpcDriftedElectron,1)

};

#endif

//--------------------------------------------------------------
// $Log$

