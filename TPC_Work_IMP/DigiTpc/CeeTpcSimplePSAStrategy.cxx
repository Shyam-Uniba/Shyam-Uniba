//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Implementation of class CeeTpcSimplePSAStrategy
//      see CeeTpcSimplePSAStrategy.hh for details
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
#include "CeeTpcSimplePSAStrategy.h"

// C/C++ Headers ----------------------
#include "assert.h"
#include <algorithm>
#include <iostream>


using std::max;

// Collaborating Class Headers --------
#include "CeeTpcSample.h"
#include "CeeTpcDigi.h"

// Class Member definitions -----------
CeeTpcSimplePSAStrategy::CeeTpcSimplePSAStrategy(const unsigned int threshold)
  : CeeTpcAbsPSAStrategy(), fcurrentPadID(0), finprogress(false),
    famp(0),ft(0),fthreshold(threshold)
{}


CeeTpcDigi*
CeeTpcSimplePSAStrategy::ProcessNext(const CeeTpcSample* sample)
{
  unsigned int newPadID=sample->padId();
  unsigned int newamp=sample->amp();
  //  std::cout << newPadID << " " << newamp << std::endl;
  if(finprogress){
    if(newPadID==fcurrentPadID && newamp>=fthreshold){
      if(newamp>famp){
	famp=newamp;
	//ft=sample->t();
      }
      fmcid.AddIDCollection(sample->mcId(),newamp);
    }
    else{
      CeeTpcDigi* digi=new CeeTpcDigi(famp,ft,fcurrentPadID,fmcid);
      if(newamp>=fthreshold){ // start new pulse on different pad
		fcurrentPadID=newPadID;
		famp=sample->amp();
		ft=sample->t();
      }
      else finprogress=false;
      return digi;
    }
  }
  else if(newamp>fthreshold){ // start new pulse!
    fcurrentPadID=newPadID;
    famp=newamp;
    ft=sample->t();
    fmcid.ClearData();
    fmcid.AddIDCollection(sample->mcId(),newamp);
    finprogress=true;
  }
  return 0;
}

void CeeTpcSimplePSAStrategy::Process(const std::vector<CeeTpcSample*> & samples, 
								   std::vector<CeeTpcDigi*>& digis,
								   double padThreshold) {

  /*
  for(int i=0;i<samples.size();i++) {
	std::cout << samples[i]->padId() << " " << samples[i]->amp() << std::endl;
  }
  */
	std::cout << "Using Simple PSA strategy!" << std::endl;
  int startIndex;
  
  for(int i=0;i<samples.size();i++) {
    unsigned int newPadID=samples[i]->padId();
    unsigned int newamp=samples[i]->amp();

    if(finprogress){
      if(newPadID==fcurrentPadID && newamp>=fthreshold){
        if(newamp>famp){
          famp=newamp;
      }
      fmcid.AddIDCollection(samples[i]->mcId(),newamp);
      }
      else{
      CeeTpcDigi* digi=new CeeTpcDigi(famp,ft,fcurrentPadID,fmcid);
      digi->tlength(samples[i]->t()-ft);
      finprogress=false;

      digis.push_back(digi);
      }
    }
    else if(newamp>fthreshold){ // start new pulse!
      fcurrentPadID=newPadID;
      famp=newamp;
      ft=samples[i]->t();
      fmcid.ClearData();
      fmcid.AddIDCollection(samples[i]->mcId(),newamp);
      startIndex = i;
      finprogress=true;
    }
	
	
  }

}


