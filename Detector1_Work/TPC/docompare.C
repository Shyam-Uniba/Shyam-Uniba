 void docompare(float etamin, float etamax, TString graphname ="")
    {
	
        gStyle->SetTitleSize(0.04,"");
        gStyle->SetTitleSize(0.04,"XY");	
        gStyle->SetTitleOffset(1.20,"Y");	
        gStyle->SetOptStat(1);
        gStyle->SetOptFit(1);

    
    TFile *f[3]; // default, mb = 0.55%, 0.24%
    f[0]= TFile::Open(Form("./woTPC/FastSimulation_Output_Avg_eta_%1.1f_%1.1f.root",etamin,etamax));
    f[1]= TFile::Open(Form("./wTPC_resol_41mum/FastSimulation_Output_Avg_eta_%1.1f_%1.1f.root",etamin,etamax));
    f[2]= TFile::Open(Form("./wTPC_resol_95mum/FastSimulation_Output_Avg_eta_%1.1f_%1.1f.root",etamin,etamax));


   	TCanvas *c = new TCanvas("c","c",0,52,1400,1000);
   	c->SetGridy();
	c->SetMargin(0.11, 0.07 ,0.10,0.07);

    TGraph *gr[3];
     
    gr[0] = (TGraph*) f[0]->Get(graphname.Data());
    gr[1] = (TGraph*) f[1]->Get(graphname.Data());
    gr[2] = (TGraph*) f[2]->Get(graphname.Data());




    Int_t colors[6] = {1,2,3,4,38,7};

    TMultiGraph *mg;
    mg = new TMultiGraph();
    if (graphname=="P_Resol") mg->SetTitle(";p (GeV/c); #sigmap/p");
    else if (graphname=="Pt_Resol") mg->SetTitle(";p_{T} (GeV/c); #sigmap_{T}/p_{T}");
    else if (graphname=="PR_ResolXY") mg->SetTitle(";p_{T} (GeV/c); Transverse Pointing Resolution (#mum)");
    else if (graphname=="PR_ResolZ") mg->SetTitle(";p_{T} (GeV/c); Longitudinal Pointing Resolution (#mum)");
    else if (graphname=="PR_ResolXY_Mom") mg->SetTitle(";p (GeV/c); Transverse Pointing Resolution (#mum)");
    else cout<<"Graph Name not correct !!!!!!"<<endl;

    for (int i=0; i<3; i++)
    {
    	gr[i]->SetLineColor(colors[i]);
     gr[i]->SetLineWidth(2.0);
    	mg->Add(gr[i]);
    }
    
    c->cd();
    mg->Draw("AC");
    mg->GetXaxis()->CenterTitle();
    mg->GetYaxis()->CenterTitle();
    mg->GetYaxis()->SetRangeUser(0.,3.);
    mg->GetXaxis()->SetRangeUser(0.,20.);
    if (graphname=="PR_ResolXY") {c->SetLogy(); mg->GetYaxis()->SetRangeUser(1.,500.);}
    else if (graphname=="PR_ResolZ")  {c->SetLogy(); mg->GetYaxis()->SetRangeUser(1.,500.);}
    else if (graphname=="PR_ResolXY_Mom")  {c->SetLogy(); mg->GetYaxis()->SetRangeUser(1.,500.);};
    gPad->Modified();
    gPad->Update();

   // Draw Requirement #eta (-1.0,1.0)
	   TF1 *PWGReq_p, *PWGReq_DCA2D;
			
   if (graphname=="P_Resol")
   {
    PWGReq_p = new TF1("PWGReq_pT", "TMath::Sqrt((0.05*x)^2+0.5^2)",0.,45.);
    PWGReq_p->SetLineColor(kMagenta);
    PWGReq_p->SetLineStyle(3);
    PWGReq_p->SetLineWidth(2.0);
    PWGReq_p->Draw("same");
    TLegend *l= new TLegend(0.70,0.80,0.90,0.90);
    l->SetTextSize(0.035);
    l->SetBorderSize(0);
    l->AddEntry(PWGReq_p,"PWGReq");
    l->Draw("same");
    }

    if (graphname=="PR_ResolXY")
    {
    PWGReq_DCA2D = new TF1("PWGReq_DCA2D", "TMath::Sqrt((20.0/x)^2+5.0^2)",0.,45.);
    PWGReq_DCA2D->SetLineColor(kMagenta);
    PWGReq_DCA2D->SetLineStyle(3);
    PWGReq_DCA2D->SetLineWidth(2.0);
    PWGReq_DCA2D->Draw("same");
    TLegend *l= new TLegend(0.70,0.80,0.90,0.90);
    l->SetTextSize(0.035);
    l->SetBorderSize(0);
    l->AddEntry(PWGReq_DCA2D,"PWGReq");
    l->Draw("same");
    }
   
    TLegend *leg_hist = new TLegend(0.20,0.75,0.40,0.90);
    leg_hist->SetHeader(Form("%1.1f< #eta <%1.1f",etamin,etamax));
    leg_hist->SetTextFont(42);
    leg_hist->SetBorderSize(0);
    leg_hist->SetTextSize(0.03); 
    leg_hist->AddEntry(gr[0],"Without TPC","l"); 
    leg_hist->AddEntry(gr[1],"TPC Resol: #sigma_{r#varphi} = 41 #mum, #sigma_{z} = 145 #mum,","l"); 
    leg_hist->AddEntry(gr[2],"TPC Resol: #sigma_{r#varphi} = 95 #mum, #sigma_{z} = 145 #mum,","l"); 
    leg_hist->Draw();
    c->SaveAs(Form("Comparison%s_eta%1.1f_%1.1f.png",graphname.Data(),etamin,etamax));
    
   }
