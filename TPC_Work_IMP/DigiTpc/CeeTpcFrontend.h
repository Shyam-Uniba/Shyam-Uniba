//-----------------------------------------------------------
// File and Version Information:
// $Id: CeeTpcFrontend.hh,v 1.6 2006/03/10 15:57:17 sneubert Exp $
//
// Description:
//      TPC Digitization
//      Environment class for TPC frontend (Amplifier/Shaper/ADC/DSP)
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

#ifndef CEETPCFRONTEND_HH
#define CEETPCFRONTEND_HH

// Base Class Headers ----------------


// Collaborating Class Headers -------
#include <ostream> // remove if you do not need streaming op

// Collaborating Class Declarations --

class CeeTpcFrontend {
public:

  // Constructors/Destructors ---------
  CeeTpcFrontend(const double Tint,
	      const double Tdiff,
	      const double Tsig,
	      const double adcThreshold, 
	      const double Adcmax,
	      const int Adcbits,
	      const double samplingfreq_MHz,
	      const double T0,
	      const int timebits=16,
	      const unsigned int PSAthreshold=5); 
  ~CeeTpcFrontend();

  // Operators
  friend std::ostream& operator<< (std::ostream& s, const CeeTpcFrontend& me);

  // Accessors -----------------------
  double adcThreshold() const {return fadcThreshold;}
  double samplingFrequency() const {return 1000./fdt;}
  double t0() const {return ft0;}
  double dt() const {return fdt;}
  double adcbits() const {return fadcbits;}
  double adcmax() const {return fadcmax;}
  double adcstep() const {return fadcstep;}
  unsigned int psaThreshold() const {return fpsaThreshold;}
  double tdiff() const {return ftdiff;}
  double tint() const {return ftint;}
  double tsig() const {return ftsig;}
  // Modifiers -----------------------
  void setAdcThreshold(double newThreshold) { fadcThreshold=newThreshold;}

  // Operations ----------------------
  unsigned int Clock(double const t) const;
  double ClockFine(double const t) const;
  double TimeAtClock(double const t) const;
  double Clock2Time(unsigned int const clock) const;
  unsigned int A2D(double const amp) const;

private:

  // Private Data Members ------------
  double fadcThreshold;
  double fadcmax;         // value at adc input which gives full count
  int fadcbits;           // number of adc bits
  double fadcstep;        // range between two adc counts
  unsigned int fmaxcounts;
  double fdt;              // time between two samples
  double ft0;
  int ftimebits;          // number of bits in timestamp
  unsigned int fmaxsamples;
  unsigned int fpsaThreshold;
  // preamp time constants
  double ftint;
  double ftdiff;
  double ftsig;

  // Private Methods -----------------
  // disallow
  CeeTpcFrontend& operator=(const CeeTpcFrontend&);
  CeeTpcFrontend(const CeeTpcFrontend&);
};

#endif

