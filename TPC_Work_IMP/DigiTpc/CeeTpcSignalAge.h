//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Functor for comparison of pointers to CeeTpcSignals
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

#ifndef CEETPCSIGNALAGE_HH
#define CEETPCSIGNALAGE_HH

class CeeTpcSignal;

class CeeTpcSignalAge {
public:

  // Constructors/Destructors ---------
  CeeTpcSignalAge(){;}
  ~CeeTpcSignalAge(){;}

  // Operators
  bool operator()(CeeTpcSignal* s1, CeeTpcSignal* s2);
  
};

#endif

//--------------------------------------------------------------
// $Log$
//--------------------------------------------------------------
