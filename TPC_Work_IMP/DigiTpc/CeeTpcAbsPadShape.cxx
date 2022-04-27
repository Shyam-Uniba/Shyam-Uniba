// This Class' Header ------------------
#include "CeeTpcAbsPadShape.h"

// C/C++ Headers ----------------------
#include "TPolyLine.h"
#include "TMath.h"
// Collaborating Class Headers --------
#include "CeeTpcPRLookupTable.h"
// Class Member definitions -----------

CeeTpcAbsPadShape::CeeTpcAbsPadShape(const unsigned int fID)
  :lookupTable(0), ID(fID)
{;}
CeeTpcAbsPadShape::~CeeTpcAbsPadShape()
{
  if (lookupTable != 0)
    delete lookupTable;
}

double CeeTpcAbsPadShape::GetValue(const double x, const double z) const
{
  return(lookupTable->GetValue(x,z));
}

bool operator== (const CeeTpcAbsPadShape& lhs, const CeeTpcAbsPadShape& rhs)
{
  return(lhs.ID==rhs.ID);
}

std::ostream& operator<< (std::ostream& s, const CeeTpcAbsPadShape& me)
{
  return s << "CeeTpcAbsPadShape\n"
           << "ID="<<me.ID<<"\n"
           << "lookupTable="<<me.lookupTable<<"\n";
}


void CeeTpcAbsPadShape::Draw(double x, double z, double alpha, int color) const {
  unsigned int n=GetNBoundaryPoints();
  TPolyLine* line=new TPolyLine(n);
  double ca=TMath::Cos(alpha);
  double sa=TMath::Sin(alpha);
  for(unsigned int i=0; i<n; ++i){
    double fx; double fz;
    GetBoundaryPoint(i,fx,fz);
    double u=ca*fx-sa*fz+x;
    double v=sa*fx+ca*fz+z;
    line->SetPoint(i,u,v);
  }
  line->SetLineColor(color);
  line->Draw();
}

