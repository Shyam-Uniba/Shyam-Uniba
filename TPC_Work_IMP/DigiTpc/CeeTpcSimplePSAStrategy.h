//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      PndTpc Digitization
//      Implementation of a concrete pulse shape analyzer
//
// Environment:
//      Software developed for the PANDA Detector at FAIR.
//
// Author List:
//      Sebastian Neubert    TUM            (original author)
//
//
//-----------------------------------------------------------

#ifndef CEETPCSIMPLEPSASTRATEGY_HH
#define CEETPCSIMPLEPSASTRATEGY_HH

// Base Class Headers ----------------
#include "CeeTpcAbsPSAStrategy.h"

// Collaborating Class Headers -------
#include <ostream> // remove if you do not need streaming op
#include <vector>
#include "McIdCollection.h"
// Collaborating Class Declarations --



class CeeTpcSimplePSAStrategy : public CeeTpcAbsPSAStrategy {
public:

  // Constructors/Destructors ---------
  CeeTpcSimplePSAStrategy(const unsigned int threshold);
  virtual ~CeeTpcSimplePSAStrategy(){;}

  // Accessors -----------------------


  // Modifiers -----------------------


  // Operations ----------------------
  virtual CeeTpcDigi* ProcessNext(const CeeTpcSample* sample);

  virtual void Process(const std::vector<CeeTpcSample*> & samples, 
					   std::vector<CeeTpcDigi*>& digis,
					   double padThreshold);



private:

  // Private Data Members ------------
  unsigned int fcurrentPadID;
  bool finprogress;
  unsigned int famp;
  unsigned int ft;
  unsigned int fthreshold;
  
  McIdCollection fmcid;
  // Private Methods -----------------

};

#endif

//--------------------------------------------------------------
// $Log$
//--------------------------------------------------------------
