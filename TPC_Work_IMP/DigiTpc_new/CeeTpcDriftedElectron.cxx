//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Implementation of class CeeTpcDriftedElectron
//      see CeeTpcDriftedElectron.hh for details
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
#include "CeeTpcDriftedElectron.h"

// C/C++ Headers ----------------------
#include <cmath>

// Collaborating Class Headers --------
#include "CeeTpcPoint.h"

// Class Member definitions -----------

ClassImp(CeeTpcDriftedElectron)

CeeTpcDriftedElectron::CeeTpcDriftedElectron():
  fx(0),fz(0),ft(0),ftpcPClusterp(0)
{;}



CeeTpcDriftedElectron::CeeTpcDriftedElectron(const double X, 
				       const double Z,
				       const double T,
				       CeeTpcPoint* tPCp)
  : fx(X),fz(Z),ft(T),
    ftpcPClusterp(tPCp)
{;}

Int_t CeeTpcDriftedElectron::mcTrackId() const {
  if(ftpcPClusterp==0)return 0;
  return ftpcPClusterp->GetTrackID();
}


Int_t CeeTpcDriftedElectron::mcMotherId() const {
  if(ftpcPClusterp==0)return 0;
  return ftpcPClusterp->GetMotherId();
}

bool operator== (const CeeTpcDriftedElectron& lhs, const CeeTpcDriftedElectron& rhs)
{
  double dx=lhs.fx - rhs.fx;
  double dz=lhs.fz - rhs.fz;
  double dt=lhs.ft - rhs.ft;
  double d2=dx*dx+dz*dz;
  return (d2<1E-10) && (fabs(dt)<1E-4);
}

