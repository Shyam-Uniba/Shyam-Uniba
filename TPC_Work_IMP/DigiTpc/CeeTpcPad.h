//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      PndTpc readout pad
//      Geometry definition for PndTpc Digitization
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

#ifndef CEETPCPAD_HH
#define CEETPCPAD_HH

// Base Class Headers ----------------


// Collaborating Class Headers -------
#include <ostream> // remove if you do not need streaming op
#include <vector>
#include <Rtypes.h>
// Collaborating Class Declarations --
class CeeTpcAbsPadShape;


class CeeTpcPad {
public:

  // Constructors/Destructors ---------
  CeeTpcPad();
  CeeTpcPad(const double x, const double z, const double angle,
	 CeeTpcAbsPadShape* const shape,
	 const unsigned int sectorID,
	 const unsigned int ID);
  ~CeeTpcPad(){;}

  // Operators
  friend bool operator== (const CeeTpcPad& lhs, const CeeTpcPad& rhs);
  friend bool operator< (const CeeTpcPad& lhs, const CeeTpcPad& rhs);
  friend std::ostream& operator<< (std::ostream& s, const CeeTpcPad& me);

  // Accessors -----------------------
  double x() const {return fx;}
  double z() const {return fz;}
  double angle() const {return fangle;}
  double pos(const int i) const {
    if(i==0) return fx;
    else if(i==1) return fz;
    else if(i==2) return fangle;
    else throw;}
  CeeTpcAbsPadShape* shape() const {return fshape;}
  unsigned int sectorId() const {return fsectorId;};
  unsigned int padId() const {return fid;}
  unsigned int id() const{return fid;} 
  // Modifiers -----------------------
  void setx(const double X){fx=X;}
  void setz(const double Z){fz=Z;}
  void setangle(const double Angle){
    fangle=Angle;
    EvalBoundingRect();}
  void setshape(CeeTpcAbsPadShape* const Shape){
    fshape = Shape;
    EvalBoundingRect();}
  void setsectorId(const unsigned int Id){fsectorId=Id;}
  void addNeighbour(const unsigned int& Id){fneighbourIds.push_back(Id);}
  bool removeNeighbour(unsigned int Id);
  unsigned int nNeighbours() const {return fneighbourIds.size();}
  unsigned int getNeighbour(const unsigned int& Id) const {return fneighbourIds[Id];}

  // Operations ----------------------
  bool Contains(const double X, const double Z) const;
  bool CircleIntersection(const double X,
			  const double Z,
			  const double r) const;
  double GetValue(const double X, const double Z) const;
  //returns the number of points, you can get with GetBoundaryPoint
  int GetNBoundaryPoints() const;
  //returns those points on the boundary, which describe the form
  void GetBoundaryPoint(const int index,
			double& X, double& Z) const;
  void Draw(int c=1) const;

private:

  // Private Data Members ------------
  // Center of gravity
  double fx;
  double fz;
  //angle (in math. direction of rotation, in rad) between padplane and shape
  double fangle;
  //The minimal sizes for a bounding box with the origin in the centre
  double fwidth;
  double fheight;

  CeeTpcAbsPadShape* fshape;
  unsigned int fsectorId;
  unsigned int fid;

  std::vector<unsigned int> fneighbourIds;

  // Private Methods -----------------
  void ToShapeCoord(double& xP, double& zP) const;
  void ToPlaneCoord(double& xP, double& zP) const;
  void EvalBoundingRect();
 public:
    ClassDef(CeeTpcPad,1)
};

#endif

//--------------------------------------------------------------
// $Log$
//--------------------------------------------------------------

