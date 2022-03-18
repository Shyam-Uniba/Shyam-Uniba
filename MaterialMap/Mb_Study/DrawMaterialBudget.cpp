void DrawMaterialBudget(const char* name="detector_geometry.root"){
	TGeoManager::Import(name);
	string filename= name;
	const size_t period_idx = filename.rfind('.');
  if (std::string::npos != period_idx)
  {
    filename.erase(period_idx);
  }

   TObjArray* allvolumes = gGeoManager->GetListOfVolumes();
  //  gGeoManager->GetListOfNodes()->Print();
  //  gGeoManager->GetListOfVolumes()->Print();
    for(Int_t i=0; i<allvolumes->GetEntries();i++){
          TGeoVolume* vol= (TGeoVolume*)allvolumes->At(i);
          TString volname = vol->GetIconName();
          cout<<"Volume No: "<<i<<"   Name: "<<volname<<endl;
          //if (volname.Contains("SVTX")) vol->SetLineColor(kRed); 	
          //else if (volname.Contains("BARR")) vol->SetLineColor(kBlue); 
          vol->SetTransparency(20); // set the transparency level for the volumes
          
        // if (volname.Contains("InnerTrackerBarrel") || volname.Contains("VertexBarrel") || volname.Contains("MedialTrackerBarrel") || volname.Contains("OuterTrackerBarrel"))
        if (volname.Contains("gem"))
         {  
          Int_t nd = vol->GetNdaughters();
          for (Int_t i=0; i<nd; i++) {
          TString detname = vol->GetNode(i)->GetName();
         // if (!detname.Contains("layer")) continue;
          cout<<"==========================="<<endl;
          cout<<"Layer Name: "<<detname<<"Det Name"<<volname<<endl;
         // vol->InspectShape();
         // vol->InspectMaterial();

          }
        }
  }

	string output_plot_name=filename+"_MB.pdf";
   	TCanvas *cradlen = new TCanvas("c","c",0,52,1400,1000);
   	cradlen->SetGridy();
	   cradlen->SetMargin(0.08, 0.13 ,0.10,0.07);
 //TH2F *X0 = gGeoManager->GetTopVolume()->LegoPlot(180,0.,180.,180,0.,360.);
// TH2F *TGeoChecker::LegoPlot This function is defined in TGeoChecker
// https://root.cern.ch/doc/master/TGeoChecker_8cxx_source.html#l02040 line 20240
// gGeoManager->RandomRays(1000, 0., 0., 0.);
 gGeoManager->SetVisLevel(5);
 TH2F *X0 = gGeoManager->GetTopVolume()->LegoPlot(90,0.,180.,180,0.,360.);
//TH2F* LegoPlot(Int_t ntheta = 20, Double_t themin = 0., Double_t themax = 180., Int_t nphi = 60, Double_t phimin = 0., Double_t phimax = 360., Double_t rmin = 0., Double_t rmax = 9999999, Option_t* option = "") 	// *MENU*

  X0->SetTitle("Effective radiation length (X/X_{0}) vs #theta (deg) & #varphi (deg);#varphi (deg);#theta (deg);X/X_{0}");
  X0->GetXaxis()->CenterTitle();
  X0->GetYaxis()->CenterTitle();
  X0->GetZaxis()->CenterTitle();
  X0->GetYaxis()->SetTitleOffset(1.0);
  X0->GetZaxis()->SetTitleOffset(0.80);
  X0->GetZaxis()->SetRangeUser(0.,150.);
	 gStyle->SetPalette(kInvertedDarkBodyRadiator);
  X0->SetStats(kFALSE);
  X0->Draw("colz");
  cradlen->Update();
  TPaletteAxis *palette = (TPaletteAxis*)X0->GetListOfFunctions()->FindObject("palette");
  double y1 = palette->GetY1NDC();
  double y2 = palette->GetY2NDC();
  double x1 = palette->GetX1NDC();
  double x2 = palette->GetX2NDC();
  palette->SetX1NDC(x1+0.02);
  palette->SetX2NDC(x2+0.02);
  cradlen->Print(output_plot_name.data());
  output_plot_name = filename+"_MB.png";
  cradlen->Print(output_plot_name.data());
  new TCanvas;
  gGeoManager->GetTopNode()->Draw("ogl");
}
