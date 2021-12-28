void DrawMaterialBudget_CEE(){
	TFile *f = TFile::Open("tpc_2box.root");
 new TGeoManager("Geometry", "default geometry");
 TGeoVolume *TOPTPC = (TGeoVolume*)f->Get("TOPTPC"); 
 gGeoManager->SetTopVolume(TOPTPC);
	string output_plot_name="CeeTPC_MB.pdf";
	TCanvas *cradlen = new TCanvas("radlen", "radiation length");
	cradlen->SetRightMargin(0.18);
 //TH2F *X0 = gGeoManager->GetTopVolume()->LegoPlot(180,0.,180.,180,0.,360.);
 TH2F *X0 = gGeoManager->GetTopVolume()->LegoPlot(180,0.,180.,180,0.,360.);
//TH2F* LegoPlot(Int_t ntheta = 20, Double_t themin = 0., Double_t themax = 180., Int_t nphi = 60, Double_t phimin = 0., Double_t phimax = 360., Double_t rmin = 0., Double_t rmax = 9999999, Option_t* option = "") 	// *MENU*

  X0->SetXTitle( "#varphi (deg)");
  X0->SetYTitle( "#theta (deg)");
  X0->SetZTitle( "X/X0");
  X0->GetZaxis()->SetTitleOffset(1.4);
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
  output_plot_name = "CeeTPC_MB.png";
  cradlen->Print(output_plot_name.data());
  new TCanvas;
  gGeoManager->GetTopNode()->Draw("ogl");
}
