//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Implementation of class CeeTpcPRLookupTable
//      see CeeTpcPRLookupTable.h for details
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
#include "CeeTpcPRLookupTable.h"

// C/C++ Headers ----------------------

// Collaborating Class Headers --------
#include "CeeTpcAbsPadShape.h"
#include "CeeTpcGem.h"
#include "TError.h"
#include "TMath.h"
// Class Member definitions -----------

CeeTpcPRLookupTable::CeeTpcPRLookupTable()
  : isbuilt(false),range(0),step(0),nx(0),nz(0),table(0)
{;}


CeeTpcPRLookupTable::~CeeTpcPRLookupTable()
{
  if(table!=0)
  {
    for(int i=0;i<nx;++i)
      delete [] table[i];
    delete [] table;
  }
}

double CeeTpcPRLookupTable::GetValue(const double dx, const double dz) const{
  if(!isbuilt)return 0;
  int ilx=(int)TMath::Floor(dx/step);
  int ilz=(int)TMath::Floor(dz/step);
  int iux=ilx+1;
  int iuz=ilz+1;
  double lx=ilx*step;  // lower x
  double lz=ilz*step;  // lower z
  double ux=iux*step;  // upper x
  double uz=iuz*step;  // upper z
  
  if(ilx<-nx || ilx>=nx){
	//Warning("CeeTpcPRLookupTable::GetValue","ilx out of range=%f",(double)ilx);
	  return 0;
  }
  if(ilz<-nz || ilz>=nz){
	//Warning("CeeTpcPRLookupTable::GetValue","ilz out of range=%f",(double)ilz);
	return 0;
  }
  if(iux<-nx || iux>=nx){
  // Warning("CeeTpcPRLookupTable::GetValue","iux out of range=%f",(double)iux);
   return 0;
  }
  if(iuz<-nz || iuz>=nz){
   //Warning("CeeTpcPRLookupTable::GetValue","iuz out of range=%f",(double)iuz);
   return 0;
  }

  //bilinear interpolation:
  double t=(dx-lx)/(ux-lx);
  double u=(dz-lz)/(uz-lz);
  double result=(1-t)*(1-u)*table[ilx+nx][ilz+nz];
  result+=t*(1-u)*table[iux+nx][ilz+nz];
  result+=t*u*table[iux+nx][iuz+nz];
  result+=(1-t)*u*table[ilx+nx][iuz+nz];
  return result;
}

void CeeTpcPRLookupTable::BuildTable(const CeeTpcGem& gem,
			     const CeeTpcAbsPadShape& pad,
			     const double frange,
			     const double fstep,
			     const double fintStep){
  if(table!=0){
    for(int i=0;i<nx;++i){
      delete [] table[i];
    }
    delete [] table;
  }
  range=frange; step=fstep;
  double padwidth;
  double padheight;
  pad.EvalBoundingRect(padwidth, padheight, 0);
  // nx and nz can be set by looking the distribuition of Avlanche spread
  nx=(int)ceil( (1.75 + 0.5*padwidth) /step);//Maximum Pad fired can be 4 pads = 2 regions = 4*0.5 = 2.0 cm
  nz=(int)ceil( (1.8 + 0.5*padheight) /step);//Maximum Pad fired can be 2 pads = 2 regions = 2*1.2 = 2.4 cm
  //build table
  table=new double*[2*nx];
  for(int i=0;i<2*nx;++i){
    table[i]=new double[2*nz];
  }
  std::ostream& s = std::cerr;
  //loop over table
  s<<"Building TPC LookupTable ";

  for(int ix=-nx;ix<nx;++ix){
    if(ix%10 == 0) s<<".";
    for(int iz=-nz;iz<nz;++iz){
      double thisx=ix*step;
      double thisz=iz*step;
      table[ix+nx][iz+nz]=Integrate(gem,pad,fintStep,thisx,thisz);
    }
  }
  s<<std::endl;
  isbuilt=true;
}


double 
CeeTpcPRLookupTable::Integrate(const CeeTpcGem& gem, 
			    const CeeTpcAbsPadShape& p,
			    const double intStep,
			    const double dx,      
			    const double dz) const 
{
  double padwidth;
  double padheight;
  p.EvalBoundingRect(padwidth, padheight, 0);
  int nintx=(int)floor(padwidth/intStep);
  int nintz=(int)floor(padheight/intStep);
  double sum=0;
 // double offsetx=0.5*(padwidth-intStep); // No offset in our padplane
 // double offsetz=0.5*(padheight-intStep); // No offset in our padplane
  double offsetx=0;
  double offsetz=0;
  //initialize Avalanche
  for(int ix=0;ix<nintx;++ix){
    for(int iz=0;iz<nintz;++iz)
      {
	if (!p.Contains(ix*intStep-offsetx,iz*intStep-offsetz)) continue;
      double thisx=ix*intStep-offsetx-dx;
      double thisz=iz*intStep-offsetz-dz;
      double r=sqrt(thisx*thisx+thisz*thisz);
      sum+=gem.cloudShape()->Eval(r)*1/gem.spread_xz()*1/sqrt(2*3.1416); // TODO: cache these values!!!
    }
  }

  //normalize to area
  double s2=intStep*intStep;
  sum*=s2;
  return sum;
}


