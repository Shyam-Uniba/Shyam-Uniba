void eventDisplay()
{//
//  gROOT->LoadMacro("$VMCWORKDIR/gconfig/rootlogon.C");

    //-----User Settings:-----------------------------------------------
    TString  InputFile     ="../../../macro/test.root";
    TString  ParFile       ="../../../macro/params.root";
    TString  HitFile      ="../../../macro/hit_tpc.root";
//    TString  InputFile     ="test_CeeUrqmd2.root";
  //  TString  ParFile       ="params_CeeUrqmd2.root";
    // TString  InputFile     ="test_CeePluto.root";
    //TString  ParFile       ="params_CeePluto.root";
    TString  OutFile	 ="tst.root";

   Bool_t enablePointDraw = kTRUE;
   Bool_t enableHitDraw = kTRUE;



    // -----   Reconstruction run   -------------------------------------------
    FairRunAna *fRun= new FairRunAna();
    fRun->SetInputFile(InputFile.Data());
    fRun->SetOutputFile("tst.root");



   TFile* testFile;
   testFile = new TFile(HitFile.Data());
   if (!testFile->IsZombie()){
   fRun->AddFriend(HitFile.Data());
   }
   else{
	  enableHitDraw = kFALSE;
   }
   testFile->Close();

    FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
    FairParRootFileIo* parInput1 = new FairParRootFileIo();
    parInput1->open(ParFile.Data());
    rtdb->setFirstInput(parInput1);

//  parInput1->GetObjectInfo("FairGeoParSet");
    parInput1->Print();

    FairEventManager *fMan= new FairEventManager();

  if (enablePointDraw) {
    //----------------------Tracks and points -------------------------------------
    FairMCTracks    *Track     = new FairMCTracks("Monte-Carlo Tracks");
    FairMCPointDraw *TPCPoint  = new FairMCPointDraw("CeeTpcPoint", kGreen, kFullSquare);

    fMan->AddTask(Track);
    fMan->AddTask(TPCPoint);
}
  //--------------- Hits ----------------------

  if (enableHitDraw) {
	  FairHitPointSetDraw *TPCHits = new FairHitPointSetDraw("CeeTpcHit",kMagenta,kFullSquare);
	  fMan->AddTask(TPCHits);
  }
    //virtual void Init( Int_t visopt = 1, Int_t vislvl = 3, Int_t maxvisnds = 10000);
    fMan->Init();
    TEveElementList* cont = new TEveElementList("Text Setting");
    gEve->AddElement(cont);
    TEvePointSet *marker = new TEvePointSet(1);
    marker->SetName("Origin marker");
    marker->SetMarkerColor(6);
    marker->SetMarkerStyle(3);
    Float_t a = 0.0001;
    marker->SetPoint(0, a,  +a, +a);
    marker->RefMainTrans().SetPos(0.,250.,300.);

    TEveText *t[2];

    t[0] = new TEveText("CEE Eventdisplay");
    t[0]->PtrMainTrans()->RotateLF(1, 3, TMath::PiOver2());
    t[0]->SetMainColor(kMagenta);
    t[0]->SetFontSize(35);
    t[0]->SetFontMode(TGLFont::kExtrude);
    t[0]->SetLighting(kTRUE);
    t[0]->RefMainTrans().SetPos(0.,250.,300.);

    t[1] = new TEveText("(Shyam Kumar)");
    t[1]->PtrMainTrans()->RotateLF(1, 3, TMath::PiOver2());
    t[1]->SetMainColor(kMagenta);
    t[1]->SetFontSize(20);
    t[1]->SetFontMode(TGLFont::kExtrude);
    t[1]->SetLighting(kTRUE);
    t[1]->RefMainTrans().SetPos(0.,220,350.);

    cont->AddElement(marker);
    cont->AddElement(t[0]);
    cont->AddElement(t[1]);
    gEve->Redraw3D();


}


//FairRootManager* fManager = FairRootManager::Instance();
//FairFileSource *fFileSource = new FairFileSource(InputFile);
//fManager->SetSource(fFileSource);
//fManager->Register("GeoTracks","FairMCTrack","ceesim",kTRUE)
//TClonesArray *tracklist = static_cast<TClonesArray*>(fManager->GetObject("GeoTracks"));
