//  Momentum resolution for charged particles
// Shyam Kumar
// shyam.kumar@cern.ch
// Code based on https://arxiv.org/abs/1805.12014 formula's by Werner Reigler

#include<TMath.h>
#include <TString.h>


	void MultipleScatteringComponent(Double_t mp, Double_t momI, Double_t &Ptresol, Double_t effradlen, Int_t N);
        
         
	 Double_t mpi = 139.57018;   // mass of charged pions
	 
	 Double_t speed= 3.0e8; //speed of light
	 
	 Int_t charge=1;
	  
	 Double_t min_effradlen=50.e-4/9.37; // Silicon of thickness 50mum
	 
	 Double_t N = 6; // 6 silicon layers from 2,3,4,5,6 and 7 cm
  Double_t resolution_rphi = 10*1.0e-6/sqrt(12); // pixel resolution_rphi in meter
  Double_t resolution_z = 10*1.0e-6/sqrt(12); // pixel resolution_z in meter
  Double_t min_length = 0.05; // Radius of outer Barrel layer-first layer
  Double_t magfield = 3.0; // Mag field Eic 3.0 T
  Double_t eta = 0.0;
  Double_t r0 = 0.02; // 2cm radius of first layer

  Double_t N_factor_Pt_Arc = sqrt(720*pow(N,3)/((N-1)*(N+1)*(N+2)*(N+3)));
  Double_t N_factor_Pt_MS = sqrt(N/(pow(N,2)-1));

  Double_t N_2 = pow(N,2); Double_t N_3 = pow(N,3);
  Double_t r0_l0 = r0/min_length; Double_t r0_l0_2 = pow(r0_l0,2); Double_t r0_l0_3 = pow(r0_l0,3); Double_t r0_l0_4 = pow(r0_l0,4);
 
  Double_t N_factor_Pt_TransPA_1 = 1/sqrt((N-1)*(N+1)*(N+2)*(N+3));
  Double_t N_factor_Pt_TransPA_2 = sqrt((N_3-N/3.0-2.0/3)+(4*(2*N_3-N_2-N)*r0_l0)+(4*(7*N_3-N_2-N)*r0_l0_2)+40*N_3*r0_l0_3+20*N_3*r0_l0_4);

  Double_t N_factor_Pt_TransPA_SR = N_factor_Pt_TransPA_1*N_factor_Pt_TransPA_2; // Spatial resolution_rphi
  Double_t N_factor_Pt_TransPA_MS = sqrt((N-3./4)/(N-1)+N/(2.0*(N-1))*r0_l0+N_2/(4.0*(N-1))*r0_l0_2);

  Double_t N_factor_Pt_LongPA_1 = 1./sqrt((N+1)*(N+2)); 
  Double_t N_factor_Pt_LongPA_2 = sqrt((N+1./2)+3.0*N*r0_l0+3.0*N*r0_l0_2);
  Double_t N_factor_Pt_LongPA_SR = N_factor_Pt_LongPA_1*N_factor_Pt_LongPA_2; // Spatial resolution_z
	
