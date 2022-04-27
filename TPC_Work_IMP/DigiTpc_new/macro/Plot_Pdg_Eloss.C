void Plot_Pdg_Eloss(){

   gStyle->SetPalette(1);
   gStyle->SetOptTitle(1);
   gStyle->SetTitleOffset(.85,"X");gStyle->SetTitleOffset(.85,"Y");
   gStyle->SetTitleSize(.04,"XY");gStyle->SetTitleSize(.04,"t");
   gStyle->SetLabelSize(.04,"XY");
   gStyle->SetHistLineWidth(2);
   gStyle->SetOptFit(1);
    gStyle->SetOptStat(111111);
 //  gStyle->SetOptStat(0); 

 TFile* file = new TFile("../../../macro/test.root");
 TTree *t=(TTree *) file->Get("ceesim") ;
 TClonesArray *tpc_array=new TClonesArray("CeeTpcPoint");
 t->SetBranchAddress("CeeTpcPoint",&tpc_array);//Branch name
 Int_t nEntries =  t->GetEntriesFast(); // Number of events in the tree

  TCanvas* c1 = new TCanvas("Simple plot1", "Simulation", 1500, 1000);
  c1->SetMargin(0.08,0.1,0.08,0.07);
  TCanvas* c2 = new TCanvas("Simple plot2", "Simulation", 1500, 1000);
  c2->SetMargin(0.08,0.1,0.08,0.07);

  TH1F* hpdg=new TH1F("hpdg","Pdg Code", 50,-50.,0);
  hpdg->GetXaxis()->SetTitle("Pdg-Code");
  hpdg->GetYaxis()->SetTitle("Entries (a.u.)");
  hpdg->GetXaxis()->CenterTitle();
  hpdg->GetYaxis()->CenterTitle();

  TH1F* heloss=new TH1F("heloss","Energy Loss (KeV)", 100,0,1);
  heloss->GetXaxis()->SetTitle("dE/dX (KeV)");
  heloss->GetYaxis()->SetTitle("Entries (a.u.)");
  heloss->GetXaxis()->CenterTitle();
  heloss->GetYaxis()->CenterTitle();
 
  CeeTpcPoint* tpcpoint;
  Double_t energyloss, pdgcode;
  

  for (Int_t j=0; j< nEntries; j++)
	{
		t->GetEntry(j);
		//--------------------Start detector Entries--------------------
		for (Int_t k=0; k<tpc_array->GetEntriesFast(); k++) // tpc_array->GetEntriesFast()
		{ 
			tpcpoint = (CeeTpcPoint*)tpc_array->At(k);
                        if (tpcpoint->GetMotherId()<0) continue;
                        pdgcode = tpcpoint->GetPdgCode();
			energyloss = tpcpoint->GetEnergyLoss(); 
			energyloss = (energyloss*1000000); // Energy loss in KeV
                        if (pdgcode==-12){
                        hpdg->Fill(pdgcode);
                        heloss->Fill(energyloss);
                       }
			
		}
	      	             
       }
  c1->cd();
  hpdg->Draw("hist-text");
  c1->SaveAs("~/Desktop/Main_Plots/TPCPoint_Pdgcode_Sec_neg12.pdf");
 
  c2->cd();
  heloss->Draw("hist-text");
  c2->SaveAs("~/Desktop/Main_Plots/TPCPoint_Energyloss_Sec_neg12.pdf");

}
