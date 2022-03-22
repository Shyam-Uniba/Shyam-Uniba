//  Momentum resolution for charged particles
// Shyam Kumar
// shyam.kumar@cern.ch

#include<TMath.h>
#include <TString.h>


 void GetGraphResolution(TGraph *gr[3], const Int_t n, Double_t pt[n], Double_t ms[n], Double_t arc[n], Double_t sum[n], Double_t eta,TString name ="");
  void MultipleScattering(Double_t mp, Double_t momI, Double_t &theta, Double_t effradlen);
  Double_t function_dX0SinTheta(Double_t mp, Double_t momI, Double_t effradlen); // // sigmatheta arXiv:1805.12014 [physics.ins-det] Werner Reigler
        
  
  // masses in MeV
	 Double_t mel = 0.51099891; // Mass of electron and positron
	       
	 Double_t mmu = 105.6583715;   // Mass of muons
	        
	 Double_t mpi = 139.57018;   // mass of charged pions
	        
	 Double_t mk = 493.667;   // mass of charged kaons
	        
	 Double_t mp = 938.272046;  // mass of protons and antiprotons
	 
	 Double_t speed= 3.0e8; //speed of light
	 
	 Int_t charge=1;
	  
  //Parameters from here: https://github.com/Shyam-Uniba/Shyam-Uniba/blob/main/TrackerFastSim/testDetectorUp.C	 
  Double_t N = 9; // 3 Vtx + 2Barrel + 4Micromegas
  Double_t resolution_silicon = 10*1.0e-6/sqrt(12); // pixel resolution in cm
  Double_t resolution_micromegas = 150*1.0e-6; // pixel resolution in cm micormegas
  Double_t avg_resolution = (5.0*resolution_silicon+4.0*resolution_micromegas)/9.0; // pixel resolution in meter micormegas
  // If we increase resolution slope will increase tune by looking at plots
  Double_t resolution = 38.0e-06; // Resolution in meter Caculated using Resolution
  Double_t vtx[3] ={3.3,4.35,5.4};
  Double_t Barr[2] = {13.34, 17.96};
  Double_t MM[4] = {47.72, 49.57, 75.61, 77.47};

  Double_t min_length = (MM[3]-vtx[0])*0.01; // minimum length of tracker in meter
  Double_t magfield = 3.0; // Mag field Eic 3.0 T

  Double_t effradl_beampipe = 0.0022; // Beam pipe
  Double_t effradl_vtx = 3.0*0.0005; // Three Vertex layers
  Double_t effradl_barrel = 2.0*0.0055; // Two Barrel layers
  Double_t effradl_mm = 4.0*0.004; // Micromegas layers 0.4% per layer
  Double_t NFactor_Pt = sqrt(720*N*N*N/((N-1)*(N+1)*(N+2)*(N+3)));
  Double_t N2 = pow(N,2);  Double_t N3 = pow(N,3); 
  Double_t r0 = vtx[0];  Double_t r0_2 = pow(vtx[0],2); Double_t r0_3 = pow(vtx[0],3); Double_t r0_4 = pow(vtx[0],4);
  Double_t l0 = min_length;  Double_t l0_2 = pow(min_length,2); Double_t l0_3 = pow(min_length,3); Double_t l0_4 = pow(min_length,4);

  Double_t NFactor_d0XY_Arc = sqrt((N3-N/3.-2./3)+4*(2*N3-N2-N)*r0/l0+4*(7*N3-N2-N)*r0_2/l0_2+40.*N3*r0_3/l0_3+20.*N3*r0_4/l0_4);
  Double_t NFactor_d0XY_MS = sqrt((N-3./4)/(N-1)+0.5*(N/(N-1))*r0/l0+0.25*(N2/(N-1))*r0_2/l0_2);

	
