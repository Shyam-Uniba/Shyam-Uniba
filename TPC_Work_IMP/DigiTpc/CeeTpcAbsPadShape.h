//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      The abstract basis class for the shapes used by the pads.
//      Each shape owns a LookupTable.
//
// Environment:
//      Software developed for the PANDA Detector at FAIR.
//
// Author List:
//      Cristoforo Simonetto    TUM            (original author)
//
//
//-----------------------------------------------------------

#ifndef CEETPCABSPADSHAPE_HH
#define CEETPCABSPADSHAPE_HH

// Base Class Headers ----------------


// Collaborating Class Headers -------
#include <ostream>

// Collaborating Class Declarations --
class CeeTpcPRLookupTable;

class CeeTpcAbsPadShape {
public:

  // Constructors/Destructors ---------
  CeeTpcAbsPadShape(const unsigned int fID);
  virtual ~CeeTpcAbsPadShape();

  // Operators
  friend bool operator== (const CeeTpcAbsPadShape& lhs, const CeeTpcAbsPadShape& rhs);
  
  friend std::ostream& operator<< (std::ostream& s, const CeeTpcAbsPadShape& me);

  // Accessors -----------------------
  CeeTpcPRLookupTable* GetLookupTable() const {return(lookupTable);}
  unsigned int GetID() const {return(ID);}
  
  //returns the width and height of an covering rectangle parallel to the axis,
  //if the shape is rotated by angle (in math. direction of rotation)
  virtual void EvalBoundingRect(double& width, double& height, 
				const double angle) const =0;
  //returns if a point (x,z) in the coordinate system of the shape
  //is boundary point
  virtual bool Contains(const double x, const double z) const =0;
  
  //returns the number of points, you can get with GetBoundaryPoint
  virtual int GetNBoundaryPoints() const =0;

  //returns those points on the boundary, which describe the form
  //e.g. a polygon returns the edges,
  //e.g a circle could return points with small distances from each other
  virtual void GetBoundaryPoint(const int index,
				double& x, double& z) const =0;

  //returns the integrated value of the signal caused by an electron
  //avalanche at (x,z) (in the coordinate system of the shape)
  double GetValue(const double x, const double z) const;

  // Modifiers -----------------------
  // The LookupTable should not be modified

  // Draw Pad with this shape at point (x,z) rotated by angle alpha
  virtual void Draw(double x, double z, double alpha, int color) const;

protected:

  CeeTpcPRLookupTable* lookupTable;
  unsigned int ID;
  // Private Methods -----------------

};

#endif
