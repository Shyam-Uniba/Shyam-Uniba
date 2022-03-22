 void Random_Gaus();
 void Resolution_Gaus()
    {
	
        gStyle->SetTitleSize(0.04,"");
        gStyle->SetTitleSize(0.04,"XY");	
        gStyle->SetTitleOffset(1.20,"Y");	
        gStyle->SetOptStat(1);
        gStyle->SetOptFit(1);

	TCanvas *c1 = new TCanvas("c1", "c1",0,52,1400,1000);
	c1->SetGridy();
	c1->SetMargin(0.11, 0.07 ,0.10,0.07);

	TCanvas *c2 = new TCanvas("c2", "c2",0,52,1400,1000);
	c2->SetGridy();
	c2->SetMargin(0.11, 0.07 ,0.10,0.07);

       TH1F *h_Angle = new TH1F("h_Angle","h_Angle",1000,-30.0e-5,30.0e-5);
       h_Angle->GetXaxis()->SetTitle("Mean X (meter)");
       h_Angle->GetYaxis()->SetTitle("Entries (a.u.)");
       h_Angle->GetXaxis()->CenterTitle();
       h_Angle->GetYaxis()->CenterTitle();

       TH1F *h_Resol = new TH1F("h_Resol","h_Resol",1000,-300.0e-6,300.0e-6);
       h_Resol->GetXaxis()->SetTitle("Combined Resolution (meter)");
       h_Resol->GetYaxis()->SetTitle("Entries (a.u.)");
       h_Resol->GetXaxis()->CenterTitle();
       h_Resol->GetYaxis()->CenterTitle();

       Double_t vtx[3] ={3.3,4.35,5.4};
       Double_t Barr[2] = {13.34, 17.96};
       Double_t MM[4] = {47.72, 49.57, 75.61, 77.47};
       Double_t length = (MM[3]-vtx[0])*0.01; // Length of tracker

       Double_t Sigma_Si = 10.0e-6/sqrt(12); // Resolution in meter
       Double_t Sigma_MM = 150.0e-6; // Resolution in meter

       Double_t Average_Resolution = (5.0*Sigma_Si+4.0*Sigma_MM)/9.0; // pixel resolution in meter micormegas

       Double_t Phi_Resol[9], sigma[9];
       
       Phi_Resol[0] = Sigma_Si/(vtx[0]*0.01); Phi_Resol[1] = Sigma_Si/(vtx[1]*0.01); Phi_Resol[2] = Sigma_Si/(vtx[2]*0.01); ;
       Phi_Resol[3] = Sigma_Si/(Barr[0]*0.01); Phi_Resol[4] = Sigma_Si/(Barr[1]*0.01); Phi_Resol[5] = Sigma_MM/(MM[0]*0.01);
       Phi_Resol[6] = Sigma_MM/(MM[1]*0.01);  Phi_Resol[7] = Sigma_MM/(MM[2]*0.01); Phi_Resol[8] = Sigma_MM/(MM[3]*0.01);

       for (int i=0; i<8; i++) cout<<"Layer: "<< i+1<<" Phi_Resol: "<<Phi_Resol[i]<<endl;

       for (int i=0; i<10000000;i++)
       {       	              	
       Double_t rand1 = gRandom->Gaus(0,Phi_Resol[0]);
       Double_t rand2 = gRandom->Gaus(0,Phi_Resol[1]);
       Double_t rand3 = gRandom->Gaus(0,Phi_Resol[2]);
       Double_t rand4 = gRandom->Gaus(0,Phi_Resol[3]);
       Double_t rand5 = gRandom->Gaus(0,Phi_Resol[4]);
       Double_t rand6 = gRandom->Gaus(0,Phi_Resol[5]);
       Double_t rand7 = gRandom->Gaus(0,Phi_Resol[6]);
       Double_t rand8 = gRandom->Gaus(0,Phi_Resol[7]);
       Double_t rand9 = gRandom->Gaus(0,Phi_Resol[8]);
       Double_t mean_angle = (rand1+rand2+rand3+rand4+rand5+rand6+rand7+rand8+rand9)/9.0;
       h_Angle->Fill(mean_angle);
       }
       c1->cd();
       h_Angle->Fit("gaus");
       h_Angle->Draw();
         
       TF1 *f = h_Angle->GetFunction("gaus");
       Double_t sigma_Phi_Resol = f->GetParameter(2); // sigma of Gaussian
  
       sigma[0] = sigma_Phi_Resol*vtx[0]*0.01;   sigma[1] = sigma_Phi_Resol*vtx[1]*0.01; 
       sigma[2] = sigma_Phi_Resol*vtx[2]*0.01;   sigma[3] = sigma_Phi_Resol*Barr[0]*0.01;  
       sigma[4] = sigma_Phi_Resol*Barr[1]*0.01;  sigma[5] = sigma_Phi_Resol*MM[0]*0.01;    
       sigma[6] = sigma_Phi_Resol*MM[1]*0.01;    sigma[7] = sigma_Phi_Resol*MM[2]*0.01;    sigma[8] = sigma_Phi_Resol*MM[3]*0.01; 

       for (int i=0; i<9; i++) cout<<"Layer: "<< i<<" Effective Resolution: "<<sigma[i]<<endl;


       for (int i=0; i<10000000;i++)
       {
       Double_t rand1 = gRandom->Gaus(0,sigma[0]);
       Double_t rand2 = gRandom->Gaus(0,sigma[1]);
       Double_t rand3 = gRandom->Gaus(0,sigma[2]);
       Double_t rand4 = gRandom->Gaus(0,sigma[3]);
       Double_t rand5 = gRandom->Gaus(0,sigma[4]);
       Double_t rand6 = gRandom->Gaus(0,sigma[5]);
       Double_t rand7 = gRandom->Gaus(0,sigma[6]);
       Double_t rand8 = gRandom->Gaus(0,sigma[7]);
       Double_t rand9 = gRandom->Gaus(0,sigma[8]);
       Double_t mean = (rand1+rand2+rand3+rand4+rand5+rand6+rand7+rand8+rand9)/9.0;
       h_Resol->Fill(mean);
       }
       c2->cd();
       h_Resol->Fit("gaus");
       h_Resol->Draw();

       TF1 *f1 = h_Resol->GetFunction("gaus");
       Double_t sigma_calculated = f1->GetParameter(2); // sigma of Gaussian
       cout<<"\n"<<endl;
       cout<<"Weight Average Resolution (meter): "<<Average_Resolution<<"  Calculation Resolution (meter): "<<sigma_calculated<<endl;
       cout<<"\n"<<endl;
       Random_Gaus(); // Example
}

  void Random_Gaus(){
	
      TCanvas *c3 = new TCanvas("c3", "c3",0,52,1400,1000);
	c3->SetGridy();
	c3->SetMargin(0.11, 0.07 ,0.10,0.07);

       TH1F *h = new TH1F("h","h",1000,-300.0e-6,300.0e-6);
       h->GetXaxis()->SetTitle("Mean X (meter)");
       h->GetYaxis()->SetTitle("Entries (a.u.)");
       h->GetXaxis()->CenterTitle();
       h->GetYaxis()->CenterTitle();

       for (int i=0; i<10000000;i++)
       {
       Double_t rand1 = gRandom->Gaus(0,10.0e-6/sqrt(12));
       Double_t rand2 = gRandom->Gaus(0,10.0e-6/sqrt(12));
       Double_t rand3 = gRandom->Gaus(0,10.0e-6/sqrt(12));
       Double_t rand4 = gRandom->Gaus(0,10.0e-6/sqrt(12));
       Double_t rand5 = gRandom->Gaus(0,10.0e-6/sqrt(12));
       Double_t rand6 = gRandom->Gaus(0,150.0e-6);
       Double_t rand7 = gRandom->Gaus(0,150.0e-6);
       Double_t rand8 = gRandom->Gaus(0,150.0e-6);
       Double_t rand9 = gRandom->Gaus(0,150.0e-6);
       Double_t mean = (rand1+rand2+rand3+rand4+rand5+rand6+rand7+rand8+rand9)/9.0;
       h->Fill(mean);
       }
       c3->cd();
       h->Fit("gaus");
       h->Draw();
       TF1 *f1 = h->GetFunction("gaus");
       Double_t sigma_simple_smear = f1->GetParameter(2); // sigma of Gaussian
       cout<<"Simple Smearing Resolution (meter): "<<sigma_simple_smear<<endl; // This method underestimating the resolution
    }
