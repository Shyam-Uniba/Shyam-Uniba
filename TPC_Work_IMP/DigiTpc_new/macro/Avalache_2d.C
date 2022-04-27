void Avalache_2d()
{
	
////////////////////////////////////////////////////// style of the plot
   gStyle->SetPalette(1);
   gStyle->SetOptTitle(0);
   gStyle->SetTitleOffset(.85,"X");gStyle->SetTitleOffset(.85,"Y");
   gStyle->SetTitleSize(.04,"X");gStyle->SetTitleSize(.04,"Y");
   gStyle->SetLabelSize(.04,"X");gStyle->SetLabelSize(.04,"Y");
   gStyle->SetHistLineWidth(2);
   gStyle->SetOptFit(0);
   gStyle->SetOptStat(0);
   gStyle->SetGridStyle(1);
   gStyle->SetGridWidth(2);

  TCanvas * c1 = new TCanvas("c1","coutput",1000,1000);
  c1->SetMargin(0.09, 0.03 ,0.1,0.05);
  c1->SetGrid();
  	
  auto f2 = new TF2("bigaus","bigaus",-3,3,-3,3);
  f2->SetParameters(1,0,1,0,1);
  auto h2 = new TH2D("h2", "", 100, -3, 3, 100, -3, 3);
  h2->FillRandom("bigaus", 100000);
  h2->GetXaxis()->SetTitle("Avalanche X [cm]");
  h2->GetYaxis()->SetTitle("Avalanche Z [cm]");
  h2->GetYaxis()->CenterTitle();
  h2->GetXaxis()->CenterTitle();
  c1->cd();
  h2->Draw("");
  h2->Fit("bigaus");	
  c1->SaveAs("avalanche_2d.pdf");
}
