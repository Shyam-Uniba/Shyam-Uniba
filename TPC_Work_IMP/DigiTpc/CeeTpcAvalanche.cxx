//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Implementation of class CeeTpcAvalanche
//      see CeeTpcAvalanche.hh for details
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


// This Class' Header ------------------
#include "CeeTpcAvalanche.h"

// C/C++ Headers ----------------------
#include <cmath>

// Collaborating Class Headers --------
//#include "CLHEP/GenericFunctions/Gaussian.hh"
#include "CeeTpcDriftedElectron.h"


ClassImp(CeeTpcAvalanche)

// Class Member definitions -----------

CeeTpcAvalanche::CeeTpcAvalanche()
  : fx(0),fz(0),ft(0),famp(0),
    fmother(0)
{;}

CeeTpcAvalanche::CeeTpcAvalanche(const double X, 
			   const double Z,
			   const double T,
			   const double Amp,
			   CeeTpcDriftedElectron* tpcDElectronp)
  : fx(X),fz(Z),ft(T),famp(Amp),
    fmother(tpcDElectronp)
{;}


Int_t CeeTpcAvalanche::mcTrackId() const {
  if (fmother==0) return 0;
  return fmother->mcTrackId();
}

Int_t CeeTpcAvalanche::mcMotherId() const {
  if (fmother==0) return 0;
  return fmother->mcMotherId();
}




bool operator== (const CeeTpcAvalanche& lhs,const CeeTpcAvalanche& rhs)
{
  double dx=lhs.fx - rhs.fx;
  double dz=lhs.fz - rhs.fz;
  double dt=lhs.ft - rhs.ft;
  double d2=dx*dx+dz*dz;
  return (d2<1E-10) && (fabs(dt)<1E-4);
}

std::ostream& operator<< (std::ostream& s, const CeeTpcAvalanche& me){
  s << "CeeTpcAvalanche:\n"
    << "   t    ="<<me.ft<<"\n"
    << "   x    ="<<me.fx<<"\n"
    << "   z    ="<<me.fz<<"\n"
    << "   Amp  ="<<me.famp<<"\n";
  return s;
}

