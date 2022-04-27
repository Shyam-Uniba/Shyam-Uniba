//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Implementation of class PndTpcPadShapePolygon
//      see PndTpcPadShapePolygon.hh for details
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
#include "CeeTpcPadShapePolygon.h"

// C/C++ Headers ----------------------
#include <cmath>

// Collaborating Class Headers --------
#include "TError.h"
#include "CeeTpcPRLookupTable.h"

// Class Member definitions -----------

CeeTpcPadShapePolygon::CeeTpcPadShapePolygon(const std::vector<double>& x,
				       const std::vector<double>& z,
				       const unsigned int fID)
  :CeeTpcAbsPadShape(fID)
{
  if (x.size() != z.size())
    Fatal("CeeTpcPadShapePolygon::CeeTpcPadShapePolygon","The size of x coordinates is different from the size of z coordinates. Constructing a CeeTpcPadShapePolygon is impossible.");
  if (x.size() < 3)
    Fatal("CeeTpcPadShapePolygon::CeeTpcPadShapePolygon:","It is not possible to build a polygon with only %i points.",(int)x.size());
  xCoord = x;
  zCoord = z;
  xCoord.push_back(x[0]);
  zCoord.push_back(z[0]);
  if (!Contains(0,0))
    Fatal("CeeTpcPadShapePolygon::CeeTpcPadShapePolygon","The origin is outside the polygon. (forbidden)");
  lookupTable = new CeeTpcPRLookupTable();
}

CeeTpcPadShapePolygon::~CeeTpcPadShapePolygon()
{
  //lookupTable will automaticly be deleted by ~CeeTpcAbsPadShape()
}

void 
CeeTpcPadShapePolygon::EvalBoundingRect(double& width, double& height,
				     const double angle) const
{
  double xmax=0;
  double zmax=0;
  for(int i=0; i<xCoord.size()-1;i++)
  {
    double xSh = std::fabs( xCoord[i]*std::cos(angle) -
			    zCoord[i]*std::sin(angle) );
    double zSh = std::fabs( xCoord[i]*std::sin(angle) +
			    zCoord[i]*std::cos(angle) );
    if (xSh > xmax)
      xmax = xSh;
    if (zSh > zmax)
      zmax = zSh;
  }
  width=xmax;
  height=zmax; // factor 2 removed because there is no negative coordinate:shyam
}
bool
CeeTpcPadShapePolygon::Contains(const double xp, const double zp) const
{
  //Is (xp, zp) inside the Polygon?
  double xleft;
  int left = 0;
  for (int i=0;i<xCoord.size()-1;i++)
  {
    if (zCoord[i] == zCoord[i+1])
    {
      if (std::fabs(zp-zCoord[i])<1E-5 &&
	  ( (xp-xCoord[i]>-1E-5 && xp-xCoord[i+1]<1E-5) ||
	    (xp-xCoord[i]<1E-5 && xp-xCoord[i+1]>-1E-5) ))
	return(true);//on the boundary
      continue;
    }
    if (zp <= zCoord[i] && zp <= zCoord[i+1]) continue;
    if (zCoord[i] < zp && zCoord[i+1] < zp) continue;
    xleft = xCoord[i] + (zp-zCoord[i])*
      (xCoord[i+1]-xCoord[i])/(zCoord[i+1]-zCoord[i]);
    if (std::fabs(xleft-xp) < 1E-5) return(true);//on the boundary
    if (xp < xleft) left++;
  }
  if (left%2) return true;
  return false;
}

void
CeeTpcPadShapePolygon::GetBoundaryPoint(const int index,
				double& x, double& z) const
{
  x = xCoord.at(index);
  z = zCoord.at(index);
}

std::ostream& operator<< (std::ostream& s,
			  const CeeTpcPadShapePolygon& me)
{
  s <<"CeeTpcPadShapePolygon\n"
    <<"ID="<<me.ID<<"\n"
    <<"lookupTable="<<me.lookupTable<<"\n";
  for (int i=0; i<me.xCoord.size(); i ++)
  {
    s <<"("<<me.xCoord[i]<<","<<me.zCoord[i]<<")";
    if (i%3 == 2)
      s <<"\n";
  }
  s <<"\n";
  return s;
}



