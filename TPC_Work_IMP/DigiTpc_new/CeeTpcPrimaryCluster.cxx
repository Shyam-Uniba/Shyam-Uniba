//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Implementation of class CeeTpcPrimaryCluster
//      see CeeTpcPrimaryCluster.hh for details
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
#include "CeeTpcPrimaryCluster.h"
#include "CeeDetectorList.h"

// C/C++ Headers ----------------------


// Collaborating Class Headers --------


// Class Member definitions -----------


ClassImp(CeeTpcPrimaryCluster);

CeeTpcPrimaryCluster::CeeTpcPrimaryCluster()
  : ft(0), fq(0), fmcTrackId(0),fmcMotherId(0),fpos(0,0,0)
{}

CeeTpcPrimaryCluster::CeeTpcPrimaryCluster(const Double_t T, 
				     const Int_t Q, 
				     const TVector3& Pos,
				     const Int_t McTrackId,
				     const Int_t McMotherId)
				     
  : ft(T), fq(Q), fmcTrackId(McTrackId),fmcMotherId(McMotherId),fpos(Pos)
{
	SetLink(FairLink("CeeTpcPoint", McTrackId));
}


CeeTpcPrimaryCluster::~CeeTpcPrimaryCluster()
{}

