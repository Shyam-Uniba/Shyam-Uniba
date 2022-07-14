 void Resolution_Detector1(){
	
      TCanvas *c3 = new TCanvas("c3", "c3",0,52,1400,1000);
	c3->SetGridy();
	c3->SetMargin(0.11, 0.07 ,0.10,0.07);

       TH1F *h = new TH1F("h","h",1000,-300.0e-6,300.0e-6);
       h->GetXaxis()->SetTitle("Mean X (meter)");
       h->GetYaxis()->SetTitle("Entries (a.u.)");
       h->GetXaxis()->CenterTitle();
       h->GetYaxis()->CenterTitle();
       
       Double_t shift = 0.;
       Double_t rphi_resol = 10.0e-4/sqrt(12);
       
       Double_t radius[] = {3.3,4.35,5.40,21.0+shift,22.68+shift,33.14,51.0,64.0,77.00};
       Double_t sigma_rphi_Si = 10.0e-4/sqrt(12);
       Double_t sigma_rphi_MM = 55.0e-4;
       Double_t sigma_rphi_TOF = 30.0e-4;
       unsigned int nlayers = sizeof(radius)/sizeof(radius[0]);
       
       for (int ilayer =0; ilayer<nlayers; ++ilayer){
       
       if (ilayer<5) rphi_resol = sigma_rphi_Si;
       else if (ilayer==7) rphi_resol = sigma_rphi_TOF;
       else rphi_resol = sigma_rphi_MM;
       cout<<" Layer No. "<<ilayer<<"\t Radius (cm): "<<radius[ilayer]<<"\t RPhi Resol (cm): "<<rphi_resol<<"\t Phi Resol: "<<rphi_resol/radius[ilayer]<<endl;
       }

  
       for (int i=0; i<10000000;i++)
       {
       double sum =0.;
       for (int ilayer =0; ilayer<nlayers; ++ilayer){
       
       if (ilayer<5) rphi_resol = sigma_rphi_Si;
       else if (ilayer==7) rphi_resol = sigma_rphi_TOF;
       else rphi_resol = sigma_rphi_MM;
       sum+=gRandom->Gaus(0.,rphi_resol/radius[ilayer]);       
       }
       double mean = sum/9.0;
       h->Fill(mean);
       }
       c3->cd();
       h->Fit("gaus");
       h->Draw();
       TF1 *f1 = h->GetFunction("gaus");
       Double_t sigma_simple_smear = f1->GetParameter(2); // sigma of Gaussian
       cout<<"Phi Smearing Sigma: "<<sigma_simple_smear<<endl; // 
    }
