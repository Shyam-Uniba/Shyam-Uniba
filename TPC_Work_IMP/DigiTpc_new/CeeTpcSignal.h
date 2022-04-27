//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Data Representation of an Analog Signal created by the TPC
//      GEM readout. Signal shape after Preamp/Shaper 
//
// Environment:
//      Software developed for the PANDA Detector at FAIR.
//
// Author List:
//      Sebastian Neubert    TUM            (original author)
//
//
//-----------------------------------------------------------

#ifndef CEETPCSIGNAL_HH
#define CEETPCSIGNAL_HH

// Base Class Headers ----------------
#include "FairMultiLinkedData.h"

// Collaborating Class Headers -------
#include <ostream> // remove if you do not need streaming op
#include "McId.h"
// Collaborating Class Declarations --
class CeeTpcAvalanche;


class CeeTpcSignal : public FairMultiLinkedData {
public:

  // Constructors/Destructors ---------
  CeeTpcSignal();
  CeeTpcSignal(unsigned int event, double T, double Amp,
	    unsigned int PadID, unsigned int SectorId, unsigned int RowId, CeeTpcAvalanche* Mother);
  ~CeeTpcSignal(){;}
  
  // Operators
  friend bool operator== (const CeeTpcSignal& lhs, const CeeTpcSignal& rhs);
  friend bool operator< (const CeeTpcSignal& lhs, const CeeTpcSignal& rhs);
  friend std::ostream& operator<< (std::ostream& s, const CeeTpcSignal& me);

  // Accessors -----------------------
  double t() const {return ft;}
  double amp() const {return famp;}
  unsigned int padId() const {return fpadId;}
  unsigned int sectorId() const {return fSectorId;}
  unsigned int rowId() const {return fRowId;}
  unsigned int mcTrackId() const; 
  int mcMotherId() const;
  unsigned int mcHitId() const;
  unsigned int mcEventId() const {return fmcEventId;}
  McId mcId() const;
  CeeTpcAvalanche* mother() const {return fmother;}
  
  // Modifiers -----------------------
  void sett(const double T) {ft=T;}
  void setamp(const double A) {famp=A;}
  void setpadId(const unsigned int PadID) {fpadId=PadID;}
  void setmcTrackId(const unsigned int ID) {fmcTrackId=ID;}
  void setmcMotherId(const int ID) {fmcMotherId=ID;}
 // void setmcHitId(const unsigned int ID) {fmcHitId=ID;}
  void setmcEventId(const unsigned int ID) {fmcEventId=ID;}
 
  // Operations ----------------------

private:

  // Private Data Members ------------
  double ft;  // Start time
  double famp;  // Amplitude
  unsigned int fpadId;
  unsigned int fmcTrackId;
  int fmcMotherId;
  unsigned int fmcHitId;
  unsigned int fmcEventId;
  unsigned int fSectorId;
  unsigned int fRowId;
  unsigned int fEvent;
  CeeTpcAvalanche* fmother;

  // Private Methods -----------------

public:
  ClassDef(CeeTpcSignal,1)

};

// Sorting Functor
class CeeTpcSignalComparePadID {
  public:
    CeeTpcSignalComparePadID(){;}
    ~CeeTpcSignalComparePadID(){;}
    bool operator()(CeeTpcSignal* s1, CeeTpcSignal* s2){return s1->padId()<s2->padId();}
  };


#endif

