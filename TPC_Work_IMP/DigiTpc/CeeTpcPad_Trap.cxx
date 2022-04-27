//-----------------------------------------------------------
// File and Version Information:
//-----------------------------------------------------------

// This Class' Header ------------------
#include "CeeTpcPad_Trap.h"
#include <iostream>
// C/C++ Headers ----------------------
#include <cmath>
#include "TError.h"

// Class Member definitions -----------

CeeTpcPad_Trap::CeeTpcPad_Trap()
  : fgx(),fgz(),flx(),flz(),fSectorId(0),fPadId(0),fAngle(0),
	fGCenter(0.,0.),fLCenter(0.,0.), fRowId(0), fPadInRowId(0)
{;}

CeeTpcPad_Trap::CeeTpcPad_Trap(const double x1, const double z1,
		         const double x2, const double z2,
				 const double x3, const double z3,
				 const double x4, const double z4,
	             const unsigned int sectorID,
	             const unsigned int padID,
				 const double angle)
  :fAngle(angle),fSectorId(sectorID),fPadId(padID)
{
	fgx.push_back(x1); fgx.push_back(x2); fgx.push_back(x3); fgx.push_back(x4);
	fgz.push_back(z1); fgz.push_back(z2); fgz.push_back(z3); fgz.push_back(z4);
}


void CeeTpcPad_Trap::Draw(int color) const
{
  Double_t x[5], z[5];
  x[0] = fgx[0]; x[1] = fgx[1]; x[2] = fgx[2]; x[3] = fgx[3]; x[4] = fgx[0];
  z[0] = fgz[0]; z[1] = fgz[1]; z[2] = fgz[2]; z[3] = fgz[3]; z[4] = fgz[0];
  TPolyLine *line = new TPolyLine(5,x,z);
  line->SetFillColor(color);
  line->SetLineColor(color);
  line->SetLineWidth(1);
  line->Draw();
}


ClassImp(CeeTpcPad_Trap)


