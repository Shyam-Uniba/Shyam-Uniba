//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Inherits from the abstract class PndTpcAbsPadShape
//      This class defines the shape of a pad by a vector of ordered points.
//      The user should to take care that the origin is almost circa in the
//      centre of the polygon (origins outside the polygon are forbidden).
//
// Environment:
//      Software developed for the PANDA Detector at FAIR.
//
// Author List:
//      Cristoforo Simonetto    TUM            (original author)
//
//
//-----------------------------------------------------------

#ifndef CEETPCPADSHAPEPOLYGON_HH
#define CEETPCPADSHAPEPOLYGON_HH

// Base Class Headers ----------------
#include "CeeTpcAbsPadShape.h"

// Collaborating Class Headers -------
#include <vector>

// Collaborating Class Declarations --


class CeeTpcPadShapePolygon: public CeeTpcAbsPadShape {
public:

  // Constructors/Destructors ---------
  CeeTpcPadShapePolygon(const std::vector<double>& x,
		     const std::vector<double>& z,
		     const unsigned int fID);
  virtual ~CeeTpcPadShapePolygon();

  // Operators
  friend std::ostream& operator<< (std::ostream& s,
				  const CeeTpcPadShapePolygon& me);

  // Accessors -----------------------
  
  // Modifiers -----------------------
  // Once a CeeTpcPadShapePolygon is constructed it should not be modified.

  // Operations
  virtual void EvalBoundingRect(double& width, double& height,
				const double angle) const;
  virtual bool Contains(const double xp, const double zp) const;
  virtual int GetNBoundaryPoints() const {return(xCoord.size()-1);};
  virtual void GetBoundaryPoint(const int index,
				double& x, double& z) const;

private:

  // Private Data Members ------------
  std::vector<double> xCoord;              //Coordinates of the polygon in the
  std::vector<double> zCoord;              //right order. The last is the first
  
  // Private Methods -----------------
  
};

#endif
