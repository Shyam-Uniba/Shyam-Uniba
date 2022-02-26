void Resolution_2d()
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
  c1->SetMargin(0.09, 0.1 ,0.1,0.05);
  c1->SetGrid();
  
  Double_t resolution_Si = 10.0e-6/sqrt(12); // Silicon resolution  	
  Double_t resolution_MM = 150.0e-6; // Silicon resolution  	

  auto h2 = new TH2D("h2", "", 1000,-300e-6, 300e-6, 1000,-300e-6, 300e-6);

  Double_t x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8, x9, y9;

  auto f1 = new TF2("f1","bigaus",-5.0*resolution_Si,5.0*resolution_Si,-5.0*resolution_Si,5.0*resolution_Si);
  f1->SetParameters(1,0,resolution_Si,0,resolution_Si);

  auto f2 = new TF2("f2","bigaus",-5.0*resolution_MM,5.0*resolution_MM,-5.0*resolution_MM,5.0*resolution_MM);
  f2->SetParameters(1,0,resolution_MM,0,resolution_MM);

  for (int i=0; i<10000000; i++){
  f1->GetRandom2(x1,y1);
  f1->GetRandom2(x2,y2);
  f1->GetRandom2(x3,y3);
  f1->GetRandom2(x4,y4);
  f1->GetRandom2(x5,y5);
  f2->GetRandom2(x6,y6);
  f2->GetRandom2(x7,y7);
  f2->GetRandom2(x8,y8);
  f2->GetRandom2(x9,y9);

  Double_t Xavg = (x1+x2+x3+x4+x5+x6+x7+x8+x9)/9.0;
  Double_t Yavg = (y1+y2+y3+y4+y5+y6+y7+y8+y9)/9.0;
  h2->Fill(Xavg,Yavg);
//  cout<<Xavg<<"   "<<Yavg<<endl;
  }

  h2->GetXaxis()->SetTitle("#sigma_{x} [m]");
  h2->GetYaxis()->SetTitle("#sigma_{y} [m]");
  h2->GetYaxis()->CenterTitle();
  h2->GetXaxis()->CenterTitle();
  c1->cd();
  h2->Draw("");
  h2->Fit(f2,"R");	
  c1->SaveAs("avalanche_2d.pdf");
}
