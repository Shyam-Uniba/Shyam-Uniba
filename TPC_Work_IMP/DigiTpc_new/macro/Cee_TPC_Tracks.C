void Cee_TPC_Tracks()
{

   auto c = new TCanvas("c","c",1500,1000);
   gStyle->SetOptStat(kFALSE);
   gStyle->SetTitleOffset(1.7,"XY");	

 TFile* file = new TFile("../../../macro/test.root");
 TTree *t=(TTree *) file->Get("ceesim") ;
 TClonesArray *tpc_array=new TClonesArray("CeeTpcPoint");
 t->SetBranchAddress("CeeTpcPoint",&tpc_array);//Branch name
 Int_t nEntries =  t->GetEntriesFast(); // Number of events in the tree
 CeeTpcPoint* tpcpoint;
 Double_t x[10000], y[10000], z[10000];
 std::vector<float> a,b,d;
 Int_t mcentries = 0;
 TPolyLine3D *pl;
 int pdgcode;
 TPaveText* pt[5];

   TH3F *h3box = new TH3F("h3box","",75, -45.,45., 240, -60., 60., 80, -40., 40.);
   TH3F *h3box1 = new TH3F("h3box1","",1, -45.,45., 1, -60., 60., 80, -40., 40.);
   TH3F *h3box2 = new TH3F("h3box2","",75, -45.,45., 120, -60., 60., 80, -40., 40.);
   int count=0;

   h3box->SetFillColor(kGreen);
   h3box1->SetFillColor(kBlue);
   h3box->GetXaxis()->SetTitle("Z-direction [cm]");
   h3box->GetYaxis()->SetTitle("X-direction [cm]");
   h3box->GetZaxis()->SetTitle("Y-direction [cm]");
   h3box->GetXaxis()->CenterTitle();
   h3box->GetYaxis()->CenterTitle();
   h3box->GetZaxis()->CenterTitle();
   h3box2->SetMarkerColor(kMagenta);
   h3box2->SetMarkerSize(0.5);
   h3box2->SetMarkerStyle(20);
   h3box2->SetFillColor(kBlack);
  // h3box->GetXaxis()->SetRangeUser(-5.0,5.0);
  // h3box->GetYaxis()->SetRangeUser(-5.0,5.0);
  // t->Draw("CeeTpcPoint.fY:CeeTpcPoint.fX:CeeTpcPoint.fZ>> h3box2","","");
   h3box->SetFillStyle(3001);
   h3box1->SetFillStyle(3001);
   h3box1->Fill(0.5, 0.5,-39.9,  1.);
   c->cd();

   h3box->Draw("");
   h3box1->Draw("box1-same");
 std::map<unsigned int,std::vector<CeeTpcPoint*> > map_point;
  for (Int_t j=0; j< nEntries; j++)
	{
		t->GetEntry(j);
                mcentries = tpc_array->GetEntriesFast();
		//--------------------Start detector Entries--------------------
		for (Int_t k=0; k<mcentries; k++) // tpc_array->GetEntriesFast()
		{ 
			tpcpoint = (CeeTpcPoint*)tpc_array->At(k);
			if (tpcpoint->GetMotherId()>=0) continue;
			unsigned int trackid=tpcpoint->GetTrackID();
			map_point[trackid].push_back(tpcpoint);
                        			
		}

   std::map<unsigned int,std::vector<CeeTpcPoint*> >::iterator point_It;
   for (point_It = map_point.begin(); point_It!=map_point.end(); point_It++){
   for( auto point_iter = point_It->second.begin() ; point_iter != point_It->second.end() ; ++point_iter ){
		  a.push_back((*point_iter)->GetX());
		  b.push_back((*point_iter)->GetY());
		  d.push_back((*point_iter)->GetZ());
		  pdgcode = (*point_iter)->GetPdgCode();
	   }
    const int n = a.size();
    for (int i =0; i<n; i++){
    	x[i] = a[i];
    	y[i] = b[i];
    	z[i] = d[i];
    }
       pl = new TPolyLine3D(n,z,x,y,"L");
       if(fabs(pdgcode)==2212) pl->SetLineColor(kBlue);
       if(fabs(pdgcode)==211) pl->SetLineColor(kRed);
       if(fabs(pdgcode)==321) pl->SetLineColor(kGreen);
       if(fabs(pdgcode)==11) pl->SetLineColor(kMagenta);
       if(fabs(pdgcode)==13) pl->SetLineColor(kOrange+7);
       pl->SetLineStyle(8);
       pl->SetLineWidth(3);
       pl->Draw("L-same");
       a.clear();
       b.clear();
       d.clear();
   }
   map_point.clear();
	}
   float k=0.20;
   for(int m=0; m<5; m++){
   k=k+0.05;
   pt[m]=new TPaveText(0.90,k,1.0,k+0.10,"NDC");
   pt[m]->SetFillStyle(0);
   pt[m]->SetBorderSize(0);
   pt[m]->SetTextSize(0.03);
   pt[m]->SetTextFont(42);
   pt[m]->SetTextAlign(13);
   if (m==0){
   pt[m]->SetTextColor(kBlue);
   pt[m]->AddText(0.,0.,"Proton");
   }
   if (m==1){
   pt[m]->SetTextColor(kRed);
   pt[m]->AddText(0.,0.,"Pion");
   }
   if (m==2){
   pt[m]->SetTextColor(kGreen);
   pt[m]->AddText(0.,0.,"Kaon");
   }
   if (m==3){
   pt[m]->SetTextColor(kMagenta);
   pt[m]->AddText(0.,0.,"Electron");
   }
   if (m==4){
   pt[m]->SetTextColor(kOrange+7);
   pt[m]->AddText(0.,0.,"Muon");
   }
   pt[m]->Draw("same");
   }

   TPaveText* t1=new TPaveText(0.75,0.75,0.85,0.90,"NDC");
   t1->SetFillStyle(0);
   t1->SetBorderSize(0);
   t1->SetTextSize(0.04);
   t1->SetTextFont(42);
   t1->SetTextAlign(13);
   t1->SetTextColor(kRed);
   TText *text = t1->AddText(0.,0.,"TPC Pad-Plane"); 
   text->SetTextAngle(15);
   t1->Draw();

   t1=new TPaveText(0.40,0.25,0.85,0.35,"NDC");
   t1->SetFillStyle(0);
   t1->SetBorderSize(0);
   t1->SetTextSize(0.04);
   t1->SetTextFont(42);
   t1->SetTextAlign(13);
   t1->SetTextColor(kMagenta);
   text = t1->AddText(0.,0.,"High Voltage Plane");
   text->SetTextAngle(10); 
   t1->Draw();
   gPad->RedrawAxis();

 TString PadPlanefile ="PadCoordinate.txt"; // PadPlane
 CeeTpcPadPlane* PadPlane= new CeeTpcPadPlane(PadPlanefile);
  int npads=PadPlane->GetNPads(); // Total Pads
  for(int i=0;i<npads;++i){
    CeeTpcPad* apad=0;
    try{
      apad=PadPlane->GetPad(i);
    }
    catch (std::exception &e){
      cout << e.what() << endl;
      cout.flush();
      continue;
    }
    std::vector<Double_t> gPosX, gPosZ;
    gPosX = apad->GetGlobalXCoordinates();
    gPosZ = apad->GetGlobalZCoordinates();
    const int n = 5;
    for (int i =0; i<n; i++){
    	x[i] = gPosX[i]*0.1;
    	y[i] = 40.;
    	z[i] = gPosZ[i]*0.1;
    }
       pl = new TPolyLine3D(n,z,x,y,"L");
       pl->SetLineColor(kRed);
       pl->SetLineStyle(8);
       pl->SetLineWidth(1);
       pl->Draw("L-same");
/*       pl->SetLineStyle(8);
       pl->SetLineWidth(3);
       pl->Draw("L-same");*/
  }

   c->SaveAs("Cee_TPC.png");
}
