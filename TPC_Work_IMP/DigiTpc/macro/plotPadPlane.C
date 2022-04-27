#include <iostream>
#include <vector>
#include <sstream>
#include "TLatex.h"
#include "TCanvas.h"
#include "TVector2.h"
#include "TArrow.h"
#include "TWbox.h"
#include "TPad.h"
#include "TH2D.h"
#include "TStyle.h"

void plotPadPlane(){

TString padshapes ="Hexagons0.15.dat"; // 2mmPads.dat, Hexagons.dat Pad Shapes
TString padplane ="padplane.dat"; // PadPlane
Bool_t drawlinks=false;
Bool_t drawPadIDs=false;

CeeTpcGem* gem=new CeeTpcGem(2000, 0.028, 14.9); // Gain and Spread sigmaxz = 0.028, sigmat=14.9 ns
  
CeeTpcPadShapePool* _padShapes = new CeeTpcPadShapePool(padshapes);
  //				   *_gem,
  //				   0.4, // lookup range
				     //			   0.02, // Lookup Step
  //				   0.01); // LookupIntegrationStep
  
  
  CeeTpcPadPlane* _padPlane= new CeeTpcPadPlane(padplane, _padShapes);
  
  
  std::cout<<*_padPlane<<std::endl;

  TCanvas* c=new TCanvas("c","regions",10,10,800,800);
  double x0=_padPlane->GetX0();
  double z0=_padPlane->GetZ0();
  double x1=x0+_padPlane->GetXBin()*_padPlane->GetNX();
  double z1=z0+_padPlane->GetZBin()*_padPlane->GetNZ();
  
  std::cout<<x0<<"  "<<x1<<"  "<<z0<<"  "<<z1<<std::endl;

  gPad->Range(x0,z0,x1,z1);

  gStyle->SetOptStat(0);
  
  TH2D* hist = new TH2D("gsfdg","",100,x0,x1,100,z0,z1);
  hist->Draw();
  hist->GetXaxis()->SetTitle("X-direction");
  hist->GetYaxis()->SetTitle("Z-direction");
  
  // plot regions
  int npads=_padPlane->GetNPads(); // Total Pads = 135168
  std::vector<TVector2> regmin(1000);
  std::vector<TVector2> regmax(1000);

  
  for(int i=0;i<npads;++i){
    CeeTpcPad* apad=0;
    try{
      apad=_padPlane->GetPad(i);
    }
    catch (std::exception &e){
      cout << e.what() << endl;
      cout.flush();
      continue;
    }
    if(drawPadIDs) {
      stringstream ss;
      ss<<i;
      
      TLatex* tex = new TLatex();
      tex->SetTextSize(0.007);
      tex->SetTextAlign(22);
      tex->DrawLatex(apad->x(), apad->z(), ss.str().c_str());
    }
    
    int sectorid=apad->sectorId();
    apad->Draw(sectorid%10+1);
    unsigned int nn=apad->nNeighbours();
    // draw clusterizer links
    if(drawlinks){
      for(unsigned int in=0;in<nn;++in){
	CeeTpcPad* neigh=_padPlane->GetPad(apad->getNeighbour(in));
	TArrow* link=new TArrow(apad->x(),apad->z(),neigh->x(),neigh->z(),0.001,"-|>-");
	link->SetFillColor(1);
	link->Draw();
      }
    }
  }
  
  c->Update();
  c->Draw();

  cout << "Sectors for x>0:" << endl;
  std::vector<unsigned int> sectors=_padPlane->GetSectorIds();
  unsigned int ns=sectors.size();
  unsigned int nselected=0;
  for(unsigned int is=0;is<ns;++is){
    CeeTpcPadPlane::bounding_box bx=_padPlane->GetSector(sectors[is]);
    if(bx.minx>0){
      cout << "," << is;
      ++nselected;
    }
  }
  cout << endl << nselected << " out of " << ns << " sectors." << endl;

  c->SaveAs("padplane.eps");
}

