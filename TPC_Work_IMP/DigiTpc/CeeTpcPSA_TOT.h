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
//      Christian Hoeppner   TUM            (original author)
//      Sverre Doerheim    TUM            (original author)
//
//
//-----------------------------------------------------------

#ifndef CEETPCPSA_TOT_HH
#define CEETPCPSA_TOT_HH

// Base Class Headers ----------------
#include "CeeTpcAbsPSAStrategy.h"


// Collaborating Class Headers -------
#include <ostream> // remove if you do not need streaming op
#include <vector>
#include "McIdCollection.h"
// Collaborating Class Declarations --i

class CeeTpcPSA_TOT : public CeeTpcAbsPSAStrategy {
 public:
  // Constructors/Destructors ---------
  CeeTpcPSA_TOT();
  virtual ~CeeTpcPSA_TOT(){;}

  
  // Accessors -----------------------


  // Modifiers -----------------------


  // Operations ----------------------
  virtual CeeTpcDigi* ProcessNext(const CeeTpcSample* sample);

  virtual void Process(const std::vector<CeeTpcSample*> & samples, 
					   std::vector<CeeTpcDigi*>& digis,
					   double padThreshold);
  void setOpt(unsigned int f){fNbEmptySampleAllowed=f;} //nb of empty samples tolarate inside a pulse
  void setTimeCalib(double c){fTimeCalib=c;} // set calibration constant for timing 
  void setSamplePersistence(bool opt=kTRUE){fsamplePersistence = opt;}

 private:

  // Private Data Members ------------
  unsigned int fcurrentPadID;
  bool finprogress;
  bool fsamplePersistence;
  unsigned int famp;
  unsigned int ft;
  unsigned int fthreshold;
  unsigned int fNbEmptySampleAllowed;
  double fTimeCalib;

  McIdCollection fmcid;
  // Private Methods -----------------
  void processPulse(std::vector<CeeTpcSample*>,double& t0,double& A,double& length);

};
#endif
