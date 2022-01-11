void PlotFastSimOutput(float etamin, float etamax){


   		gStyle->SetTitleSize(0.055, "t");
        gStyle->SetTitleOffset(1.2, "XY");
        gStyle->SetLegendBorderSize(0);
        gStyle->SetPadTickX(1);
        gStyle->SetPadTickY(1);
        gStyle->SetOptStat(0.);	
        gStyle->SetOptFit(0);
        gStyle->SetOptTitle(0);


  float eta = etamin;
  const int nbins = 100;
  float binwidth = (etamax-etamin)/100;
  
  TFile *f[nbins];
  TGraph *grPt[nbins],*grP[nbins], *grPRXY[nbins], *grPRZ[nbins];

    // Reading the Graph
    for (int i=0; i<nbins; i++){

    	f[i] = TFile::Open(Form("Output/FastSimulation_Output_eta_%1.2f.root",eta));
      grP[i] =   (TGraph*) f[i]->Get("grTotalMomRes1");
    	grPt[i] =   (TGraph*) f[i]->Get("grMomRes1");
    	grPRXY[i] = (TGraph*) f[i]->Get("pointRRes0");
    	grPRZ[i] =  (TGraph*) f[i]->Get("pointZRes0");

        eta+=binwidth;
}

        const int NPt = grPt[0]->GetN(); // NPoints
        double x[NPt], AvgPtRes_Arr[NPt], AvgPRes_Arr[NPt], AvgPR_XY_Arr[NPt], AvgPR_Z_Arr[NPt];
        TGraph *grPt_Avg, *grP_Avg, *grPRXY_Avg, *grPRZ_Avg;


        for (int ipoint =0; ipoint<NPt; ipoint++){ // X -axis

          double Pt, PtRes=0., PR_XY=0., PR_Z=0., P = 0., PRes = 0.;  
          double AvgPtRes =0., AvgPR_XY=0., AvgPR_Z=0., AvgPRes = 0.;	
         
         for (int igraph=0; igraph<nbins; igraph++){
         
         grP[igraph]->GetPoint(ipoint,P,PRes); 
         grPt[igraph]->GetPoint(ipoint,Pt,PtRes); 
         grPRXY[igraph]->GetPoint(ipoint,Pt,PR_XY);  	
         grPRZ[igraph]->GetPoint(ipoint,Pt,PR_Z); 

         AvgPRes+=PRes;
         AvgPtRes+=PtRes;
         AvgPR_XY+=PR_XY;
         AvgPR_Z+=PR_Z;
        }

        x[ipoint] = Pt;
        AvgPtRes_Arr[ipoint] = AvgPtRes/nbins; 
        AvgPRes_Arr[ipoint] = AvgPRes/nbins;
        AvgPR_XY_Arr[ipoint] = AvgPR_XY/nbins; 
        AvgPR_Z_Arr[ipoint] = AvgPR_Z/nbins;
        }

        grP_Avg = new TGraph(NPt,x,AvgPRes_Arr);
        grP_Avg->SetName("P_Resol");
        grP_Avg->GetXaxis()->SetTitle("p GeV/c");
        grP_Avg->GetYaxis()->SetTitle("#sigma_{p}/p");

        grPt_Avg = new TGraph(NPt,x,AvgPtRes_Arr);
        grPt_Avg->SetName("Pt_Resol");
        grPt_Avg->GetXaxis()->SetTitle("p_{T} GeV/c");
        grPt_Avg->GetYaxis()->SetTitle("#sigma_{p_{T}}/p_{T}");

        grPRXY_Avg = new TGraph(NPt,x,AvgPR_XY_Arr);
        grPRXY_Avg->SetName("PR_ResolXY");
        grPRXY_Avg->GetXaxis()->SetTitle("p_{T} GeV/c");
        grPRXY_Avg->GetYaxis()->SetTitle("Transverse Pointing Resolution (#mum)");

        grPRZ_Avg = new TGraph(NPt,x,AvgPR_Z_Arr);
        grPRZ_Avg->SetName("PR_ResolZ");
        grPRZ_Avg->GetXaxis()->SetTitle("p_{T} GeV/c");
        grPRZ_Avg->GetYaxis()->SetTitle("Longitudinal Pointing Resolution (#mum)");

        TFile *fout = new TFile(Form("FastSimulation_Output_Avg_eta_%1.1f_%1.1f.root",etamin,etamax),"recreate");
        fout->cd();
        grP_Avg->Write();
        grPt_Avg->Write();
        grPRXY_Avg->Write();
        grPRZ_Avg->Write();
        fout->Close();

}
