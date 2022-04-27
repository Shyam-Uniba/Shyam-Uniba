//-----------------------------------------------------------
// File and Version Information:
// $Id: CeeTpcPrimaryCluster.hh,v 1.8 2006/08/17 20:03:05 sneubert Exp $
//
// Description:
//      Data class for the Digitization. (Transient)
//      Represents primary clusters of electrons which are created
//      by the passage of a charged particle through the TPC gas-volume
//
//      Created by: PndTpcClusterizerTask
//      Used by:    PndTpcDrifterTask
//
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

#ifndef CEETPCPRIMARYCLUSTER_HH
#define CEETPCPRIMARYCLUSTER_HH

// Base Class Headers ----------------
#include "TObject.h"
#include "FairMultiLinkedData.h"

// Collaborating Class Headers -------
#include "TVector3.h"

// Collaborating Class Declarations --


class CeeTpcPrimaryCluster : public FairMultiLinkedData {
public:

  // Constructors/Destructors ---------
  CeeTpcPrimaryCluster();

  CeeTpcPrimaryCluster(const Double_t T, const Int_t P, const TVector3& Pos,
		       const Int_t McTrackId,
		       const Int_t McMotherId);

  ~CeeTpcPrimaryCluster();

  // Operators
  friend bool operator<  (const CeeTpcPrimaryCluster& lhs,
			  const CeeTpcPrimaryCluster& rhs)
  {return lhs.ft<rhs.ft;}

  // Accessors -----------------------
  Double_t t() const {return ft;}
  Int_t q() const {return fq;}
  const TVector3& pos() const {return fpos;}
  Double_t pos(Int_t i) const {return fpos(i);}
  Double_t x() const {return fpos(0);}
  Double_t y() const {return fpos(1);}
  Double_t z() const {return fpos(2);}
 const Int_t mcTrackId() const {return fmcTrackId;}
 const Int_t mcMotherId() const {return fmcMotherId;}

  // Modifiers -----------------------
  void sett(const Double_t T){ft=T;}
  void setq(const Int_t Q){fq=Q;}
  void setpos(const Double_t X, const Double_t Y, const Double_t Z)
  {fpos(0)=X;fpos(1)=Y;fpos(2)=Z;}
  

private:

  // Private Data Members ------------
  Double_t ft;               // time of creation
  Int_t fq;                  // charge
  Int_t fmcTrackId;     
  Int_t fmcMotherId;
  TVector3 fpos;         // position

  // Private Methods -----------------

public: 
  ClassDef(CeeTpcPrimaryCluster,1)

};

#endif
