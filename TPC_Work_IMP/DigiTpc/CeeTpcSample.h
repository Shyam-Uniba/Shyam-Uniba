//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      TPC digitization:
//      Output of ADC
//
// Environment:
//      Software developed for the PANDA Detector at FAIR.
//
// Author List:
//      Sebastian Neubert    TUM            (original author)
//	Xiaodong Zhang       TUM       (modify it on Oct. 06, 2009) 
//      Maxence Vandenbroucke TUM      (modify it on Jan. 26, 2010) 
//      Felix Boehmer        TUM       (modified on Nov. 28, 2010) 
//-----------------------------------------------------------

#ifndef CEETPCSAMPLE_HH
#define CEETPCSAMPLE_HH

// Base Class Headers ----------------
#include "TObject.h"

// Collaborating Class Headers -------
#include <ostream> // remove if you do not need streaming op
#include <assert.h>
#include "McIdCollection.h"

// Collaborating Class Declarations --


class CeeTpcSample : public TObject{
public:

  // Constructors/Destructors ---------
  CeeTpcSample();
  CeeTpcSample(const int T, 
	       const int Amp, 
	       const unsigned int PadID, 
	       const McIdCollection& mcid); 
  
  CeeTpcSample(const int T,                       
	       const int Amp,
	       const unsigned int PadID);
  
  ~CeeTpcSample();

  // Operators
  friend bool operator== (const CeeTpcSample& lhs, const CeeTpcSample& rhs);
  friend bool operator< (const CeeTpcSample& lhs, const CeeTpcSample& rhs);
  friend std::ostream& operator<< (std::ostream& s, const CeeTpcSample& me);

  // Accessors -----------------------
  int amp() const {return famp;}
  int   t() const {return ft;}
  int padId() const {return fpadId;}
  
  const McIdCollection& mcId() const {return fmcId;}
  //*** next four functions added by x. zhang on 08.10.2009 ****//
  unsigned int  sourceId() const { return fsourceId; }
  unsigned int     adcId() const { return fadcId; }
  unsigned int    chipId() const { return fchipId; }
  unsigned int channelId() const { return fchannelId; }
  time_t    ADCTimeStamp() const { return ftimeStamp; }

  // Modifiers -----------------------
  //*** next four functions added by x. zhang on 08.10.2009 ****//
  void setAmp( int Amp) { famp = Amp; }
  void setSample(unsigned int s) { ft = s; }
  void setPadId(int pi) { fpadId = pi; }
  void setSourceId(unsigned int sId) { fsourceId = sId; }
  void setADCId(unsigned int aId) { fadcId = aId; }
  void setChipId(unsigned int cId) { fchipId = cId; }
  void setChannelId(unsigned int chId) { fchannelId = chId; }
  void setADCTimeStamp(time_t time) { ftimeStamp = time;}
  
  // Operations ----------------------
  void reset();

  /*
  unsigned int nSig() const {
    return fsigs.size();
  }
  void addSig(const PndTpcSignal& d){
    fsigs.push_back(d);
  }
  const PndTpcSignal& getSig(int i) const{
    assert (i<fsigs.size());
    return fsigs.at(i);
  }
  */

private:

  // Private Data Members ------------
  int famp;
  int ft;
  int fpadId;
  unsigned int fsourceId;
  unsigned int fadcId;
  unsigned int fchipId;
  unsigned int fchannelId;     // added by X. Zhang, Oct. 29, 2009     

  time_t ftimeStamp;
  
  //std::vector <PndTpcSignal> fsigs;
  
  McIdCollection fmcId;
  // Private Methods -----------------

public:
  ClassDef(CeeTpcSample,1)

};

#endif
