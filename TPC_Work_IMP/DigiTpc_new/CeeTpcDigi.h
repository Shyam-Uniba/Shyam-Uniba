//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      PndTpc Digitization
//      Persisted data. Raw data
//
//
// Environment:
//      Software developed for the PANDA Detector at FAIR.
//
// Author List:
//      Sebastian Neubert    TUM            (original author)
//
//-----------------------------------------------------------


#ifndef TPCDIGI_HH
#define TPCDIGI_HH

// Base Class Headers ----------------
#include "FairMultiLinkedData.h"

// Collaborating Class Headers -------
#include <ostream> // remove if you do not need streaming op
#include "McIdCollection.h"
#include <iostream>
#include <assert.h>
#include <vector>


class CeeTpcDigi : public FairMultiLinkedData {
public:

  // Constructors/Destructors ---------
  CeeTpcDigi();
  CeeTpcDigi(const unsigned int PadID, const unsigned int index = 0, const unsigned int sec = 0,
		     const unsigned int rowid = 0, const int timebin = 0, const int adc =0, const int mctrackId = 0);

  ~CeeTpcDigi(){;}

  // Operators
  friend bool operator== (const CeeTpcDigi& lhs, const CeeTpcDigi& rhs);
  friend bool operator< (const CeeTpcDigi& lhs, const CeeTpcDigi& rhs);
  friend std::ostream& operator<< (std::ostream& s, const CeeTpcDigi& me);

  // Accessors -----------------------
  unsigned int padId() const {return fpadID;}
  unsigned int index() const {return findex;}
  const Int_t   GetSector ()  const {return fSector;}
  const Int_t   GetRow()   {return fRow;} // 240 pads in X-direction
  const Int_t   GetTimeBin () const {return fTimeBin;}
  const Int_t GetAdc ()     const {return fAdc;}
  const Int_t GetMCTrackId ()     const {return fMCTrackId;}


  // Modifiers -----------------------
  void padId(unsigned int val) {fpadID=val;}
  void index(unsigned int caindex){findex = caindex;}
  void SetSector (Int_t sec = 1)  {fSector = sec;} // put the sector = 1 for CEE TPC
  void SetAdc (Int_t Adc)   {fAdc = Adc;}
  void SetTimeBin (Int_t timebin) {fTimeBin=timebin;}
  void SetMCTrackId(Int_t trackid){fMCTrackId=trackid;}
  void SetRow(Int_t val) {fRow=val;}

 private:

  // Private Data Members (Digi Objects) ------------
  Int_t fTimeBin;
  Int_t fAdc;
  Int_t fMCTrackId;
  Int_t   fSector;
  Int_t   fRow;
  unsigned int fpadID;
  unsigned int findex; /// Position of Digi in TClonesArray (needed for FairLinks to work with PndTpcCluster)

  // Private Methods -----------------
public:
  ClassDef(CeeTpcDigi,1)

};

#endif

