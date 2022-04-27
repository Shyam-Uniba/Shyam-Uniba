void PlotPad_dEdX(){

   gStyle->SetPalette(1);
   gStyle->SetOptTitle(1);
   gStyle->SetTitleOffset(.85,"X");gStyle->SetTitleOffset(.85,"Y");
   gStyle->SetTitleSize(.04,"XY");gStyle->SetTitleSize(.04,"t");
   gStyle->SetLabelSize(.04,"XY");
   gStyle->SetHistLineWidth(2);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0); 

 TFile* file = new TFile("../../../macro/test.root");
 TTree *t=(TTree *) file->Get("ceesim") ;
 TClonesArray *tpc_array=new TClonesArray("CeeTpcPoint");
 t->SetBranchAddress("CeeTpcPoint",&tpc_array);//Branch name
 Int_t nEntries =  t->GetEntriesFast(); // Number of events in the tree

  TCanvas* c1 = new TCanvas("Simple plot1", "Simulation", 1500, 1000);
  c1->SetMargin(0.08,0.1,0.08,0.07);

  TH2F* ma=new TH2F("map","Hit Pads [dEdX (KeV) ]",75,-45.,45.,240,-60.,60.);
  ma->GetXaxis()->SetTitle("Z [cm]");
  ma->GetYaxis()->SetTitle("X [cm]");
  ma->GetXaxis()->CenterTitle();
  ma->GetYaxis()->CenterTitle();
 
  CeeTpcPoint* tpcpoint;
  Double_t energyloss, x, z;
  

  for (Int_t j=0; j< nEntries; j++)
	{
		t->GetEntry(j);
		//--------------------Start detector Entries--------------------
		for (Int_t k=0; k<tpc_array->GetEntriesFast(); k++) // tpc_array->GetEntriesFast()
		{ 
			tpcpoint = (CeeTpcPoint*)tpc_array->At(k);
                        if (tpcpoint->GetMotherId()>=0) continue;
			energyloss = tpcpoint->GetEnergyLoss(); 
                            x = tpcpoint->GetX();
                            z = tpcpoint->GetZ();
			energyloss = (energyloss*1000000); // Energy loss in KeV
                        ma->Fill(z+1.2/2,x+0.5/2,energyloss);
			
		}
	      	             
       }

 
  c1->cd();
  ma->Draw("colz");
  c1->SaveAs("Pad_Plane_dedx.pdf");

}
