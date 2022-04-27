//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Policy class for the digitizition of the TPC in the ADC module
//
//
// Environment:
//      Software developed for the PANDA Detector at FAIR.
//
// Author List:
//      Sebastian Neubert    TUM            (original author)
//
//
//-----------------------------------------------------------

#ifndef CEETPCDIGITIZATIONPOLICY_HH
#define CEETPCDIGITIZATIONPOLICY_HH

// Collaborating Class Headers -------
#include <vector>

// Collaborating Class Declarations --
class CeeTpcSample;
class CeeTpcSignal;
class CeeTpcFrontend;
class CeeTpcAbsPulseshape;

struct CeeTpcDigitizationPolicy {
  void Digitize(std::vector< CeeTpcSignal* >* SignalVector,
		std::vector< CeeTpcSample* >* SampleVector,
		const CeeTpcFrontend* Frontend,
		const CeeTpcAbsPulseshape* Pulseshape);
};

#endif

//--------------------------------------------------------------
// $Log$
//--------------------------------------------------------------
