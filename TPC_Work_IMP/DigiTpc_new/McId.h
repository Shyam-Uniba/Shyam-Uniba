//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Monte carlo ID which can be used by event mixing applications
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

#ifndef MCID_HH
#define MCID_HH

// Base Class Headers ----------------
#include "TObject.h"

// Collaborating Class Headers -------
#include <ostream> // remove if you do not need streaming op

// Collaborating Class Declarations --



class McId : public TObject{
public:

  // Constructors/Destructors ---------
  McId();

  McId(const unsigned int eventid,
       const unsigned int trackid,
       const int secid=0,
       const double Weight=1);

  virtual ~McId();

  // Operators
  friend bool operator== (const McId& lhs, const McId& rhs);
  friend bool operator< (const McId& lhs, const McId& rhs);
  friend std::ostream& operator<< (std::ostream& s, const McId& me);

  // Accessors -----------------------
  unsigned int mceventID() const {return fmceventID;}
  unsigned int mctrackID() const {return fmctrackID;}
  int mcsecID() const {return fmcsecID;}
  double weight() const {return fweight;}

  // Modifiers -----------------------
  void setweight(double w) {fweight=w;}
  void setEventID(unsigned int id){fmceventID=id;}
  void setTrackID(unsigned int id){fmctrackID=id;}
  void setSecID(int id){fmcsecID=id;}

  // Operations ----------------------
  double IncWeight(double w=1) {return fweight+=w;}

private:

  // Private Data Members ------------
  unsigned int fmceventID; // Event Id
  unsigned int fmctrackID; // Track Id
  int fmcsecID; // 0 if primary track, else > 0, noise: -1
  double fweight;
  
  // Private Methods -----------------

public: 
  ClassDef(McId,1)

};

#endif

//--------------------------------------------------------------
// $Log$
//--------------------------------------------------------------
