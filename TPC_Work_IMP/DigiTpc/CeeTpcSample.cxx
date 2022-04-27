//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Implementation of class CeeTpcSample
//      see CeeTpcSample.hh for details
//
// Environment:
//      Software developed for the PANDA Detector at FAIR.
//
// Author List:
//      Sebastian Neubert    TUM            (original author)
//      Xiaodong Zhang       TUM       (modify it on Oct. 06, 2009)
//      Maxence Vandenbroucke TUM      (modify for crosstalk tagging 26 01 2010)
//-----------------------------------------------------------


// This Class' Header ------------------
#include "CeeTpcSample.h"

// C/C++ Headers ----------------------


// Collaborating Class Headers --------


// Class Member definitions -----------

ClassImp(CeeTpcSample)


CeeTpcSample::CeeTpcSample()
: famp(0),ft(0),fpadId(0)
{;}

CeeTpcSample::CeeTpcSample( const int T, 
			    const int Amp, 
			    const unsigned int PadID,
			    const McIdCollection& mcid)
  : famp(Amp),ft(T),fpadId(PadID),fmcId(mcid)
{;}

CeeTpcSample::CeeTpcSample( const int T,
			    const int Amp,
			    const unsigned int PadID)
  : famp(Amp),ft(T),fpadId(PadID)
{;}


CeeTpcSample::~CeeTpcSample()
{;}



bool 
operator== (const CeeTpcSample& lhs, const CeeTpcSample& rhs){
  return lhs.ft==rhs.ft && lhs.famp==rhs.famp && lhs.fpadId==rhs.fpadId;
}

bool 
operator< (const CeeTpcSample& lhs, const CeeTpcSample& rhs){
  return lhs.ft<rhs.ft;
}

std::ostream& operator<< (std::ostream& s, const CeeTpcSample& me){
  s << "CeeTpcSample:\n"
    << "   t(Sample)     ="<<me.ft<<"\n"
    << "   Amp           ="<<me.famp<<"\n"
    << "   PadID         ="<<me.fpadId<<"\n"
    << "   channelId     ="<<me.fchannelId<<"\n"
    << "   chipId        ="<<me.fchipId<<"\n"
    << "   adcId         ="<<me.fadcId<<"\n"
    << "   sourceId      ="<<me.fsourceId<<std::endl;
    return s;
}

void CeeTpcSample::reset(){
  
  ft = 0;
  famp = 0;
  fchannelId = 0;
  fchipId = 0;
  //fadcId = 0;
  //fsourceId = 0;

}

