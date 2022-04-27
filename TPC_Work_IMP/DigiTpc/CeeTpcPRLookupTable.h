//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      for PndTpc Digitization
//      Lookup table for the integration of the signal induction
//      on the pads
//
//
// Environment:
//      Software developed for the PANDA Detector at FAIR.
//
// Author List:
//      Sebastian Neubert    TUM            (original author)
//      Cristoforo Simonetto TUM
//
//
//-----------------------------------------------------------

#ifndef CEETPCPRLOOKUPTABLE_HH
#define CEETPCPRLOOKUPTABLE_HH

// Base Class Headers ----------------


// Collaborating Class Headers -------
#include <iostream> // remove if you do not need streaming op

// Collaborating Class Declarations --
class CeeTpcGem;
class CeeTpcAbsPadShape;


class CeeTpcPRLookupTable {
public:

  // Constructors/Destructors ---------
  CeeTpcPRLookupTable();
  ~CeeTpcPRLookupTable();

  // Operators
  friend std::ostream& operator<< (std::ostream& s,
				   const CeeTpcPRLookupTable& me);

  // Accessors -----------------------
  // (dx,dz) = vector from pad to avalanche!
  double GetValue(const double dx, const double dz) const;
  bool IsBuilt() const {return isbuilt;}
  int GetNStepsX() const {return 2*nx;} // Number of steps (-nx,nx) in X direction
  int GetNStepsZ() const {return 2*nz;} // Number of steps (-nz,nz) in Z direction
  double GetStep() const {return step;}

  // Modifiers -----------------------
  
  // Operations ----------------------
  void BuildTable(const CeeTpcGem& a, 
		  const CeeTpcAbsPadShape& pad, 
		  const double frange,
		  const double fstep,
		  const double fintStep);

private:

  // Private Data Members ------------
  bool isbuilt;
  // the absolute range is the width/height of PndTpcPadPolygon +2*range
  // (in neg. and pos. direction => *2)
  double range; // dummy at the moment
  double step;   //step between two interpolation points
  int nx;        // number of steps in pos. x direction (total #=nx)
  int nz;        // number of steps in pos. z direction (total #=ny)
  double** table;

  // Private Methods -----------------
  double Integrate(const CeeTpcGem& gem, 
		   const CeeTpcAbsPadShape& p,
		   const double intStep,
		   const double dx,      
		   const double dz) const;

};

#endif

