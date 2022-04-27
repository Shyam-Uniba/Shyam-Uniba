//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//     CeeTpc readout Trapezoid pad design: shyam kumar
//
//
//-----------------------------------------------------------

#ifndef CEETPCPAD_Trap_HH
#define CEETPCPAD_Trap_HH

// Collaborating Class Headers -------
#include <vector>
#include <Rtypes.h>
#include <TVector2.h>

class CeeTpcPad_Trap {
public:

  // Constructors/Destructors ---------
  CeeTpcPad_Trap();
  CeeTpcPad_Trap(const double x1, const double z1,
		         const double x2, const double z2,
				 const double x3, const double z3,
				 const double x4, const double z4,
	             const unsigned int sectorID,
	             const unsigned int padID,
				 const double angle);

  ~CeeTpcPad_Trap(){
	  fgx.clear(); fgz.clear();
	  flx.clear(); flz.clear();
  }


  // Accessors -----------------------
  std::vector<Double_t> GetGlobalXCoordinates() const {return fgx;}
  std::vector<Double_t> GetGlobalZCoordinates() const {return fgz;}
  std::vector<Double_t> GetLocalXCoordinates() const {return flx;}
  std::vector<Double_t> GetLocalZCoordinates() const {return flz;}
  unsigned int GetSectorId() const {return fSectorId;};
  unsigned int GetPadId() const {return fPadId;}
  unsigned int GetRowId() const {return fRowId;}
  unsigned int GetPadInRowId() const {return fPadInRowId;}
  double GetAngle() const {return fAngle;}
  TVector2 GetGlobalCenterPosition(){return fGCenter;}
  TVector2 GetLocalCenterPosition(){return fLCenter;}

  // Modifiers -----------------------
  //Setting coordinates of a Pad Global and Local
  void SetGlobalCoordinates(Double_t* x, Double_t* z){
      for(int i=0;i<4;i++){
          fgx.push_back(x[i]);
          fgz.push_back(z[i]);
      }
  }
  void SetLocalCoordinates(Double_t* x, Double_t* z){
      for(int i=0;i<4;i++){
          flx.push_back(x[i]);
          flz.push_back(z[i]);
      }
  }

  void SetAngle(const double Angle){fAngle=Angle;}
  void SetSectorId(const unsigned int Id){fSectorId=Id;}
  void SetRowId(const unsigned int Id){fRowId=Id;}
  void SetPadInRowId(const unsigned int Id){fPadInRowId=Id;}
  void SetGlobalCenterPosition(double x, double y){fGCenter.SetX(x); fGCenter.SetY(y);}
  void SetLocalCenterPosition(double x, double y) {fLCenter.SetX(x); fLCenter.SetY(y);}

  // Operations ----------------------
  void Draw(int color=1) const;

private:

  // Private Data Members ------------
  std::vector<Double_t>  fgx, flx;      // X coorinates of a Pad
  std::vector<Double_t>  fgz, flz;      // Z coorinates of a Pad
  TVector2 fGCenter, fLCenter; // Global and Local centeriod
  double fAngle; // Angle of Rotation of Pad
  unsigned int fSectorId; // Sector Id 4-Sectors 0, 1, 2, 3
  unsigned int fPadId; // Pad Number
  unsigned int fRowId;  // Pad-Row Id
  unsigned int fPadInRowId; // Pad Position in a Row
public:
    ClassDef(CeeTpcPad_Trap,1)
};

#endif

//--------------------------------------------------------------
// $Log$
//--------------------------------------------------------------


