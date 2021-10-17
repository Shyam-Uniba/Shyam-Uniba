//  Momentum resolution for charged particles
// Shyam Kumar
// shyam.kumar@cern.ch

#include<TMath.h>
#include <TString.h>


  void MultipleScattering(Double_t &mp, Double_t &momI, Double_t &theta);
        
  
  // masses in MeV
	 Double_t mel = 0.51099891; // Mass of electron and positron
	       
	 Double_t mmu = 105.6583715;   // Mass of muons
	        
	 Double_t mpi = 139.57018;   // mass of charged pions
	        
	 Double_t mk = 493.667;   // mass of charged kaons
	        
	 Double_t mp = 938.272046;  // mass of protons and antiprotons
	 
	 Double_t speed= 3.0e8; //speed of light
	 
	 Int_t charge=1;
	  
	 Double_t effradlen=0.0128; // 4 layer of Si 300 mum

	
void ms_tracker()
{
	
        gStyle->SetTitleSize(0.04,"");
        gStyle->SetTitleSize(0.04,"X");	
        gStyle->SetTitleSize(0.05,"Y");	
        gStyle->SetTitleOffset(1.05,"Y");	

  TCanvas *c1 = new TCanvas("c1", "c1",0,52,1500,800);
  c1->SetGridy();
  c1->SetMargin(0.12, 0.01 ,0.12,0.07);
	
        std::vector<Double_t> x,y,c,d,e,f; 
	       std::vector<Double_t> mse,msmu,mspi,msk,msp; 

     //--------Muon Bethe Bloch---------------------
      for (Double_t p=10; p<=1000.;p=p+10)
     {
     	
        x.push_back(p*0.001);
        //----Energy of incident particles---------- 
        Double_t ms_el=0.,ms_mu=0., ms_pi=0., ms_k=0. , ms_p=0. ;    
        Double_t MSel=0.,MSmu=0., MSpi=0., MSk=0. , MSp=0. ;  
        
        MultipleScattering(mel,p,ms_el);
        MultipleScattering(mmu,p,ms_mu);
        MultipleScattering(mpi,p,ms_pi);
        MultipleScattering(mk,p,ms_k);
        MultipleScattering(mp,p,ms_p);   
 
        mse.push_back(ms_el*1000.); // convert to mrad from rad
        msmu.push_back(ms_mu*1000.);
        mspi.push_back(ms_pi*1000.);
        msk.push_back(ms_k*1000.);
        msp.push_back(ms_p*1000.);       
    }
 //----------Pion----------------------------------
      const Int_t n=x.size();
	     Double_t a[n], b[n];
	     for(Int_t i=0;i<n;i++)
      {
	     a[i]=x[i];
	     b[i]=mse[i]; 
	     } 
     
     TGraph *gr1 = new TGraph(n,a,b);
     gr1->SetLineColor(2);
    // gr1->GetYaxis()->SetRangeUser(0.0, 4.0);
     gr1->SetLineWidth(2);
     gr1->SetMarkerColor(2);
     gr1->SetMarkerStyle(6);
     gr1->SetTitle("Multiple Scattering");
     gr1->GetXaxis()->SetTitle("p (GeV/c)");
     gr1->GetXaxis()->CenterTitle(true);
     gr1->GetYaxis()->SetTitle("#theta_{p}(mrad)"); // #sigma_{x} (cm)
     gr1->GetYaxis()->CenterTitle(true);
     gr1->Draw("ACP");

	     for(Int_t i=0;i<n;i++)
      {
	     b[i]=msmu[i]; 
	     } 
            
     TGraph *gr2 = new TGraph(n,a,b);
     gr2->SetLineWidth(2);
     gr2->SetMarkerColor(kMagenta);
     gr2->SetLineColor(kMagenta);
     gr2->SetMarkerStyle(6); 
     gr2->Draw("same");

	     for(Int_t i=0;i<n;i++)
      {
	     b[i]=mspi[i]; 
	     } 
            
     TGraph *gr3 = new TGraph(n,a,b);
     gr3->SetLineWidth(2);
     gr3->SetMarkerColor(kBlack);
     gr3->SetLineColor(kBlack);
     gr3->SetMarkerStyle(6); 
     gr3->Draw("same");

	     for(Int_t i=0;i<n;i++)
      {
	     b[i]=msk[i]; 
	     } 
            
     TGraph *gr4 = new TGraph(n,a,b);
     gr4->SetLineWidth(2);
     gr4->SetMarkerColor(kBlue);
     gr4->SetLineColor(kBlue);
     gr4->SetMarkerStyle(6); 
     gr4->Draw("same");

	     for(Int_t i=0;i<n;i++)
      {
	     b[i]=msp[i]; 
	     } 
            
     TGraph *gr5 = new TGraph(n,a,b);
     gr5->SetLineWidth(2);
     gr5->SetMarkerColor(kGreen);
     gr5->SetLineColor(kGreen);
     gr5->SetMarkerStyle(6); 
     gr5->Draw("same");


 
    //--------------Legend Draw----------------------         
         TLegend *leg_hist = new TLegend(0.6,0.6,0.99,0.93);
         leg_hist->SetHeader("Particles in Silicon Tracker");
         leg_hist->SetTextFont(42);
         leg_hist->SetTextSize(0.04);
   
         leg_hist->AddEntry(gr1,"M.S. (Electron)","l"); 
         leg_hist->AddEntry(gr2,"M.S. (Muon)","l");
         leg_hist->AddEntry(gr3,"M.S. (Pion)","l");
         leg_hist->AddEntry(gr4,"M.S. (Kaon)","l");
         leg_hist->AddEntry(gr5,"M.S. (Proton)","l");
         leg_hist->Draw();
          
}

  // This will return theta in radian
		void MultipleScattering(Double_t &mp, Double_t &momI, Double_t &theta){
			Double_t En = TMath::Sqrt(mp*mp+momI*momI);
			Double_t beta = momI/En;
			theta=((13.6*charge)/(beta*momI))*(TMath::Sqrt(effradlen))*(1+0.038*TMath::Log(effradlen)); //*(1+0.038*TMath::Log(effradlen))
			}
			
