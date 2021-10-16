///////////// code for checking the basic things works in ALICE and also in PANDA of particles by shyam kumar

#include "TGraphErrors.h"
#include "TF1.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TMath.h"
#define mpi 0.139  // 1.864 GeV/c^2

void Plot_Efficiency(float etamin, float etamax)
{

////////////////////////////////////////////////////// style of the plot
   gStyle->SetPalette(1);
   gStyle->SetOptTitle(0);
   gStyle->SetTitleOffset(.85,"X");gStyle->SetTitleOffset(.85,"Y");
   gStyle->SetTitleSize(.05,"X");gStyle->SetTitleSize(.05,"Y");
   gStyle->SetLabelSize(.04,"X");gStyle->SetLabelSize(.04,"Y");
   gStyle->SetHistLineWidth(2);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);

///////////////////////////////////////////////Reading the root file
		TFile *f = TFile::Open("../out_AllSi_vbd_0.05_0.55_0.24_GEM_res_50.0um_DIRC_12_sect_B_ATHENA_210507_FastSimEval.root");
		TTree *Tracks = (TTree*)f->Get("tracks");  

		TCanvas * c1 = new TCanvas("c1","coutput",1400,1000);
		c1->SetMargin(0.10, 0.10 ,0.1,0.05);
		TCanvas * c2 = new TCanvas("c2","coutput",1400,1000);
		c2->SetMargin(0.10, 0.10 ,0.1,0.05);
//		TCanvas * c2 = new TCanvas("c2","coutput",1400,1000);
//		c2->SetMargin(0.10, 0.10 ,0.1,0.05);


		TH1D *hpt_rec= new TH1D("hpt_rec","Reco PT",240,0.,30.);
		TH1D *hpt_mc= new TH1D("hpt_mc","MC PT",240,0.,30.);
		TH1D *hp_rec= new TH1D("hp_rec","Reco Mom",240,0.,30.);
		TH1D *hp_mc= new TH1D("hp_mc","MC Mom",240,0.,30.);
		TH1D *h_eta= new TH1D("h_eta","h_eta",500,etamin,etamax);

  Int_t trackID, gtrackID;
		Float_t gpx, gpy, gpz; 
		Float_t px, py, pz, dca2d; // MC vertex position is (0,0,0)
		// pca: Point of Closest Approach After Extrapolation
		Tracks->SetBranchAddress("gpx", &gpx);
		Tracks->SetBranchAddress("gpy", &gpy);
		Tracks->SetBranchAddress("gpz", &gpz);
		Tracks->SetBranchAddress("px", &px);
		Tracks->SetBranchAddress("py", &py);
		Tracks->SetBranchAddress("pz", &pz);
		Tracks->SetBranchAddress("dca2d", &dca2d);
		Tracks->SetBranchAddress("trackID", &trackID);
		Tracks->SetBranchAddress("gtrackID", &gtrackID);


  Int_t n = (Int_t)Tracks->GetEntries();
  cout << "Number of Events" << n << endl;
 
  for(Int_t i = 0; i <n; i++){
      Tracks->GetEntry(i);
      Double_t prec = sqrt(px*px+py*py+pz*pz);   
      Double_t pmc = sqrt(gpx*gpx+gpy*gpy+gpz*gpz);
      Double_t ptrec = sqrt(px*px+py*py);   
      Double_t ptmc = sqrt(gpx*gpx+gpy*gpy);
      Double_t Erec = sqrt(px*px+py*py+pz*pz+mpi*mpi);   
      Double_t Emc = sqrt(gpx*gpx+gpy*gpy+gpz*gpz+mpi*mpi);
      Double_t p_resol = (prec-pmc)/pmc;
      Double_t pt_resol = (ptrec-ptmc)/ptmc;

      Double_t ymc = 0.5*log((Emc+gpz)/(Emc-gpz));
      Double_t yrec = 0.5*log((Erec+pz)/(Erec-pz));
      Double_t etamc = -1.0*TMath::Log(TMath::Tan((TMath::ACos(gpz/pmc))/2));
      Double_t etarec = -1.0*TMath::Log(TMath::Tan((TMath::ACos(pz/prec))/2));

     if (etamc>etamin && etamc<etamax ){
     //	cout<<"ptmc: "<<ptmc<<"\t ptrec: "<<ptrec<<"\t gTrackId: "<<gtrackID<<"\t TrackId: "<<trackID<<endl;
     	hp_mc->Fill(pmc);
     	hpt_mc->Fill(ptmc);
      if (trackID==0){
      hp_rec->Fill(pmc);
      hpt_rec->Fill(ptmc);
      }      
      h_eta->Fill(etarec);
          
     } 
   }
  hpt_rec->Sumw2();
  hpt_mc->Sumw2();
//  hpt_rec->Divide(hpt_mc);
  hp_rec->Sumw2();
  hp_mc->Sumw2();
 // hp_rec->Divide(hp_mc);

  
  c1->cd();
  hpt_rec->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  hpt_rec->GetYaxis()->SetTitle("Efficiency");
  hpt_rec->SetLineColor(kMagenta);
  hpt_rec->Draw("EP");



  TFile *fout = new TFile(Form("eff_p_pt_eta_%1.1f_%1.1f.root",etamin,etamax),"recreate");
  fout->cd();
  hpt_rec->Write();
  hp_rec->Write();
  hpt_mc->Write();
  hp_mc->Write();
  fout->Close();

 // c2->cd();
  h_eta->GetXaxis()->SetTitle("#eta");
  h_eta->GetYaxis()->SetTitle("Entries (a.u.)");
//  h_eta->Draw("hist"); 
 // c2->SaveAs(Form("eta%1.1f_eta_%1.1f.png",etamin,etamax));
  c2->cd();
  hp_rec->Divide(hp_mc);
  hp_rec->Sumw2();
  hp_rec->GetXaxis()->SetTitle("p (GeV/c)");
  hp_rec->GetYaxis()->SetTitle("Efficiency");
  hp_rec->SetLineColor(kMagenta);
  hp_rec->Draw("EP");
  c2->SaveAs(Form("effvsp_%1.1f_eta_%1.1f.png",etamin,etamax));



}
