//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      PndTpc Digitization
//      Abstract interface for Pulse Shape Analyzer
//
// Environment:
//      Software developed for the PANDA Detector at FAIR.
//
// Author List:
//      Sebastian Neubert    TUM            (original author)
//
//
//-----------------------------------------------------------

#ifndef CEETPCABSPSASTRATEGY_HH
#define CEETPCABSPSASTRATEGY_HH

// Base Class Headers ----------------
#include <vector>
#include <iostream>
#include <stdlib.h>
#include "CeeTpcAbsPulseshape.h"

// Collaborating Class Headers -------

// Collaborating Class Declarations --
class CeeTpcSample;
class CeeTpcDigi;

class CeeTpcAbsPSAStrategy {
public:

  // Constructors/Destructors ---------
  CeeTpcAbsPSAStrategy(){}
  virtual ~CeeTpcAbsPSAStrategy(){;}


 // Operations ----------------------
  virtual CeeTpcDigi* ProcessNext(const CeeTpcSample* sample) = 0;

  virtual void Process(const std::vector<CeeTpcSample*> & samples, 
					   std::vector<CeeTpcDigi*>& digis,
					   double padThreshold) = 0;

  // TODO kill Maxence for that:
  virtual void setPs(CeeTpcAbsPulseshape* pulseshape) { std::cerr << "setPs not implemented!" << std::endl; exit(1); };

  virtual void TailCancellation(bool b) { std::cerr << "TailCancellation not implemented!" << std::endl; exit(1); };

  virtual void setOpt(unsigned int b) { std::cerr << "PndTpcAbsPSA::Set frequency not implemented!" << std::endl; exit(1); };

protected:

  // Private Data Members ------------
 

  // Private Methods -----------------

};

#endif

//--------------------------------------------------------------
// $Log$
//--------------------------------------------------------------
