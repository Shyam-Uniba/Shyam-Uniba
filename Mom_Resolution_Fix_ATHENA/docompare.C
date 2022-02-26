 void docompare()
    {
	
        gStyle->SetTitleSize(0.04,"");
        gStyle->SetTitleSize(0.04,"XY");	
        gStyle->SetTitleOffset(1.20,"Y");	
        gStyle->SetOptStat(1);
        gStyle->SetOptFit(1);

    TFile *f1_fastsim = TFile::Open("FastSimulation_Output_eta_0.00.root");
    TFile *f1_mysim = TFile::Open("Resolution_0.0.root");

    TFile *f2_fastsim = TFile::Open("FastSimulation_Output_eta_0.20.root");
    TFile *f2_mysim = TFile::Open("Resolution_0.2.root");

    TFile *f3_fastsim = TFile::Open("FastSimulation_Output_eta_0.30.root");
    TFile *f3_mysim = TFile::Open("Resolution_0.3.root");

    TFile *f4_fastsim = TFile::Open("FastSimulation_Output_eta_0.40.root");
    TFile *f4_mysim = TFile::Open("Resolution_0.4.root");

    TCanvas *c[4];
    for (int i =0; i<4; i++)
    {
   	c[i] = new TCanvas(Form("c%d",i),Form("c%d",i),0,52,1400,1000);
   	c[i]->SetGridy();
	   c[i]->SetMargin(0.11, 0.07 ,0.10,0.07);
    }

    TGraph *gr1_fastsim = (TGraph*)f1_fastsim->Get("grMomRes1");
    TGraph *gr1_mysim = (TGraph*)f1_mysim->Get("Sum_Pt_Resol");

    TGraph *gr2_fastsim = (TGraph*)f2_fastsim->Get("grMomRes1");
    TGraph *gr2_mysim = (TGraph*)f2_mysim->Get("Sum_Pt_Resol");

    TGraph *gr3_fastsim = (TGraph*)f3_fastsim->Get("grMomRes1");
    TGraph *gr3_mysim = (TGraph*)f3_mysim->Get("Sum_Pt_Resol");

    TGraph *gr4_fastsim = (TGraph*)f4_fastsim->Get("grMomRes1");
    TGraph *gr4_mysim = (TGraph*)f4_mysim->Get("Sum_Pt_Resol");

    c[0]->cd();
    gr1_fastsim->GetYaxis()->SetRangeUser(0.,2.0);
    gr1_fastsim->Draw();
    gr1_mysim->Draw("same");
    //--------------Legend Draw----------------------         
    TLegend *leg_hist = new TLegend(0.40,0.75,0.60,0.90);
    leg_hist->SetHeader("#eta = 0.0");
    leg_hist->SetTextFont(42);
    leg_hist->SetBorderSize(0);
    leg_hist->SetTextSize(0.04);
   
    leg_hist->AddEntry(gr1_fastsim,"Fast Simulation","l"); 
    leg_hist->AddEntry(gr1_mysim,"Analytical","l");
    leg_hist->Draw();

    c[1]->cd();
    gr2_fastsim->GetYaxis()->SetRangeUser(0.,2.0);
    gr2_fastsim->Draw();
    gr2_mysim->Draw("same");
    leg_hist = new TLegend(0.40,0.75,0.60,0.90);
    leg_hist->SetHeader("#eta = 0.2");
    leg_hist->SetTextFont(42);
    leg_hist->SetBorderSize(0);
    leg_hist->SetTextSize(0.04);
   
    leg_hist->AddEntry(gr2_fastsim,"Fast Simulation","l"); 
    leg_hist->AddEntry(gr2_mysim,"Analytical","l");
    leg_hist->Draw();

    c[2]->cd();
    gr3_fastsim->GetYaxis()->SetRangeUser(0.,2.0);
    gr3_fastsim->Draw();
    gr3_mysim->Draw("same");

    leg_hist = new TLegend(0.40,0.75,0.60,0.90);
    leg_hist->SetHeader("#eta = 0.3");
    leg_hist->SetTextFont(42);
    leg_hist->SetBorderSize(0);
    leg_hist->SetTextSize(0.04);
   
    leg_hist->AddEntry(gr3_fastsim,"Fast Simulation","l"); 
    leg_hist->AddEntry(gr3_mysim,"Analytical","l");
    leg_hist->Draw();

    c[3]->cd();
    gr4_fastsim->GetYaxis()->SetRangeUser(0.,2.0);
    gr4_fastsim->Draw();
    gr4_mysim->Draw("same");
    leg_hist = new TLegend(0.40,0.75,0.60,0.90);
    leg_hist->SetHeader("#eta = 0.4");
    leg_hist->SetTextFont(42);
    leg_hist->SetBorderSize(0);
    leg_hist->SetTextSize(0.04);
   
    leg_hist->AddEntry(gr4_fastsim,"Fast Simulation","l"); 
    leg_hist->AddEntry(gr4_mysim,"Analytical","l");
    leg_hist->Draw();

   c[0]->SaveAs("resolution_eta_0.0.png");
   c[1]->SaveAs("resolution_eta_0.2.png");
   c[2]->SaveAs("resolution_eta_0.3.png");
   c[3]->SaveAs("resolution_eta_0.4.png");
    
   }


