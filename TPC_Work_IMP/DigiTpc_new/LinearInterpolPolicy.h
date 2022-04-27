//-------------------------------------------------------------------
// $Id: LinearInterpolPolicy.hh,v 1.3 2005/09/02 15:11:06 sneubert Exp $
//
// Description:
//      Policy-implementation.
//      Provides a method void Interpolate(...) to distribute primary clusters
//      in the TPC between two geant hits.      
//
//      Here a simple linear interpolation is implemented. 
//
//
// Author List:
//    Sebastian Neubert                     TUM      original author
//
//-------------------------------------------------------------------

#ifndef LINEARINTERPOLPOLICY_HH
#define LINEARINTERPOLPOLICY_HH
#include "TClonesArray.h"

class CeeTpcPoint;

struct LinearInterpolPolicy {
  void Interpolate(const CeeTpcPoint* hit1, 
		   const CeeTpcPoint* hit2, 
		   TClonesArray* CLusterList, 
		   unsigned int from, 
		   unsigned int n);
public:
  ClassDef(LinearInterpolPolicy,1)
};

#endif


