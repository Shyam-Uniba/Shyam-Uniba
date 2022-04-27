//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Data class carrying information about GEMs  
//      in the TPC
//
// Environment:
//      Software developed for the PANDA Detector at FAIR.
//
// Author List:
//      Sebastian Neubert    TUM            (original author)
//
//
//-----------------------------------------------------------

#ifndef CEETPCGEM_HH
#define CEETPCGEM_HH

// Base Class Headers ----------------


// Collaborating Class Headers -------
#include <ostream>
#include "TF1.h"

// Collaborating Class Declarations --

class CeeTpcGem {
public:

  // Constructors/Destructors ---------
  CeeTpcGem();
  CeeTpcGem(const double Gain, const double Spread_xy, const double Spread_time);
  ~CeeTpcGem();

  // Operators
  friend bool operator== (const CeeTpcGem&,const CeeTpcGem&);
  friend std::ostream& operator<<(std::ostream&, const CeeTpcGem&);

  // Accessors -----------------------
  double gain() const {return fgain;}
  double spread_xz() const {return fspread_xz;}
  double spread_time() const {return fspread_time;}

  const TF1* cloudShape() const {return fcloudShape;}

  void PrintAll(std::ostream& s) const {s<<*this;}

  // Modifiers -----------------------
  void setgain(const double G){fgain=G;}
  void setspread_xz(const double S){fspread_xz=S;}
  void setspread_time(const double St){fspread_time=St;}

private:

  // Private Data Members ------------
  double fgain;
  double fspread_xz; // Spread in XZ direction
  double fspread_time; // Spread in time
  
  TF1* fcloudShape;
  
  // Private Methods -----------------
  bool operator<  (const CeeTpcGem&) const; // forbidden, CeeTpcGem
                                         // is not an ordered type
};




#endif
