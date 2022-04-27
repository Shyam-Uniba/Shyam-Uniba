//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Implementation of class PndTpcPadShapeSquare
//      see CeeTpcPadShapeSquare.hh for details
//
// Environment:
//      Software developed for the PANDA Detector at FAIR.
//
// Author List:
//      Cristoforo Simonetto    TUM            (original author)
//
//
//-----------------------------------------------------------
// This Class' Header ------------------
#include "CeeTpcPadShapeSquare.h"

// C/C++ Headers ----------------------
#include <cmath>
#include <exception>

// Collaborating Class Headers --------
#include "CeeTpcPRLookupTable.h"

// Class Member definitions -----------

CeeTpcPadShapeSquare::CeeTpcPadShapeSquare(const double fd,
				     const unsigned int fID)
  :CeeTpcAbsPadShape(fID),d(fd)
{
  lookupTable = new CeeTpcPRLookupTable();
}

CeeTpcPadShapeSquare::~CeeTpcPadShapeSquare()
{
  //lookupTable will automaticly be deleted by ~CeeTpcAbsPadShape()
}

void 
CeeTpcPadShapeSquare::EvalBoundingRect(double& width, double& height,
				    const double angle) const
{
  width = std::fabs( d*std::cos(angle) - d*std::sin(angle));
  height = std::fabs( d*std::cos(angle) + d*std::sin(angle));
  if (width>height)
    height=width;
  else
    width=height;
}

bool
CeeTpcPadShapeSquare::Contains(const double xp, const double zp) const
{
  return( std::fabs(xp)<=d &&
	  std::fabs(zp)<=d);
}

void
CeeTpcPadShapeSquare::GetBoundaryPoint(const int index,
				double& x, double& z) const
{
  switch(index)
  {
  case 0: x=  (z= d/2); break;
  case 1: x= -(z= -d/2); break;
  case 2: x=  (z=-d/2); break;
  case 3: x= -(z=d/2); break;
  default: throw "index out of range";
  }
}

std::ostream& operator<< (std::ostream& s,
			 const CeeTpcPadShapeSquare& me)
{
  return s <<"CeeTpcPadShapeSquare\n"
           <<"ID="<<me.ID<<"\n"
           <<"lookupTable="<<me.lookupTable<<"\n"
           <<"d="<<me.d<<"\n";
  
}

