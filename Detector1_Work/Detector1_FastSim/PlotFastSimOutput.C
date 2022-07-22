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
  float binwidth = 0.01; // binwidth keep fixed
  const int nbins = (etamax-etamin)/binwidth;
  
  TFile *f[nbins];
  TGraph *grPt[nbins],*grP[nbins], *grPRXY[nbins], *grPRZ[nbins];
  TGraph *grPRXY_Mom[nbins], *grPRZ_Mom[nbins];

    // Reading the Graph
    for (int i=0; i<nbins; i++){
     if (fabs(eta) >= -0.0001 && fabs(eta) <= 0.0001) eta = 0.00; // avoid negative -0.00
    	f[i] = TFile::Open(Form("Output/FastSimulation_Output_eta_%1.2f.root",eta));
      grP[i] =   (TGraph*) f[i]->Get("grTotalMomRes1");
    	grPt[i] =   (TGraph*) f[i]->Get("grMomRes1");
    	grPRXY[i] = (TGraph*) f[i]->Get("pointRRes0");
    	grPRZ[i] =  (TGraph*) f[i]->Get("pointZRes0");
      grPRXY_Mom[i] = (TGraph*) f[i]->Get("pointRRes_Mom0");
      grPRZ_Mom[i] =  (TGraph*) f[i]->Get("pointZRes_Mom0");

        eta+=binwidth;
     }

        const int NPt = grPt[0]->GetN(); // NPoints
        double xpt[NPt], xp[NPt], AvgPtRes_Arr[NPt], AvgPRes_Arr[NPt], AvgPR_XY_Pt_Arr[NPt], AvgPR_Z_Pt_Arr[NPt];
        double AvgPR_XY_Mom_Arr[NPt], AvgPR_Z_Mom_Arr[NPt];
        TGraph *grPt_Avg, *grP_Avg, *grPRXY_Pt_Avg, *grPRZ_Pt_Avg;
        TGraph *grPRXY_Mom_Avg, *grPRZ_Mom_Avg;


        for (int ipoint =0; ipoint<NPt; ipoint++){ // X -axis

          double Pt, PtRes=0., PR_XY=0., PR_Z=0., P = 0., PRes = 0.;  
          double AvgPtRes =0., AvgPR_XY=0., AvgPR_Z=0., AvgPRes = 0.;	
          double PR_XY_Mom=0., PR_Z_Mom=0., AvgPR_XY_Mom = 0., AvgPR_Z_Mom=0.;
         
         for (int igraph=0; igraph<nbins; igraph++){
         
         grP[igraph]->GetPoint(ipoint,P,PRes); 
         grPt[igraph]->GetPoint(ipoint,Pt,PtRes); 
         grPRXY[igraph]->GetPoint(ipoint,Pt,PR_XY);  	
         grPRZ[igraph]->GetPoint(ipoint,Pt,PR_Z); 
         grPRXY_Mom[igraph]->GetPoint(ipoint,P,PR_XY_Mom);    
         grPRZ_Mom[igraph]->GetPoint(ipoint,P,PR_Z_Mom); 

         AvgPRes+=PRes;
         AvgPtRes+=PtRes;
         AvgPR_XY+=PR_XY;
         AvgPR_Z+=PR_Z;
         AvgPR_XY_Mom+=PR_XY_Mom;
         AvgPR_Z_Mom+=PR_Z_Mom;
        }

        xp[ipoint] = P;
        xpt[ipoint] = Pt;
        AvgPtRes_Arr[ipoint] = AvgPtRes/nbins; 
        AvgPRes_Arr[ipoint] = AvgPRes/nbins;
        AvgPR_XY_Pt_Arr[ipoint] = AvgPR_XY/nbins; 
        AvgPR_Z_Pt_Arr[ipoint] = AvgPR_Z/nbins;
        AvgPR_XY_Mom_Arr[ipoint] = AvgPR_XY_Mom/nbins; 
        AvgPR_Z_Mom_Arr[ipoint] = AvgPR_Z_Mom/nbins;
        }

        grP_Avg = new TGraph(NPt,xp,AvgPRes_Arr);
        grP_Avg->SetName("P_Resol");
        grP_Avg->GetXaxis()->SetTitle("p GeV/c");
        grP_Avg->GetYaxis()->SetTitle("#sigma_{p}/p");

        grPt_Avg = new TGraph(NPt,xpt,AvgPtRes_Arr);
        grPt_Avg->SetName("Pt_Resol");
        grPt_Avg->GetXaxis()->SetTitle("p_{T} GeV/c");
        grPt_Avg->GetYaxis()->SetTitle("#sigma_{p_{T}}/p_{T}");

        grPRXY_Pt_Avg = new TGraph(NPt,xpt,AvgPR_XY_Pt_Arr);
        grPRXY_Pt_Avg->SetName("PR_ResolXY");
        grPRXY_Pt_Avg->GetXaxis()->SetTitle("p_{T} GeV/c");
        grPRXY_Pt_Avg->GetYaxis()->SetTitle("Transverse Pointing Resolution (#mum)");

        grPRZ_Pt_Avg = new TGraph(NPt,xpt,AvgPR_Z_Pt_Arr);
        grPRZ_Pt_Avg->SetName("PR_ResolZ");
        grPRZ_Pt_Avg->GetXaxis()->SetTitle("p_{T} GeV/c");
        grPRZ_Pt_Avg->GetYaxis()->SetTitle("Longitudinal Pointing Resolution (#mum)");

        grPRXY_Mom_Avg = new TGraph(NPt,xp,AvgPR_XY_Mom_Arr);
        grPRXY_Mom_Avg->SetName("PR_ResolXY_Mom");
        grPRXY_Mom_Avg->GetXaxis()->SetTitle("p GeV/c");
        grPRXY_Mom_Avg->GetYaxis()->SetTitle("Transverse Pointing Resolution (#mum)");

        grPRZ_Mom_Avg = new TGraph(NPt,xp,AvgPR_Z_Mom_Arr);
        grPRZ_Mom_Avg->SetName("PR_ResolZ_Mom");
        grPRZ_Mom_Avg->GetXaxis()->SetTitle("p GeV/c");
        grPRZ_Mom_Avg->GetYaxis()->SetTitle("Longitudinal Pointing Resolution (#mum)");

        TFile *fout = new TFile(Form("FastSimulation_Output_Avg_eta_%1.1f_%1.1f.root",etamin,etamax),"recreate");
        fout->cd();
        grP_Avg->Write();
        grPt_Avg->Write();
        grPRXY_Pt_Avg->Write();
        grPRZ_Pt_Avg->Write();
        grPRXY_Mom_Avg->Write();
        grPRZ_Mom_Avg->Write();
        fout->Close();
}
