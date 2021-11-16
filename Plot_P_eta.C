void Plot_P_eta()
{
	
////////////////////////////////////////////////////// style of the plot
   gStyle->SetPalette(1);
   gStyle->SetOptTitle(0);
   gStyle->SetTitleOffset(.85,"X");gStyle->SetTitleOffset(.85,"Y");
   gStyle->SetTitleSize(.05,"X");gStyle->SetTitleSize(.05,"Y");
   gStyle->SetLabelSize(.04,"X");gStyle->SetLabelSize(.04,"Y");
   gStyle->SetHistLineWidth(2);
   gStyle->SetOptFit(0);
   gStyle->SetOptStat(1);

  
   TCanvas * c1 = new TCanvas("c1","coutput",1400,1000);
   c1->SetMargin(0.10, 0.05 ,0.1,0.05);
	  c1->SetGridy();

   const int n=15;
   float eta_arr[n], pt_min[n], pt_max[n], pt_highest[n];
   int index =0;

   for (int i= -35; i<=35; i=i+5)
   {
   eta_arr[index] = i*0.1;
   pt_min[index] = 1.0/(TMath::CosH(i*0.1)); // p = pt*Cosh(eta);
   pt_max[index] = 5.0/(TMath::CosH(i*0.1));
   pt_highest[index] = 30.0/(TMath::CosH(i*0.1));
   index++;
   }
   
   TMultiGraph *mg = new TMultiGraph();
   TGraph *gr = new TGraph(n,eta_arr,pt_min);
   gr->SetMarkerColor(kBlue);
   gr->SetMarkerStyle(20);
   gr->SetMarkerSize(1.5);

   TGraph *gr1 = new TGraph(n,eta_arr,pt_max);
   gr1->SetMarkerColor(kRed);
   gr1->SetMarkerStyle(45);
   gr1->SetMarkerSize(1.8);

   TGraph *gr2 = new TGraph(n,eta_arr,pt_highest);
   gr2->SetMarkerColor(kMagenta);
   gr2->SetMarkerStyle(47);
   gr2->SetMarkerSize(1.8);

			mg->Add(gr);
			mg->Add(gr1);
			mg->Add(gr2);

			c1->cd();
			mg->Draw("ap");
   mg->GetXaxis()->SetTitle("#eta");
   mg->GetYaxis()->SetTitle("p_{T} (GeV/c)");
   mg->GetYaxis()->SetRangeUser(0.,50.);
   mg->GetXaxis()->CenterTitle();
   mg->GetYaxis()->CenterTitle();
   gPad->Modified();
   gPad->Update();

  	TLegend *l1= new TLegend(0.40,0.80,0.70,0.93);
			l1->SetTextSize(0.035);
			l1->SetBorderSize(0);
			l1->AddEntry(gr,"p = 1 GeV/c");
			l1->AddEntry(gr1,"p = 15 GeV/c");
			l1->AddEntry(gr2,"p = 30 GeV/c");
			l1->Draw();
   
   c1->SaveAs("pt_eta_corr.png");


}
