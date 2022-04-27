#ifndef CEETPCPSAPLOT_H
#define CEETPCPSAPLOT_H

#include "TGraph.h"
#include "TLine.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TSystem.h"

#include "CeeTpcSample.h"
#include "CeeTpcDigi.h"
#include "CeeTpcSignal.h"
#include "CeeTpcFrontend.h"


#include<vector>
#include<string>

class CeeTpcPSAplot {
 public:


  CeeTpcPSAplot(std::vector<CeeTpcSample*>*,
			 std::vector<CeeTpcDigi*>*,
			 std::vector<CeeTpcSignal*>*,
			 const CeeTpcFrontend* const,
			 std::string);

  ~CeeTpcPSAplot();

  void Draw();

 private:
  TGraph* fsampG;
  TGraph* fdigiG;
  TLine** fdigiLines;
  int fnDigiLines;
  TLine** fsigLines;
  int fnSigLines;
  std::string fname;
};



#endif