void Resolution_Tracker()
{
	
        gStyle->SetTitleSize(0.04,"");
        gStyle->SetTitleSize(0.04,"X");	
        gStyle->SetTitleSize(0.04,"Y");	
        gStyle->SetTitleOffset(1.05,"Y");	

							TCanvas *c1 = new TCanvas("c1", "c1",0,52,1400,1000);
							c1->SetGridy();
							c1->SetMargin(0.12, 0.01 ,0.10,0.07);

							TCanvas *c2 = new TCanvas("c2", "c2",0,52,1400,1000);
							c2->SetGridy();
							c2->SetMargin(0.12, 0.01 ,0.10,0.07);

							TCanvas *c3 = new TCanvas("c3", "c3",0,52,1400,1000);
							c3->SetGridy();
							c3->SetMargin(0.12, 0.01 ,0.10,0.07);
				
        std::vector<Double_t> x,y,c,d,e,f; 
	       std::vector<Double_t> pt_resol_arc,pt_resol_ms,Trans_PA_resol_arc,Trans_PA_resol_ms,Long_PA_resol_arc,Long_PA_resol_ms; 

        x.clear(); y.clear(); c.clear(); d.clear(); e.clear(); f.clear();
        pt_resol_arc.clear(); pt_resol_ms.clear(); Trans_PA_resol_arc.clear(); Trans_PA_resol_ms.clear();
        Long_PA_resol_arc.clear(); Long_PA_resol_ms.clear();

     //--------Muon Bethe Bloch---------------------
      for (Int_t pt=100; pt<=10000.;pt=pt+100)
     {
     	
        x.push_back(pt*0.001);
        Double_t p = pt*TMath::CosH(eta);
        Double_t theta = 2.0*TMath::ATan(TMath::Exp(-1.0*eta));
        Double_t sin_theta = fabs(TMath::Sin(theta));

        Double_t length = fabs(min_length/sin_theta);
        Double_t effradlen = fabs(min_effradlen/sin_theta); 

        //----Energy of incident particles---------- 
        Double_t ms_el=0.,ms_mu=0., ms_pi=0., ms_k=0. , ms_p=0. ;    
   
        MultipleScatteringComponent(mpi,p,ms_pi,effradlen,N);
           
       //  pT resolution 
        Double_t pi_pT_resol_Arc = 100.*pt*0.001*resolution_rphi*N_factor_Pt_Arc/(0.3*magfield*length*length); 
        Double_t pi_pT_MS = 100.*N_factor_Pt_MS*ms_pi/(0.3*magfield*length);  

       //  Transverse Pointing Angle Resolution
        Double_t Trans_PA_resol_Arc = 3.0*resolution_rphi*N_factor_Pt_TransPA_SR; 
        Double_t Trans_PA_resol_MS = r0/(pt*0.001)*ms_pi*N_factor_Pt_TransPA_MS;    

       //  Longitudinal Pointing Angle Resolution
        Double_t Long_PA_resol_Arc = 2.0*resolution_z*N_factor_Pt_LongPA_SR; 
        Double_t Long_PA_resol_MS = r0/(pt*0.001*sin_theta)*ms_pi;   
        
     
        pt_resol_arc.push_back(pi_pT_resol_Arc);
        pt_resol_ms.push_back(pi_pT_MS);  
        
        Trans_PA_resol_arc.push_back(Trans_PA_resol_Arc);
        Trans_PA_resol_ms.push_back(Trans_PA_resol_MS); 

        Long_PA_resol_arc.push_back(Long_PA_resol_Arc);
        Long_PA_resol_ms.push_back(Long_PA_resol_MS); 

     }
 //----------Pion Pt Resolution----------------------------------
      const Int_t n=x.size();
	     Double_t a[n], b[n], g[n],h[n];
	     for(Int_t i=0;i<n;i++)
      {
	     a[i]=x[i];
	     b[i]=  pt_resol_ms[i]; 
	     g[i] = pt_resol_arc[i]; 
	     h[i] = TMath::Sqrt(b[i]*b[i]+g[i]*g[i]);
	     } 
	      
     TGraph *gr1 = new TGraph(n,a,b);
     gr1->SetLineColor(2);
     gr1->GetYaxis()->SetRangeUser(0.0, 20.0);
     gr1->SetLineWidth(1);
     gr1->SetMarkerColor(2);
     gr1->SetMarkerStyle(6);
     gr1->SetTitle(Form("Transverse Momentum Resolution for #eta = %1.1f",eta));
     gr1->GetXaxis()->SetTitle("p_{T} (GeV/c)");
     gr1->GetXaxis()->CenterTitle(true);
     gr1->GetYaxis()->SetTitle("#frac{#sigma_{p_{T}}}{p_{T}}"); // #sigma_{x} (cm)
     gr1->GetYaxis()->CenterTitle(true);
     c1->cd();
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
     leg_hist->SetTextSize(0.03);   
     leg_hist->AddEntry(gr1,"p_{T} Resol. ( M.S.)","l"); 
     leg_hist->AddEntry(gr2,"p_{T} Resol. (Sagitta)","l");
     leg_hist->AddEntry(gr1sumpi,"Sum p_{T} Resol. (Pion)","l"); 
     leg_hist->Draw();

  //----------Pion Transverse Pointing Resolution----------------------------------	 
    for(Int_t i=0;i<n;i++)
      {
	     a[i]=x[i];
	     b[i]=  Trans_PA_resol_ms[i]*1.0e+6; 
	     g[i] = Trans_PA_resol_arc[i]*1.0e+6; 
	     h[i] = TMath::Sqrt(b[i]*b[i]+g[i]*g[i]);
	     } 
	      
     gr1 = new TGraph(n,a,b);
     gr1->SetLineColor(2);
     gr1->GetYaxis()->SetRangeUser(0.0, 100.0);
     gr1->SetLineWidth(1);
     gr1->SetMarkerColor(2);
     gr1->SetMarkerStyle(6);
     gr1->SetTitle(Form("Transverse Pointing Resolution for #eta = %1.1f",eta));
     gr1->GetXaxis()->SetTitle("p_{T} (GeV/c)");
     gr1->GetXaxis()->CenterTitle(true);
     gr1->GetYaxis()->SetTitle("Transverse Pointing Resolution (#mum)"); // #sigma_{x} (cm)
     gr1->GetYaxis()->CenterTitle(true);
     c2->cd();
     gr1->Draw("ACP");
            
     gr2 = new TGraph(n,a,g);
     gr2->SetLineWidth(1);
     gr2->SetMarkerColor(kMagenta);
     gr2->SetLineColor(kMagenta);
     gr2->SetMarkerStyle(6); 
     gr2->Draw("same");
            
     gr1sumpi = new TGraph(n,a,h);
     gr1sumpi->SetLineWidth(1);
     gr1sumpi->SetMarkerColor(kBlack);
     gr1sumpi->SetLineColor(kBlack);
     gr1sumpi->SetMarkerStyle(6); 
     gr1sumpi-> SetLineStyle(9);
     gr1sumpi->Draw("same");
     
    //--------------Legend Draw----------------------         
     leg_hist = new TLegend(0.60,0.7,0.99,0.93);
     leg_hist->SetHeader("Particle in Silicon Tracker");
     leg_hist->SetTextFont(42);
     leg_hist->SetTextSize(0.03);   
     leg_hist->AddEntry(gr1,"Transverse PA Resol. ( M.S.)","l"); 
     leg_hist->AddEntry(gr2,"Transverse PA Resol. (Sagitta)","l");
     leg_hist->AddEntry(gr1sumpi,"Sum Transverse PA Resol. (Pion)","l"); 
     leg_hist->Draw();

  //----------Longitudinal Pointing Resolution----------------------------------	 
    for(Int_t i=0;i<n;i++)
      {
	     a[i]=x[i];
	     b[i]=  Long_PA_resol_ms[i]*1.0e+6; 
	     g[i] = Long_PA_resol_arc[i]*1.0e+6; 
	     h[i] = TMath::Sqrt(b[i]*b[i]+g[i]*g[i]);
	     } 
	      
     gr1 = new TGraph(n,a,b);
     gr1->SetLineColor(2);
     gr1->GetYaxis()->SetRangeUser(0.0, 100.0);
     gr1->SetLineWidth(1);
     gr1->SetMarkerColor(2);
     gr1->SetMarkerStyle(6);
     gr1->SetTitle(Form("Longitudinal Pointing Resolution for #eta = %1.1f",eta));
     gr1->GetXaxis()->SetTitle("p_{T} (GeV/c)");
     gr1->GetXaxis()->CenterTitle(true);
     gr1->GetYaxis()->SetTitle("Longitudinal Pointing Resolution (#mum)"); // #sigma_{x} (cm)
     gr1->GetYaxis()->CenterTitle(true);
     c3->cd();
     gr1->Draw("ACP");
            
     gr2 = new TGraph(n,a,g);
     gr2->SetLineWidth(1);
     gr2->SetMarkerColor(kMagenta);
     gr2->SetLineColor(kMagenta);
     gr2->SetMarkerStyle(6); 
     gr2->Draw("same");
            
     gr1sumpi = new TGraph(n,a,h);
     gr1sumpi->SetLineWidth(1);
     gr1sumpi->SetMarkerColor(kBlack);
     gr1sumpi->SetLineColor(kBlack);
     gr1sumpi->SetMarkerStyle(6); 
     gr1sumpi-> SetLineStyle(9);
     gr1sumpi->Draw("same");
     
    //--------------Legend Draw----------------------         
     leg_hist = new TLegend(0.60,0.7,0.99,0.93);
     leg_hist->SetHeader("Particle in Silicon Tracker");
     leg_hist->SetTextFont(42);
     leg_hist->SetTextSize(0.03);   
     leg_hist->AddEntry(gr1,"Longitudinal PA Resol. ( M.S.)","l"); 
     leg_hist->AddEntry(gr2,"Longitudinal PA Resol. (Sagitta)","l");
     leg_hist->AddEntry(gr1sumpi,"Sum Longitudinal PA Resol. (Pion)","l"); 
     leg_hist->Draw();


          
}
  // This will return theta in radian
		void MultipleScatteringComponent(Double_t mp, Double_t momI, Double_t &Ptresol, Double_t effradlen, Int_t N){
			Double_t En = TMath::Sqrt(mp*mp+momI*momI);
			Double_t beta = momI/En;
			Ptresol=(0.0136/beta)*(TMath::Sqrt(effradlen))*(1+0.038*TMath::Log(effradlen)); //*(1+0.038*TMath::Log(effradlen))
			}
			
