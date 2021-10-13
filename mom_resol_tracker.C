//  Momentum resolution for charged particles
// Shyam Kumar
// shyam.kumar@cern.ch

#include<TMath.h>
#include <TString.h>


  void MultipleScattering(Double_t mp, Double_t momI, Double_t &theta);
        
  
  // masses in MeV
	 Double_t mel = 0.51099891; // Mass of electron and positron
	       
	 Double_t mmu = 105.6583715;   // Mass of muons
	        
	 Double_t mpi = 139.57018;   // mass of charged pions
	        
	 Double_t mk = 493.667;   // mass of charged kaons
	        
	 Double_t mp = 938.272046;  // mass of protons and antiprotons
	 
	 Double_t speed= 3.0e8; //speed of light
	 
	 Int_t charge=1;
	  
	 Double_t effradlen=0.023; // All Silicon Tracker
	 
	 Double_t N = 6; // 2 Vtx + 4Barrel
  Double_t resolution = 10*1.0e-6/sqrt(12); // pixel resolution in meter
  Double_t length = 0.43; // Radius of outer Barrl layer in meter
  Double_t magfield = 3.0; // Mag field Eic 2.0 T

	
void mom_resol_tracker()
{
	
        gStyle->SetTitleSize(0.04,"");
        gStyle->SetTitleSize(0.04,"X");	
        gStyle->SetTitleSize(0.05,"Y");	
        gStyle->SetTitleOffset(1.05,"Y");	

  TCanvas *c1 = new TCanvas("c1", "c1",0,52,1500,800);
  c1->SetGridy();
  c1->SetMargin(0.12, 0.01 ,0.12,0.07);
	
        std::vector<Double_t> x,y,c,d,e,f; 
	       std::vector<Double_t> mre,mrmu,mrpi,mrk,mrp; 

     //--------Muon Bethe Bloch---------------------
      for (Double_t p=10; p<=30000.;p=p+100)
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

        
        // Length can be parameterize as a function of eta or theta
       // Momentum (pT) measurement error due to multiple scattering
       
        MSpi=(ms_pi*p*0.001)/(0.3*magfield*length*N);
        MSpi = 100.*MSpi*TMath::Sqrt((4*180*N*N*N)/((N-1)*(N+1)*(N+2)*(N+3)));
        
        MSp=(ms_p*p*0.001)/(0.3*magfield*length*N);
        MSp = 100.*MSp*TMath::Sqrt((4*180*N*N*N)/((N-1)*(N+1)*(N+2)*(N+3))); // Gluckstern = sqrt(720/(N+4))
       
        //  pT resolution due to curvature measurement B = 0.5T

        Double_t pi_pT_resol = p*0.001*resolution/(0.3*magfield*length*length); 
        pi_pT_resol = 100.*pi_pT_resol*TMath::Sqrt((4*180*N*N*N)/((N-1)*(N+1)*(N+2)*(N+3)));
        
        Double_t p_pT_resol = p*0.001*resolution/(0.3*magfield*length*length); // pT resolution due to curvature measurement
        p_pT_resol = 100.*p_pT_resol*TMath::Sqrt((4*180*N*N*N)/((N-1)*(N+1)*(N+2)*(N+3))); 
      
         d.push_back(MSpi);
    
         f.push_back(MSp);       
 
         mrpi.push_back(pi_pT_resol);
         mrp.push_back(p_pT_resol);

        
}
 //----------Pion----------------------------------
      const Int_t n=x.size();
	     Double_t a[n], b[n], g[n],h[n];
	     for(Int_t i=0;i<n;i++)
      {
	     a[i]=x[i];
	     b[i]=d[i]; 
	     g[i] = mrpi[i]; 
	     h[i] = TMath::Sqrt(b[i]*b[i]+g[i]*g[i]);
	     } 
	      
     TGraph *gr1 = new TGraph(n,a,b);
     gr1->SetLineColor(2);
     gr1->GetYaxis()->SetRangeUser(0.0, 4.0);
     gr1->SetLineWidth(1);
     gr1->SetMarkerColor(2);
     gr1->SetMarkerStyle(6);
     gr1->SetTitle("Transverse Momentum Resolution");
     gr1->GetXaxis()->SetTitle("p_{T} (GeV/c)");
     gr1->GetXaxis()->CenterTitle(true);
     gr1->GetYaxis()->SetTitle("#frac{#sigma_{p_{T}}}{p_{T}}"); // #sigma_{x} (cm)
     gr1->GetYaxis()->CenterTitle(true);
     gr1->Draw("ACP");
            
     TGraph *gr2 = new TGraph(n,a,g);
     gr2->SetLineWidth(1);
     gr2->SetMarkerColor(kMagenta);
     gr2->SetLineColor(kMagenta);
     gr2->SetMarkerStyle(6); 
     gr2->Draw("same");
            
     TGraph *gr1sumpi = new TGraph(n,a,h);
     gr1sumpi->SetLineWidth(1);
     gr1sumpi->SetMarkerColor(kBlack);
     gr1sumpi->SetLineColor(kBlack);
     gr1sumpi->SetMarkerStyle(6); 
     gr1sumpi-> SetLineStyle(9);
     gr1sumpi->Draw("same");
     
     //------------Proton-------------------------
            
	     for(Int_t i=0;i<n;i++)
      {
	     b[i]=f[i];
	     g[i] = mrp[i]; 
	     h[i] = TMath::Sqrt(b[i]*b[i]+g[i]*g[i]);
	     }  
           
      TGraph *gr3 = new TGraph(n,a,b);
      gr3->SetLineColor(3);
      gr3->SetLineWidth(1);
      gr3->SetMarkerColor(3);
      gr3->SetMarkerStyle(6);
      gr3->Draw("same");  
           
      TGraph *gr4 = new TGraph(n,a,g);
      gr4->SetMarkerColor(kBlue);
      gr4->SetLineColor(kBlue);
      gr4->SetLineWidth(1);
      gr4->SetMarkerStyle(6); 
      gr4->Draw("same");
            
      TGraph *gr1sump = new TGraph(n,a,h);
      gr1sump->SetLineWidth(1);
      gr1sump->SetMarkerColor(kMagenta);
      gr1sump->SetLineColor(kMagenta);
      gr1sump-> SetLineStyle(9);
      gr1sump->SetMarkerStyle(6); 
      gr1sump->Draw("same");

 
    //--------------Legend Draw----------------------         
         TLegend *leg_hist = new TLegend(0.6,0.6,0.99,0.93);
         leg_hist->SetHeader("Particles in Silicon Tracker");
         leg_hist->SetTextFont(42);
         leg_hist->SetTextSize(0.04);
   
         leg_hist->AddEntry(gr1,"p_{T} Resol. M.S. (Pion)","l"); 
         leg_hist->AddEntry(gr2,"p_{T} Resol. (Pion)","l");
         leg_hist->AddEntry(gr3,"p_{T} Resol. M.S. (Proton)","l");
         leg_hist->AddEntry(gr4,"p_{T} Resol. (Proton)","l");
         leg_hist->Draw();
         
         TLegend *leg_hist1 = new TLegend(0.25,0.8,0.55,0.93);  
         leg_hist1->SetTextFont(42);
         leg_hist1->SetTextSize(0.04);
   
         leg_hist1->AddEntry(gr1sumpi,"Sum p_{T} Resol. (Pion)","l"); 
         leg_hist1->AddEntry(gr1sump,"Sum p_{T} Resol. (Proton)","l");
         leg_hist1->Draw();  
        // c1->Print("Mom_resol.eps"); 
        // c1->SaveAs("Mom_resol.Pdf"); 
        // c1->SaveAs("Mom_resol.root");          
}

  // This will return theta in radian
		void MultipleScattering(Double_t mp, Double_t momI, Double_t &theta){
			Double_t En = TMath::Sqrt(mp*mp+momI*momI);
			Double_t beta = momI/En;
			theta=((13.6*charge)/(beta*momI))*(TMath::Sqrt(effradlen))*(1+0.038*TMath::Log(effradlen)); //*(1+0.038*TMath::Log(effradlen))
			}
			
