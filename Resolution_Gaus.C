 void Resolution_Gaus()
    {
	
       TCanvas *c = new TCanvas("c", "c",0,52,1400,1000);
	     c->SetGridy();
	     c->SetMargin(0.11, 0.07 ,0.10,0.07);

       TH1F *h = new TH1F("h","h",1000,-300.0e-6,300.0e-6);
       h->GetXaxis()->SetTitle("Mean X (meter)");
       h->GetYaxis()->SetTitle("Entries (a.u.)");
       h->GetXaxis()->CenterTitle();
       h->GetYaxis()->CenterTitle();
       
       Double_t radius[] = {3.6,4.8,12.0,27.0,42.0,55.0,64.6};
       unsigned int nlayers = sizeof(radius)/sizeof(radius[0]);
       
      Double_t SiLayerResolution = 10.0e-4/sqrt(12);
      Double_t MMLayerResolution = 150.0e-4;
      Double_t LGADResolution = 30.0e-4;
      Double_t rphi_resolution[] = {SiLayerResolution,SiLayerResolution,SiLayerResolution,SiLayerResolution,SiLayerResolution,MMLayerResolution,LGADResolution};
       

       for (int i=0; i<10000000;i++)
       {
        Double_t rand=0;
       
       for (int ilayer = 0; ilayer<nlayers; ++ilayer) rand+= gRandom->Gaus(0,rphi_resolution[ilayer]/radius[ilayer]);      
       Double_t mean = (rand)/nlayers;
       h->Fill(mean);
       }
       c->cd();
       h->Fit("gaus");
       h->Draw();
       TF1 *f1 = h->GetFunction("gaus");
       Double_t sigma_simple_smear = f1->GetParameter(2); // sigma of Gaussian
       cout<<"Simple Smearing Resolution (meter): "<<sigma_simple_smear<<endl; // This method underestimating the resolution
    }
