//  Momentum resolution for charged particles
// Shyam Kumar
// shyam.kumar@cern.ch

#include<TMath.h>
#include <TString.h>


  void MultipleScattering(Double_t mp, Double_t momI, Double_t &theta, Double_t effradlen);
        
  
  // masses in MeV
	 Double_t mel = 0.51099891; // Mass of electron and positron
	       
	 Double_t mmu = 105.6583715;   // Mass of muons
	        
	 Double_t mpi = 139.57018;   // mass of charged pions
	        
	 Double_t mk = 493.667;   // mass of charged kaons
	        
	 Double_t mp = 938.272046;  // mass of protons and antiprotons
	 
	 Double_t speed= 3.0e8; //speed of light
	 
	 Int_t charge=1;
	  
	 Double_t min_effradlen=0.013; // All Silicon Tracker
	 
	 Double_t N = 9; // 3 Vtx + 2Barrel + 4Micromegas
  Double_t resolution_silicon = 10*1.0e-6/sqrt(12); // pixel resolution in meter
  Double_t resolution_micromegas = 150*1.0e-6; // pixel resolution in meter micormegas
  Double_t resolution = (5.0*resolution_silicon+4.0*resolution_micromegas)/9.0; // pixel resolution in meter micormegas
  Double_t min_length = 0.7747; // Radius of outer Barrel layer-first layer
  Double_t magfield = 3.0; // Mag field Eic 3.0 T
  Double_t eta = 0.5;

	
void mom_resol_tracker()
{
	
        gStyle->SetTitleSize(0.04,"");
        gStyle->SetTitleSize(0.04,"X");	
        gStyle->SetTitleSize(0.04,"Y");	
        gStyle->SetTitleOffset(1.05,"Y");	

							TCanvas *c1 = new TCanvas("c1", "c1",0,52,1400,1000);
							c1->SetGridy();
							c1->SetMargin(0.12, 0.01 ,0.10,0.07);
				
        std::vector<Double_t> x,y,c,d,e,f; 
	       std::vector<Double_t> mre,mrmu,mrpi,mrk,mrp; 

     //--------Muon Bethe Bloch---------------------
      for (Int_t pt=350; pt<=10000.;pt=pt+100)
     {
     	
        x.push_back(pt*0.001);
        Double_t p = pt*TMath::CosH(eta);
        Double_t theta = 2.0*TMath::ATan(TMath::Exp(-1.0*eta));
        Double_t sin_theta = fabs(TMath::Sin(theta));

        Double_t length = fabs(min_length/sin_theta);
        Double_t effradlen = fabs(min_effradlen/sin_theta); 

        //----Energy of incident particles---------- 
        Double_t ms_el=0.,ms_mu=0., ms_pi=0., ms_k=0. , ms_p=0. ;    
        
        MultipleScattering(mel,p,ms_el,effradlen);
        MultipleScattering(mmu,p,ms_mu,effradlen);
        MultipleScattering(mpi,p,ms_pi,effradlen);
        MultipleScattering(mk,p,ms_k,effradlen);
        MultipleScattering(mp,p,ms_p,effradlen);

        
        // Length can be parameterize as a function of eta or theta
       // Momentum (pT) measurement error due to multiple scattering
       
        Double_t MSpi=100.*(ms_pi*pt*0.001)*TMath::Sqrt(720/(N+4))/(0.3*magfield*length*N);
        
       //  pT resolution due to curvature measurement B = 3.0T
        Double_t pi_pT_resol = 100.*pt*0.001*resolution*TMath::Sqrt(720/(N+4))/(0.3*magfield*length*length); 
        
        
        d.push_back(MSpi);      
        mrpi.push_back(pi_pT_resol);

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
     gr1->GetYaxis()->SetRangeUser(0.0, 2.0);
     gr1->SetLineWidth(1);
     gr1->SetMarkerColor(2);
     gr1->SetMarkerStyle(6);
     gr1->SetTitle(Form("Transverse Momentum Resolution for #eta = %1.1f",eta));
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
     
  
 
    //--------------Legend Draw----------------------         
         TLegend *leg_hist = new TLegend(0.65,0.7,0.99,0.93);
         leg_hist->SetHeader("Particle in Silicon Tracker");
         leg_hist->SetTextFont(42);
         leg_hist->SetTextSize(0.04);
   
         leg_hist->AddEntry(gr1,"p_{T} Resol. ( M.S.)","l"); 
         leg_hist->AddEntry(gr2,"p_{T} Resol. (Sagitta)","l");
         leg_hist->AddEntry(gr1sumpi,"Sum p_{T} Resol. (Pion)","l"); 
         leg_hist->Draw();
        // c1->SaveAs("Mom_resol.Pdf");           
}

  // This will return theta in radian
		void MultipleScattering(Double_t mp, Double_t momI, Double_t &theta, Double_t effradlen){
			Double_t En = TMath::Sqrt(mp*mp+momI*momI);
			Double_t beta = momI/En;
			theta=((13.6*charge)/(beta*momI))*(TMath::Sqrt(effradlen))*(1+0.038*TMath::Log(effradlen)); //*(1+0.038*TMath::Log(effradlen))
			}
			
