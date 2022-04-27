//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Implementation of class PndTpcPad
//      see PndTpcPad.h for details
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

// This Class' Header ------------------
#include "CeeTpcPad.h"
#include <iostream>
// C/C++ Headers ----------------------
#include <cmath>

// Collaborating Class Headers --------
#include "CeeTpcAbsPadShape.h"
#include "TError.h"

// Class Member definitions -----------

CeeTpcPad::CeeTpcPad()
  : fx(0),fz(0),fangle(0),fwidth(0),fheight(0),fshape(0),fsectorId(0),fid(0)
{;}

CeeTpcPad::CeeTpcPad(const double X,
	       const double Z,
	       const double Angle,
	       CeeTpcAbsPadShape* const Shape,
	       const unsigned int sectorID,
	       const unsigned int ID)
  : fx(X),fz(Z),fangle(Angle),fshape(Shape),fsectorId(sectorID),fid(ID)
{
  EvalBoundingRect();
}

bool CeeTpcPad::Contains(const double X, const double Z) const
{
  if (fshape == 0)
    Fatal("CeeTpcPad::Contains","No AbsPadShapePolygon assigned.");
  double xSh = X;
  double zSh = Z;
  ToShapeCoord(xSh, zSh);
  return(fshape->Contains(xSh, zSh));
}

bool CeeTpcPad::CircleIntersection(const double X, const double Z,
			   const double r) const
{
  double dx=std::fabs(fx-X); double dz=std::fabs(fz-Z);
  double hw = 0.5*fwidth; double hh = 0.5*fheight;
  //most Pads are far away
  if (dx > hw+r || dz > hh+r )
    return(false);
  return ( (dx < hw+r && dz < hh) || (dx < hw && dz < hh+r) ||
	   (dx-hw)*(dx-hw)+(dz-hh)*(dz-hh) < r*r );
}

double CeeTpcPad::GetValue(const double X, const double Z) const
{
  if (fshape == 0)
    Fatal("CeeTpcPad::GetValue","No AbsPadShape assigned.");
  double xSh = X;
  double zSh = Z;
  ToShapeCoord(xSh, zSh);
  return(fshape->GetValue(xSh, zSh));
}

int CeeTpcPad::GetNBoundaryPoints() const
{
  if (fshape == 0)
    Fatal("CeeTpcPad::GetNBoundaryPoints","No AbsPadShape assigned.");
  return(fshape->GetNBoundaryPoints());
}

void CeeTpcPad::GetBoundaryPoint(const int index, double& X, double& Z) const
{
  if (fshape == 0)
    Fatal("CeeTpcPad::GetBoundaryPoint","No AbsPadShape assigned.");
   fshape->GetBoundaryPoint(index, X, Z);
   ToPlaneCoord(X, Z);
}

bool operator== (const CeeTpcPad& lhs, const CeeTpcPad& rhs)
{
  //true if same position, angle and shape
  double dx=lhs.fx-rhs.fx;
  double dz=lhs.fz-rhs.fz;
  double da=lhs.fangle-rhs.fangle;
  return lhs.fshape==rhs.fshape &&
    (dx*dx<1E-10) && (dz*dz<1E-10) && (da*da<1E-10);
}

bool operator< (const CeeTpcPad& lhs, const CeeTpcPad& rhs){
  //true if lhs below (y) or left (x) of rhs
  double dz=lhs.fz-rhs.fz;
  double dx=lhs.fx-rhs.fx;
  bool flag=(dz*dz<1E-10) && (dx<-1E-5);
    return (dz<-1E-5) || flag;
}

std::ostream& operator<< (std::ostream& s, const CeeTpcPad& me){
  return s << "CeeTpcPad\n"
           << "sectorID="<<me.fsectorId<<"\n"
	   << "ID="<<me.fid<<"\n"
	   << "x="<<me.fx<<"    z="<<me.fz<<"    angle="<<me.fangle<<"\n"
           << "shape="<<me.fshape<<"\n";
}

void CeeTpcPad::ToShapeCoord(double& xP, double& zP) const
{
  double xori=xP-fx;
  double zori=zP-fz;
  xP = xori*std::cos(fangle) + zori*std::sin(fangle);
  zP = -xori*std::sin(fangle) + zori*std::cos(fangle);
}

void CeeTpcPad::ToPlaneCoord(double& xP, double& zP) const
{
  double xori = xP*std::cos(-fangle) + zP*std::sin(-fangle);
  zP = -xP*std::sin(-fangle) + zP*std::cos(-fangle)+fz;
  xP = xori+fx;
}

void CeeTpcPad::EvalBoundingRect()
{
  if (fshape == 0)
  {
    fwidth = 0;
    fheight =0;
    return;
  }
  fshape->EvalBoundingRect(fwidth, fheight, fangle);
}

void CeeTpcPad::Draw(int c) const
{
  fshape->Draw(fx,fz,fangle, c);
}

bool CeeTpcPad::removeNeighbour(unsigned int n) {
  std::vector<unsigned int>::iterator it;
  for(it=fneighbourIds.begin(); it!=fneighbourIds.end(); it++) 
    if(*it==n) {
      fneighbourIds.erase(it);
      return false; //no error
    }
  return true; //not found
}
ClassImp(CeeTpcPad)

