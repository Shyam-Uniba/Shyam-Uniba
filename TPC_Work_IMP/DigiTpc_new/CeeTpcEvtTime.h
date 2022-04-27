//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      simple class to hold the eventtime for event mixing
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

#ifndef CEETPCEVTTIME_HH
#define CEETPCEVTTIME_HH

// Base Class Headers ----------------
#include "TObject.h"

// Collaborating Class Headers -------

// Collaborating Class Declarations --



class CeeTpcEvtTime : public TObject{
public:

  // Constructors/Destructors ---------
  CeeTpcEvtTime(){}
  CeeTpcEvtTime(double T0, double evtspacing, unsigned int Id):ft0(T0), fevtspacing(evtspacing), fid(Id){}
  ~CeeTpcEvtTime(){}

  // Operators
 

  // Accessors -----------------------
  double t0() const {return ft0;}
  unsigned int id() const {return fid;}

  // Modifiers -----------------------


  // Operations ----------------------

private:

  // Private Data Members ------------
  double ft0; // event time
  unsigned int fid;
  double fevtspacing;

  // Private Methods -----------------

public:
  ClassDef(CeeTpcEvtTime,1)

};

#endif

//--------------------------------------------------------------
// $Log$ Please add entries here if you do further modification
//--------------------------------------------------------------
