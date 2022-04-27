// Macro for displaying the tracks for the TPC simulation
// input file test.root contains the MC information
//Shyam Kumar


{
 
  TCanvas* c1 = new TCanvas("c1", "", 2000, 1200);
  c1->SetFillColor(kBlack);
  TFile* file = new TFile("../../../macro/params.root");
  file->Get("FairBaseParSet");
    TGeoManager *gGeoMan = gGeoManager;
    if(!gGeoMan) {
      file->Get("FairGeoParSet");
      gGeoMan = gGeoManager;
      if(!gGeoMan) {
        std::cout<<"GeoManager not found"<<std::endl;
        return;
      }
   
   gGeoMan->SetVisLevel(3);
   TObjArray* allvolumes = gGeoMan->GetListOfVolumes();
   for(Int_t i=0; i<allvolumes->GetEntries();i++){
          TGeoVolume* vol= (TGeoVolume*)allvolumes->At(i);
          vol->SetTransparency(5); // set the transparency level for the volumes
  }

    gGeoMan->GetTopVolume()->Draw("same");
  }
  

 TView *view = c1->GetView();
 view->SetRange(-318.8399,-187.5,50,318.8399,187.5,500);
 c1->SetTheta(7.696541);
 c1->SetPhi(1.657977);
 view->ZoomView(c1,0.7);
 // Pave text to write name of detector on the Pad
 TPaveText* t1=new TPaveText(0.80,0.90,0.99,0.99,"NDC");
 t1->SetFillStyle(0);
 t1->SetBorderSize(0);
 t1->SetTextSize(0.05);
 t1->SetTextFont(42);
 t1->SetTextAlign(13);
 t1->AddText(0.,0.,"#splitline{CEE Detector}{(Shyam Kumar)}"); 
 t1->SetTextColor(kWhite);
 t1->Draw();
 
 //Particle with color code
 double d=0.80;
 TPaveText* part[4];
 for (int p=0; p<4; p++){
 if (p==0) d=0.80;
 if (p==1) d=0.75; 
 if (p==2) d=0.70;
 if (p==3) d=0.65;
 part[p]=new TPaveText(0.01,d,0.3,d+0.10,"NDC");
 part[p]->SetFillStyle(0);
 part[p]->SetBorderSize(0);
 part[p]->SetTextSize(0.04);
 part[p]->SetTextFont(42);
 part[p]->SetTextAlign(13);
 if (p==0){
 part[p]->SetTextColor(kBlue);
 part[p]->AddText(0.,0.,"Proton");
 part[p]->Draw("");
 }
 if (p==1){
 part[p]->SetTextColor(kRed); 
 part[p]->AddText(0.,0.,"Pion");
 part[p]->Draw("");
 }
  if (p==2){
 part[p]->SetTextColor(kGreen); 
 part[p]->AddText(0.,0.,"Kaon");
 part[p]->Draw("");
 }
  if (p==3){
 part[p]->SetTextColor(kMagenta); 
 part[p]->AddText(0.,0.,"Electron");
 part[p]->Draw("");
 }
 }



 
  // Pave text to show the event on the detector 
 TPaveText* t2=new TPaveText(0.01,0.90,0.3,0.99,"NDC");
 t2->SetFillStyle(0);
 t2->SetBorderSize(0);
 t2->SetTextSize(0.04);
 t2->SetTextFont(42);
 t2->SetTextAlign(13);
 t2->SetTextColor(kWhite);
  
  //drawing tracks
  TFile* file1 = new TFile("../../../macro/test.root");
  TTree *t=(TTree *)file1->Get("ceesim");
   
  TClonesArray *fT=new TClonesArray("TGeoTrack");

  t->SetBranchAddress("GeoTracks",&fT) ;

  TGeoTrack *tr;
  
   for (Int_t j=0; j< t->GetEntriesFast(); j++)	{
    t->GetEntry(j);
    t2->AddText(0.,0.,Form("Event %d",j));
    t2->Draw("");
    for (Int_t i=0; i<fT->GetEntriesFast(); i++)	{
      tr=(TGeoTrack *)fT->At(i);
      cout<<tr->GetPDG()<<endl;
      if (tr->GetPDG()==2212)tr->SetLineColor(kBlue);
      if (tr->GetPDG()==211)tr->SetLineColor(kRed);
      if (tr->GetPDG()==311)tr->SetLineColor(kGreen);
      if (tr->GetPDG()==11)tr->SetLineColor(kMagenta);
      
      if (i==0) tr->Draw("/*"); // option to draw all tracks
      else tr->Draw("same");
      c1->Print("ceeanim.gif+10");

    }
    t2->Clear();
  }

}

