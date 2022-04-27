//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Implementation of class PndTpcSignal
//      see PndTpcSignal.h for details
//
// Environment:
//      Software developed for the PANDA Detector at FAIR.
//
// Author List:
//      Sebastian Neubert    TUM            (original author)
//
//
//-----------------------------------------------------------

// This Class' Header ------------------
#include "CeeTpcSignal.h"

// C/C++ Headers ----------------------
#include <math.h>

// Collaborating Class Headers --------
#include "CeeTpcAvalanche.h"

// Class Member definitions -----------

ClassImp(CeeTpcSignal)

CeeTpcSignal::CeeTpcSignal()
  : ft(0),famp(0),fpadId(0),fmcEventId(0), fRegion(0), fmcTrackId(0),fmcMotherId(0),fmcHitId(0),fmother(0), fEvent(0)
{;}

CeeTpcSignal::CeeTpcSignal(unsigned int event, double T,
		     double Amp, 
		     unsigned int PadID, 
			 unsigned int Region,
		     CeeTpcAvalanche* Mother)
  : fEvent(event), ft(T),famp(Amp),fpadId(PadID), fRegion(Region),fmother(Mother)
{
fmcTrackId = 0;
fmcMotherId = 0;
fmcHitId = 0;
fmcEventId = 0;
}


unsigned int 
CeeTpcSignal::mcTrackId() const {
  if(fmother==0) return fmcTrackId;
  return fmother->mcTrackId();
}

int
CeeTpcSignal::mcMotherId() const {
  if(fmother==0) return fmcMotherId;
  return fmother->mcMotherId();
}


McId CeeTpcSignal::mcId() const {
  return McId( mcEventId(),mcTrackId(),mcMotherId() );
}

bool operator== (const CeeTpcSignal& lhs, const CeeTpcSignal& rhs) {
  return fabs(lhs.ft-rhs.ft)<1E-4 && lhs.fpadId==rhs.fpadId;
}

bool operator< (const CeeTpcSignal& lhs, const CeeTpcSignal& rhs) {
  return lhs.ft<rhs.ft;
}

std::ostream& operator<< (std::ostream& s, const CeeTpcSignal& me){
  return s<<"CeeTpcSignal:\n"
	  <<"     t="<<me.ft<<"\n"
	  <<"     Amp="<<me.famp<<"\n"
	  <<"     PadID="<<me.fpadId<<"\n";
}