void tracker_performance(Double_t eta = 0.0)
{
	
        gStyle->SetTitleSize(0.04,"");
        gStyle->SetTitleSize(0.04,"X");	
        gStyle->SetTitleSize(0.04,"Y");	
        gStyle->SetTitleOffset(1.05,"Y");

        TCanvas *c1[5];	

        for (int i=0; i<2; i++){
        c1[i] = new TCanvas(Form("c1_%d",i), Form("c1_%d",i),0,52,1400,1000);
        c1[i]->SetGridy();
        c1[i]->SetMargin(0.12, 0.01 ,0.10,0.07);     
        }

        std::vector<Double_t> x,y,c,d,e,f; 
	    std::vector<Double_t> pT_Arc, pT_MS, d0XY_Arc, d0XY_MS, d0Z_Arc, d0Z_MS;
        pT_Arc.clear(); pT_MS.clear(); d0XY_Arc.clear(); 
        d0XY_MS.clear(); d0Z_Arc.clear(); d0Z_MS.clear();

     //--------Muon Bethe Bloch---------------------
      for (Int_t pt=350; pt<=30000.;pt=pt+100)
     {
     	
        x.push_back(pt*0.001);
        Double_t p = pt*TMath::CosH(eta);
        Double_t theta = 2.0*TMath::ATan(TMath::Exp(-1.0*eta));
        Double_t sin_theta = fabs(TMath::Sin(theta));

        Double_t length = fabs(min_length/sin_theta);

         Double_t magfield_eta = magfield*sin_theta;
        Double_t effradl_beampipe_eta = fabs(effradl_beampipe/sin_theta); 
        Double_t effradl_vtx_eta = fabs(effradl_vtx/sin_theta); 
	    Double_t effradl_barrel_eta = fabs(effradl_barrel/sin_theta); 
	    Double_t effradl_mm_eta = fabs(effradl_mm/sin_theta);
        Double_t length_vtx = (Barr[0]-vtx[0])*0.01/sin_theta; 
        Double_t length_barr = (MM[0]-Barr[0])*0.01/sin_theta; 
        Double_t length_mm = (MM[3]-MM[0])*0.01/sin_theta; 

        Double_t sum_effradl = effradl_beampipe_eta+effradl_vtx_eta+effradl_barrel_eta+effradl_mm_eta;

        //----Energy of incident particles---------- 
        Double_t ms_pi_beampipe=0., ms_pi_vtx=0., ms_pi_barr=0. , ms_pi_mm=0., sum_ms_pi=0.;    
       
        MultipleScattering(mpi,p,ms_pi_beampipe,effradl_beampipe_eta);
        MultipleScattering(mpi,p,ms_pi_vtx,effradl_vtx_eta);
        MultipleScattering(mpi,p,ms_pi_barr,effradl_barrel_eta);
        MultipleScattering(mpi,p,ms_pi_mm,effradl_mm_eta);
        MultipleScattering(mpi,p,sum_ms_pi,sum_effradl);

        Double_t En = sqrt(mpi*mpi+p*p);
        Double_t beta = p/En;
        
        // Length can be parameterize as a function of eta or theta
       // Momentum (pT) measurement error due to multiple scattering
       
       // Double_t MSpi=100.*pt*0.001*sqrt(720/(N+5))*(sqrt((length_vtx*ms_pi_vtx/3)*(length_vtx*ms_pi_vtx/3)+(length_barr*ms_pi_barr/2)*(length_barr*ms_pi_barr/2)+(length_mm*ms_pi_mm/4)*(length_mm*ms_pi_mm/4)))/(0.3*magfield*sin_theta*length*length);
        
       // Double_t MSpi=100.*(N/sqrt(N*N-1))*0.0136*(1/(0.3*magfield*length*beta))*(sqrt(sum_effradl))*(1+0.038*TMath::Log(sum_effradl));
        Double_t pT_MS_cal=100.*pt*0.001*(N/sqrt(N*N-1))*1/(0.3*magfield*length)*sum_ms_pi; // Above formula 
       //  pT resolution due to curvature measurement B = 3.0T
        Double_t pT_Arc_cal = 100.*pt*0.001*resolution*NFactor_Pt/(0.3*magfield_eta*length*length); 
        
        pT_MS.push_back(pT_MS_cal);      
        pT_Arc.push_back(pT_Arc_cal);

        // Transverse Pointing Resolution
        Double_t d0XY_Arc_cal = 3.0*resolution_silicon*1.0/(sqrt((N-1)*(N+1)*(N+2)*(N+3)))*NFactor_d0XY_Arc;
      //  Double_t d0XY_Arc_approx = 3.0*resolution*1.0/(sqrt(N+5))*sqrt(1+8.0*r0/l0+28.0*r0_2/l0_2+40.*r0_3/l0_3+20.*r0_4/l0_4);
       // cout<<"Arc PAXY Full: "<<d0XY_Arc_cal<<endl;
       // cout<<"Arc PAXY Approx: "<<d0XY_Arc_approx<<endl;
        Double_t d0XY_MS_cal = (r0*0.01/(beta*pt*0.001))*function_dX0SinTheta(mpi,p,sum_effradl)*NFactor_d0XY_MS;
        d0XY_MS.push_back(d0XY_MS_cal);
        d0XY_Arc.push_back(d0XY_Arc_cal); 

     }
 //----------Pion----------------------------------
      const Int_t n=x.size();
	  Double_t a[n], b[n], g[n],h[n];

	     for(Int_t i=0;i<n;i++)
    {
	     a[i]=x[i];
	     b[i]=pT_MS[i]; 
	     g[i] = pT_Arc[i]; 
	     h[i] = sqrt(b[i]*b[i]+g[i]*g[i]);
	 } 


	// Pt Resolution Estimation    
    TGraph *gr_pT[3];
    GetGraphResolution(gr_pT, n, a, b, g, h, eta,"Pt_Resol");
    c1[0]->cd();
    gr_pT[0]->Draw("ACP");
    gr_pT[1]->Draw("same");
    gr_pT[2]->Draw("same");

    //--------------Legend Draw----------------------         
    TLegend *leg_hist = new TLegend(0.60,0.7,0.99,0.93);
    leg_hist->SetHeader("Particle in Silicon Tracker");
    leg_hist->SetTextFont(42);
    leg_hist->SetTextSize(0.04);
    leg_hist->AddEntry(gr_pT[0],"p_{T} Resol. ( M.S.)","l"); 
    leg_hist->AddEntry(gr_pT[1],"p_{T} Resol. (Sagitta)","l");
    leg_hist->AddEntry(gr_pT[2],"Sum p_{T} Resol. (Pion)","l"); 
    leg_hist->Draw();
    
    // Transverse Pointing Resolution
    for(Int_t i=0;i<n;i++)
    {
         a[i]=x[i];
         b[i]= d0XY_MS[i]; 
         g[i] = d0XY_Arc[i]; 
         h[i] = sqrt(b[i]*b[i]+g[i]*g[i]);
     } 

    TGraph *gr_PAXY[3];
    GetGraphResolution(gr_PAXY, n, a, b, g, h, eta,"PA_ResolXY");
    c1[1]->cd();
    gr_PAXY[0]->Draw("ACP");
    gr_PAXY[1]->Draw("same");
    gr_PAXY[2]->Draw("same");

    //--------------Legend Draw----------------------         
    leg_hist = new TLegend(0.60,0.7,0.99,0.93);
    leg_hist->SetHeader("Particle in Silicon Tracker");
    leg_hist->SetTextFont(42);
    leg_hist->SetTextSize(0.04);
    leg_hist->AddEntry(gr_PAXY[0],"PA_{xy}  Resol. ( M.S.)","l"); 
    leg_hist->AddEntry(gr_PAXY[1],"PA_{xy}  Resol. (Intrinsic)","l");
    leg_hist->AddEntry(gr_PAXY[2],"Sum PA_{xy} Resol. (Pion)","l"); 
    leg_hist->Draw();


     
    TFile *fout = new TFile(Form("Resolution_%1.1f.root",eta),"recreate");
    fout->cd();
    gr_pT[0]->Write();
    gr_pT[1]->Write();
    gr_pT[2]->Write();
    gr_PAXY[0]->Write();
    gr_PAXY[1]->Write();
    gr_PAXY[2]->Write();
    fout->Close();
 

        // c1->SaveAs("Mom_resol.Pdf");           
}

  // This will return theta in radian
		void MultipleScattering(Double_t mp, Double_t momI, Double_t &theta, Double_t effradlen){
			Double_t En = sqrt(mp*mp+momI*momI);
			Double_t beta = momI/En;
            Double_t fy = ((13.6*charge)*(sqrt(effradlen))*(1+0.038*TMath::Log(effradlen)));
            theta = fy/(beta*momI); // sigmatheta arXiv:1805.12014 [physics.ins-det] Werner Reigler
			}

         Double_t function_dX0SinTheta(Double_t mp, Double_t momI, Double_t effradlen){
              
               Double_t En = sqrt(mp*mp+momI*momI);
               Double_t beta = momI/En;
               Double_t fy = ((13.6*charge)*(sqrt(effradlen))*(1+0.038*TMath::Log(effradlen)));
               return fy;

            }

            void GetGraphResolution(TGraph *gr[3], const Int_t n, Double_t *pt, Double_t *ms, Double_t *arc, Double_t *sum, Double_t eta, TString name ="")
            {

             gr[0] = new TGraph(n,pt,ms);
             gr[0]->SetLineColor(2);
             if (name=="Pt_Resol"){
             gr[0]->SetName("Pt_Resol_MS");
             gr[0]->SetTitle(Form("Transverse Momentum Resolution for #eta = %1.1f",eta)); 
             gr[0]->GetYaxis()->SetRangeUser(0.0, 5.0);
             gr[0]->GetYaxis()->SetTitle("#frac{#sigma_{p_{T}}}{p_{T}}"); // #sigma_{x} (cm) 
             }
                
             else if (name=="PA_ResolXY"){
             gr[0]->SetName("PA_Resol_MSXY");
             gr[0]->SetTitle(Form("Transverse Pointing Resolution for #eta = %1.1f",eta)); 
             gr[0]->GetYaxis()->SetRangeUser(0.0, 100.0);
             gr[0]->GetYaxis()->SetTitle("Transverse Pointing Resolution (#mum)"); // #sigma_{x} (cm) 
            }
             else if (name =="PA_ResolZ"){
            gr[0]->SetName("PA_Resol_MSZ"); 
            gr[0]->SetTitle(Form("Longitudinal Pointing Resolution for #eta = %1.1f",eta)); 
            gr[0]->GetYaxis()->SetRangeUser(0.0, 1000.0);
            gr[0]->GetYaxis()->SetTitle("Longitudinal Pointing Resolution (#mum)"); // #sigma_{x} (cm)  
             } 
             
             gr[0]->SetLineWidth(1);
             gr[0]->SetMarkerColor(2);
             gr[0]->SetMarkerStyle(6);             
             gr[0]->GetXaxis()->SetTitle("p_{T} (GeV/c)");
             gr[0]->GetXaxis()->CenterTitle(true);           
             gr[0]->GetYaxis()->CenterTitle(true); 
           
             gr[1] = new TGraph(n,pt,arc);
             gr[1]->SetLineWidth(1);
             if (name=="Pt_Resol") gr[1]->SetName("Pt_Resol_Arc");
             else if (name=="PA_ResolXY") gr[1]->SetName("PA_ResolXY_Arc");
             else if (name=="PA_ResolZ") gr[1]->SetName("PA_ResolZ_Arc");
             gr[1]->SetMarkerColor(kMagenta);
             gr[1]->SetLineColor(kMagenta);
             gr[1]->SetMarkerStyle(6); 

             gr[2] = new TGraph(n,pt,sum);
             gr[2]->SetLineWidth(1);
             if (name=="Pt_Resol") gr[2]->SetName("Sum_Pt_Resol");
             else if (name=="PA_ResolXY") gr[2]->SetName("Sum_PA_ResolXY");
             else if (name=="PA_ResolZ") gr[2]->SetName("Sum_PA_ResolZ");
             gr[2]->SetTitle(Form("#eta = %1.2f",eta));
             gr[2]->SetMarkerColor(kBlack);
             gr[2]->SetLineColor(kBlack);
             gr[2]->SetMarkerStyle(6); 
             gr[2]-> SetLineStyle(9);
              
            }
			
