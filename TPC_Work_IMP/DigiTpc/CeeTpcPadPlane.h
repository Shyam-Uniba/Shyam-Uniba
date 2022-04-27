//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Pad plane in the CeeTpc
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

#ifndef CEETPCPADPLANE_HH
#define CEETPCPADPLANE_HH

// Base Class Headers ----------------



// Collaborating Class Headers -------
#include <ostream> // remove if you do not need streaming op
#include "CeeTpcPad.h"
#include <vector>
#include <map>
#include "math.h"
#include <stdio.h>
#include <Rtypes.h>

// Collaborating Class Declarations --
class CeeTpcPadShapePool;

class CeeTpcPadPlane {
public:

  // Constructors/Destructors ---------
  CeeTpcPadPlane(int nx, 
	      int nz, 
	      double xbin,
	      double zbin,
	      double x0,
	      double z0,
		  double fxmax_padplane,
		  double fxmax_pad);
  CeeTpcPadPlane(const char* const filename,
	      const CeeTpcPadShapePool* const pspool);
  ~CeeTpcPadPlane();

  // Operators
  friend std::ostream& operator<< (std::ostream& s, const CeeTpcPadPlane& me);

  // Accessors -----------------------
  void GetPadList(const double x, 
		   const double z, 
		   const double r, 
		   std::vector<CeeTpcPad*>& buffer) const;
  void GetPadXZ(const unsigned int PadID, 
		double& x, double& z) const;
  CeeTpcPad* GetPad(const unsigned int PadID) const;
  unsigned int GetNPads() const;//the number of pads

  const std::vector<CeeTpcPad*>& GetPads() const {return PadReferences;}

  const unsigned int GetNSectors() const {return fnSectors;}
  std::map<unsigned int,CeeTpcPad*>* GetSectorList(unsigned int sectorId) {return fSectorLists[sectorId];}
  std::vector<unsigned int> GetSectorIds()const;


  // a simple bounding box
  struct bounding_box {
    bounding_box() : minx(999),maxx(-999),minz(999),maxz(-999) {}
    double minx,maxx,minz,maxz;
  };

  bounding_box& GetSector(unsigned int i) {return fSectors[i];}

  const int GetNX() const {return nx;}
  const int GetNZ() const {return nz;}
  const double GetXBin() const {return xbin;}
  const double GetZBin() const {return zbin;}
  const double GetX0() const {return x0;}
  const double GetZ0() const {return z0;}
  const int MaxPadX() const {return (xmax_padplane/xmax_pad);}

  // Modifiers -----------------------

  // Pad Plane will take Ownership!
  void AddPad(CeeTpcPad* pad);
  void ReadFromFile(const char* const filename,
		    const CeeTpcPadShapePool* const pspool);

  // Reserves place in the Mapping vector PadReferences
  void AllocatePadReferences(const int n){PadReferences.resize(n,0);}

  // Operations ----------------------
  struct PadIndexer {
    PadIndexer(int fnx,
	       int fnz,
	       double fxbin,
	       double fzbin,
	       double fx0,
	       double fz0,
		   double fxmax_padplane,
		   double fxmax_pad)
      : nx(fnx),nz(fnz),xbin(fxbin),zbin(fzbin),x0(fx0),z0(fz0),xmax_padplane(fxmax_padplane),xmax_pad(fxmax_pad){;}

    int operator()(CeeTpcPad* p) const
    {
      double res=floor((p->x()-x0)/xbin) + floor((p->z()-z0)/zbin)*nx;
      return (int)res;
    }
    int operator()(double x, double z) const
    {
      double res=floor((x-x0)/xbin) + floor((z-z0)/zbin)*nx;
      return (int)res;
    }
    int nx;
    int nz;
    double xbin;
    double zbin;
    double x0;
    double z0;
    double xmax_padplane;
    double xmax_pad;
  };


  void WriteToStream(std::ostream& stream) const;
  PadIndexer Indexer;

private:
   // Private Data Members ------------
  int nx;      // Number of Regions in x direction
  int nz;      // Number of Regions in z direction
  double xbin; // bin width in x
  double zbin; // bin width in z
  double x0;   // (x0,z0) Origin of plane
  double z0;
  double xmax_padplane; // Max length of padplane in X-direction
  double xmax_pad; // Max length of pad in X-direction

  std::vector< std::vector<CeeTpcPad*>* > Regions; // regions are just 
                                                // for fast searching

  std::vector<CeeTpcPad*> PadReferences;


  std::map<unsigned int,bounding_box> fSectors;  
  std::map<unsigned int,std::map<unsigned int,CeeTpcPad*>* > fSectorLists;
  unsigned int fnSectors; // assume Sectors are enumerated 0...(n-1)
                          // a sector represents the collection of pads
                          // that is read out by one front end processor


  // Private Methods -----------------
  bool ReadLine(char* const line,
		const CeeTpcPadShapePool* const pspool);
public:

  ClassDef(CeeTpcPadPlane,1)

};

// exceptions thrown by this class
#include <exception>
struct unknown_padID : std::exception{
  virtual const char* what() const throw() {
    return "Unkown Pad ID";
  }
};
struct superior_unknown_padId : std::exception{
  superior_unknown_padId(const int padId) : fPadId(padId) {};
  virtual const char* what() const throw() {
    char* bla = new char[50];
    sprintf(bla, "Unknown Pad ID %d", fPadId);
    return bla;
  }
  private:
  int fPadId;
};
struct used_padID : std::exception{
  virtual const char* what() const throw() {
    return "The Pad ID is already assigned.";
  }
}; 



#endif
