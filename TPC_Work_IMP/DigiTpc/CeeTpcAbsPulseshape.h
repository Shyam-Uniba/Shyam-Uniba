//-----------------------------------------------------------
// File and Version Information:
// $Id: CeeTpcAbsPulseshape.hh,v 1.1 2005/09/22 15:46:09 sneubert Exp $
//
// Description:
//      TPC Digitization
//      abstract Base class for Pulseshapes
//
// Environment:
//      Software developed for the PANDA Detector at FAIR.
//
// Author List:
//      Sebastian Neubert    TUM            (original author)
//
//
//-----------------------------------------------------------

#ifndef CEETPCABSPULSESHAPE_HH
#define CEETPCABSPULSESHAPE_HH

// Collaborating Class Headers -------


// Collaborating Class Declarations --

class CeeTpcAbsPulseshape {
public:

  // Constructors/Destructors ---------
  CeeTpcAbsPulseshape(){;}
  virtual ~CeeTpcAbsPulseshape(){;}

  // Operators
  
  virtual double operator() (const double t, 
			     const double amp, const double toffset) const = 0;
  virtual double value(const double t, 
		       const double amp, const double toffset) const 
  {return operator()(t,amp,toffset);}

};

#endif

//--------------------------------------------------------------
// $Log: CeeTpcAbsPulseshape.hh,v $
// Revision 1.1  2005/09/22 15:46:09  sneubert
// *** empty log message ***
//
//--------------------------------------------------------------

