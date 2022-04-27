//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Implementation of class CeeTpcSignalAge
//      see CeeTpcSignalAge.h for details
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
#include "CeeTpcSignalAge.h"
#include "CeeTpcSignal.h"

bool 
CeeTpcSignalAge::operator()(CeeTpcSignal* s1, CeeTpcSignal* s2)
{
  return s1->t()<s2->t();
}


