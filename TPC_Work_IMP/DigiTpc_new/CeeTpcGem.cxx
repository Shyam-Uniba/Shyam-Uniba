//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Implementation of class CeeTpcGem
//      see CeeTpcGem.h for details
//
// Environment:
//      Software developed for the PANDA Detector at FAIR.
//
// Author List:
//      Sebastian Neubert    TUM            (original author)
//
//
//-----------------------------------------------------------

// This Class' Header ------------------
#include "CeeTpcGem.h"

// C/C++ Headers ----------------------
#include <cmath>

// Collaborating Class Headers --------


// Class Member definitions -----------

CeeTpcGem::CeeTpcGem()
  : fgain(0),fspread_xz(0),fspread_time(0)
{
  fcloudShape = new TF1("CloudShape","gaus(0)",-99,99);
}

CeeTpcGem::CeeTpcGem(const double Gain,
		       const double Spread_xz, const double Spread_time)
  : fgain(Gain),fspread_xz(Spread_xz),fspread_time(Spread_time)
{
  fcloudShape = new TF1("CloudShape","gausn(0)",-99,99);
  fcloudShape->SetParameter(0,1); // normalized gauss
  fcloudShape->SetParameter(1,0);
  fcloudShape->SetParameter(2,Spread_xz);
}

CeeTpcGem::~CeeTpcGem(){}


bool operator== (const CeeTpcGem& lhs, const CeeTpcGem& rhs)
{
  bool bG=fabs(lhs.fgain-rhs.fgain)<1E-3;
  bool bS=fabs(lhs.fspread_xz-rhs.fspread_xz)<1E-8;
  return bG && bS;
}

std::ostream& operator<< (std::ostream& s, const CeeTpcGem& r){
  return s << "\n---------------------------------------- \n "
	   << "CeeTpcGem:\n"
           << "   Gain = "<<r.fgain<<"\n"
           << "   Spread XZ = "<<r.fspread_xz<<"\n"
		   << "   Spread Time = "<<r.fspread_time<<"\n"
	   << "---------------------------------------- \n ";
}

