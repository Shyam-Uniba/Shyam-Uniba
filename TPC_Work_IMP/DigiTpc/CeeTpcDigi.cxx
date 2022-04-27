//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Implementation of class CeeTpcDigi
//      see CeeTpcDigi.hh for details
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
#include "CeeTpcDigi.h"

// C/C++ Headers ----------------------


// Collaborating Class Headers --------


// Class Member definitions -----------

CeeTpcDigi::CeeTpcDigi()
  : fpadID(0), findex(0), fSector(0), fRow(0),fRegion(0),
	fTimeBin(0),fAdc(0),fMCTrackId(0)
{}


CeeTpcDigi::CeeTpcDigi( const unsigned int PadID, const unsigned int caindex,
		                const int sec, const int region, const int timebin,
						const int adc, const int mctrackId)
  : fpadID(PadID), findex(caindex), fSector(sec),fRegion(region), fTimeBin(timebin),
	fAdc(adc), fMCTrackId(mctrackId)
{
}

ClassImp(CeeTpcDigi)

