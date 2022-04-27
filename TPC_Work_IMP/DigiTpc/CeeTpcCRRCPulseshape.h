//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Pulseshape from an CRRC-Shaper
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

#ifndef CEETPCCRRCPULSESHAPE_HH
#define CEETPCCRRCPULSESHAPE_HH

// Base Class Headers ----------------
#include "CeeTpcAbsPulseshape.h"

// Collaborating Class Headers -------
#include <ostream> // remove if you do not need streaming op

// Collaborating Class Declarations --



class CeeTpcCRRCPulseshape  : public CeeTpcAbsPulseshape{
public:

  // Constructors/Destructors ---------
  CeeTpcCRRCPulseshape(){;}
  CeeTpcCRRCPulseshape(double Tint, double Tdif, double Tsig);
  virtual ~CeeTpcCRRCPulseshape(){;}

  // Operators
  
  // Accessors -----------------------


  // Modifiers -----------------------


  // Operations ----------------------
  virtual double operator() (const double t, 
			     const double amp, 
			     const double toffset) const;

private:

  // Private Data Members ------------
  double fTint;
  double fTdif;
  double fTsig;
 

  // Private Methods -----------------
  
  double general_solution(const double t, 
		     const double amp, 
		     const double toffset) const;
  double degenerate_solution(const double t,  // for Tdif=Tint
		     const double amp, 
		     const double toffset) const; 

};

#endif

//--------------------------------------------------------------
// $Log$
//-----------------------------------

