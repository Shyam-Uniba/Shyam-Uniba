void DrawMaterialBudget_1d(const char* name="detector_geometry.root"){
	gStyle->SetTitleOffset(1.10,"XY");
	TGeoManager::Import(name);
	TCanvas *cradlen = new TCanvas("radlen", "radiation length",1200,1000);
	cradlen->SetRightMargin(0.18);

	TCanvas *cradlen_1d = new TCanvas("cradlen_1d", "radiation length",1200,1000);
	cradlen_1d->SetMargin(0.1,0.05,0.1,0.05);
 //TH2F *X0 = gGeoManager->GetTopVolume()->LegoPlot(180,0.,180.,180,0.,360.);
// TH2F *TGeoChecker::LegoPlot This function is defined in TGeoChecker
// https://root.cern.ch/doc/master/TGeoChecker_8cxx_source.html#l02040 line 20240
// gGeoManager->RandomRays(1000, 0., 0., 0.);
  TH2F *X0 = gGeoManager->GetVolume(0)->LegoPlot(180,0.,180.,180,0.,360.);
  X0->SetXTitle( "#varphi (deg)");
  X0->SetYTitle( "#theta (deg)");
  X0->SetZTitle( "X/X0");
  X0->GetZaxis()->SetTitleOffset(1.4);
	 gStyle->SetPalette(kInvertedDarkBodyRadiator);
  X0->SetStats(kFALSE);
  cradlen->cd();
  X0->Draw("colz");
  cradlen->Update();
  new TCanvas;
  gGeoManager->GetVolume(0)->Draw("ogl");

  cradlen_1d->cd();
  cradlen_1d->SetGridy();
  cradlen_1d->SetGridx();
  TH1F *hMb_1d = (TH1F*) X0->ProjectionY("Mb_1d_py",1,-1,"");
  hMb_1d->SetStats(kFALSE);
  hMb_1d->SetLineWidth(2);
  hMb_1d->GetYaxis()->SetTitle("X/X_{0}");
  hMb_1d->GetXaxis()->CenterTitle();
  hMb_1d->GetYaxis()->CenterTitle();
  hMb_1d->Scale(1./180);
  hMb_1d->Draw("hist");

}
