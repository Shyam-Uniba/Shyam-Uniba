//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Inherits from the abstract class CeeTpcAbsPadShape.
//      This class defines the shape of a square pad with the
//      origin in the centre
//
// Environment:
//      Software developed for the PANDA Detector at FAIR.
//
// Author List:
//      Cristoforo Simonetto    TUM            (original author)
//
//
//-----------------------------------------------------------

#ifndef CEETPCPADSHAPESQUARE_HH
#define CEETPCPADSHAPESQUARE_HH

// Base Class Headers ----------------
#include "CeeTpcAbsPadShape.h"

// Collaborating Class Headers -------


// Collaborating Class Declarations --



class CeeTpcPadShapeSquare: public CeeTpcAbsPadShape {
public:

  // Constructors/Destructors ---------
  CeeTpcPadShapeSquare(const double fd,
		       const unsigned int fID);
  virtual ~CeeTpcPadShapeSquare();

  // Operators
  friend std::ostream& operator<<(std::ostream& s,
				  const CeeTpcPadShapeSquare& me);

  // Accessors -----------------------
  virtual void EvalBoundingRect(double& width, double& height,
				const double angle) const;
  virtual bool Contains(const double xp, const double zp) const;
  virtual int GetNBoundaryPoints() const {return(4);};
  virtual void GetBoundaryPoint(const int index,
				double& x, double& z) const;

private:

  // Private Data Members ------------
  double d; //length of a size

  // Private Methods -----------------

};

#endif
