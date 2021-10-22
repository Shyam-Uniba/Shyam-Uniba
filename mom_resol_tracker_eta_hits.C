//  Momentum resolution for charged particles
// Shyam Kumar
// shyam.kumar@cern.ch

#include<TMath.h>
#include <TString.h>


  void MultipleScattering(Double_t &mp, Double_t &momI, Double_t &theta, Double_t &effradlen);
        
  
  // masses in MeV
	 Double_t mel = 0.51099891; // Mass of electron and positron
	       
	 Double_t mmu = 105.6583715;   // Mass of muons
	        
	 Double_t mpi = 139.57018;   // mass of charged pions
	        
	 Double_t mk = 493.667;   // mass of charged kaons
	        
	 Double_t mp = 938.272046;  // mass of protons and antiprotons
	 
	 Double_t speed= 3.0e8; //speed of light
	 
	 Int_t charge=1;
	  

	 
	 Double_t N = 7; // 3 Vtx + 4Barrel
  Double_t resolution = 10*1.0e-6/sqrt(12); // pixel resolution in meter
  Double_t magfield = 3.0; // Mag field Eic 2.0 T
  Double_t p =0.0; // p means pt
  Double_t effradlen=0.023; // All Silicon Tracker
  Double_t length = 0.0; // Radius of outer Barrel layer in meter  
  Double_t pmin = 20.;  
  Double_t pmax = 30.;     

	
void mom_resol_tracker_eta_hits()
{
	       

        gStyle->SetTitleSize(0.04,"");
        gStyle->SetTitleSize(0.04,"X");	
        gStyle->SetTitleSize(0.05,"Y");	
        gStyle->SetTitleOffset(1.05,"Y");	
        gStyle->SetOptTitle(0);	
        gStyle->SetOptStat(0);	

  TCanvas *c1 = new TCanvas("c1", "c1",0,52,1500,1000);
  c1->SetGridy();
  c1->SetMargin(0.12, 0.01 ,0.12,0.07);
	
        std::vector<Double_t> x,y,d; 
	       std::vector<Double_t> mrpi; 

   TFile *fmb = TFile::Open("mb_tracker.root");
   TProfile *fmb_prof = (TProfile*) fmb->Get("thetaprof");

   TFile* f = new TFile("SimpleNtuple.root"); // Tree with tracks and hits
   TNtuple* Tracks = (TNtuple*)f->Get("Tracks");
   TTree* hit = (TTree*) f->Get("Hits");
   Tracks->AddFriend(hit);

  float px, py, pz, trackId, pcax, pcay, pcaz;
  std::vector<float> *x0 = 0;
  std::vector<float> *y0 = 0;
  std::vector<float> *z0 = 0;
  std::vector<int> *hittrackId = 0;
  Tracks->SetBranchAddress("px", &px);
  Tracks->SetBranchAddress("py", &py);
  Tracks->SetBranchAddress("pz", &pz);
  Tracks->SetBranchAddress("trackId", &trackId);
  Tracks->SetBranchAddress("X0", &x0);
  Tracks->SetBranchAddress("Y0", &y0);
  Tracks->SetBranchAddress("Z0", &z0);
  Tracks->SetBranchAddress("TrackId", &hittrackId);

  Int_t ntracks = Tracks->GetEntriesFast();

  TProfile* etaprofile = new TProfile("etaprofile","Radiaion length (#theta)",40,-4.,4.);
//-----------------Main part of code-----------------------------------
  for (Int_t trackno=0; trackno<1000; trackno++)// Reading the entries in tree
  {
        Tracks->GetEntry(trackno);	
        if (trackId<0) continue; // Skipping tracks not reconstructed
        Double_t prec = sqrt(px*px+py*py+pz*pz);
        Double_t pt = sqrt(px*px+py*py); // 
        if (prec<pmin || prec>pmax) continue;
        Double_t eta = -1.0*TMath::Log(TMath::Tan((TMath::ACos(pz/prec))/2));

        Double_t ms_pi=0., MSpi=0.; 
        Int_t nhits = hittrackId->size(); 
        N = nhits; // N Points
       if (N<=1) continue;

       TVector3 firstpoint, lastpoint;
       firstpoint.SetXYZ(x0->at(0),y0->at(0),z0->at(0));
       lastpoint.SetXYZ(x0->at(nhits-1),y0->at(nhits-1),z0->at(nhits-1));
       length = ((lastpoint-firstpoint).Mag())*0.01; // meter from cm
       
       Int_t etabin = fmb_prof->GetXaxis()->FindBin(eta);
       effradlen = fmb_prof->GetBinContent(etabin);
       cout<<"Track No. "<<trackno<<" N: "<<nhits<<" Length: "<<length<<endl;
       cout<<"Eta: "<<eta<<" Radlength: "<<effradlen<<endl;
        
        MultipleScattering(mpi,prec,ms_pi,effradlen);


     
        // Length can be parameterize as a function of eta or theta
       // Momentum (pT) measurement error due to multiple scattering
       
        MSpi=(ms_pi*pt*0.001)/(0.3*magfield*length*N);
        MSpi = 100.*MSpi*TMath::Sqrt(720/(N+4));
        
       //  pT resolution due to curvature measurement B = 0.5T

        Double_t pi_pT_resol = pt*0.001*resolution/(0.3*magfield*length*length); 
        pi_pT_resol = 100.*pi_pT_resol*TMath::Sqrt(720/(N+4));

        Double_t sum_mom_resol = sqrt(pi_pT_resol*pi_pT_resol+ MSpi*MSpi);
   
        etaprofile->Fill(eta,sum_mom_resol);  
       cout<<"Sum: "<<sum_mom_resol<<endl;  
}

     c1->cd();
     etaprofile->GetXaxis()->SetTitle("#eta");
     etaprofile->GetYaxis()->SetTitle("#sigma_{p_{T}}/p_{T}");
     etaprofile->GetXaxis()->CenterTitle();
     etaprofile->GetYaxis()->CenterTitle();
     etaprofile->GetYaxis()->SetRangeUser(0.,10.0);
     etaprofile->SetMarkerStyle(20);
     etaprofile->SetMarkerSize(1.2);
     etaprofile->Draw("hist-P");
         
}

  // This will return theta in radian
		void MultipleScattering(Double_t &mp, Double_t &momI, Double_t &theta, Double_t &effradlen){
			Double_t En = TMath::Sqrt(mp*mp+momI*momI);
			Double_t beta = momI/En;
			theta=((13.6*charge)/(beta*momI))*(TMath::Sqrt(effradlen))*(1+0.038*TMath::Log(effradlen)); //*(1+0.038*TMath::Log(effradlen))
			}
			
