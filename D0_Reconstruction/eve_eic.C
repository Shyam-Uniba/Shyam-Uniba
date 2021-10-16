// Macro for event display; 
// shyam.kumar@ba.infn.it, shyam.kumar@cern.ch

void SetViewers(TEveViewer *RPhi, TEveViewer *RhoZ, TEveScene *fRPhiScene, TEveScene *fRhoZScene);
void eve_eic(Int_t startEv, Int_t endEv)
{
	
   Bool_t enable_rectrack_draw = kFALSE;
   Bool_t enable_hit_draw = kTRUE;
   Bool_t drawname = kFALSE;

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
  Int_t ntracks = 100; //Tracks->GetEntriesFast();

   TString rootfile="simple_geom.gdml"; // Eic Tracker
   TEveManager::Create();
   gGeoManager = TGeoManager::Import(rootfile); // or use TGeoManager::Import(rootfile)
  if (gGeoManager == nullptr)
        return;
    TEveGeoTopNode *TNod = new TEveGeoTopNode(gGeoManager,gGeoManager->GetTopNode());
    TGeoBBox *box = dynamic_cast<TGeoBBox *>(gGeoManager->GetTopNode()->GetVolume()->GetShape());

    gEve->AddGlobalElement(TNod);
    gEve->FullRedraw3D(kTRUE);
 
  // gEve->AddEvent(new TEveEventManager("Event", "ALICE VSD Event"));

    TEveProjectionManager *fRPhiProjManager = new TEveProjectionManager(TEveProjection::kPT_RPhi);
    TEveProjectionManager *fRhoZProjManager = new TEveProjectionManager(TEveProjection::kPT_RhoZ);
    gEve->AddToListTree(fRPhiProjManager, kFALSE);
    gEve->AddToListTree(fRhoZProjManager, kFALSE);
    TEveProjectionAxes *fAxesPhi = new TEveProjectionAxes(fRPhiProjManager);
    TEveProjectionAxes *fAxesRho = new TEveProjectionAxes(fRhoZProjManager);

    TEveWindowSlot *RPhiSlot = TEveWindow::CreateWindowInTab(gEve->GetBrowser()->GetTabRight());
    TEveWindowPack *RPhiPack = RPhiSlot->MakePack();
    RPhiPack->SetElementName("RPhi View");
    RPhiPack->SetShowTitleBar(kFALSE);
    RPhiPack->NewSlot()->MakeCurrent();
    TEveViewer *fRPhiView = gEve->SpawnNewViewer("RPhi View", "");
    TEveScene *fRPhiScene = gEve->SpawnNewScene("RPhi", "Scene holding axis.");
    fRPhiScene->AddElement(fAxesPhi);

    TEveWindowSlot *RhoZSlot = TEveWindow::CreateWindowInTab(gEve->GetBrowser()->GetTabRight());
    TEveWindowPack *RhoZPack = RhoZSlot->MakePack();
    RhoZPack->SetElementName("RhoZ View");
    RhoZPack->SetShowTitleBar(kFALSE);
    RhoZPack->NewSlot()->MakeCurrent();
    TEveViewer *fRhoZView = gEve->SpawnNewViewer("RhoZ View", "");
    TEveScene *fRhoZScene = gEve->SpawnNewScene("RhoZ", "Scene holding axis.");
    fRhoZScene->AddElement(fAxesRho);
    if (drawname){
    TEveText *t = new TEveText("Shyam Kumar");
    t->PtrMainTrans()->RotateLF(1, 3, TMath::PiOver2());
    t->SetElementName("Shyam");
    t->SetMainColor(kCyan);
    t->SetFontSize(40);
    t->SetFontMode(TGLFont::kExtrude);
    t->SetLighting(kTRUE);
    t->RefMainTrans().SetPos(0.,500,500.);
    gEve->AddElement(t);
   }
    SetViewers(fRPhiView, fRhoZView,fRPhiScene,fRhoZScene);

   // Draw Reconstruced Tracks
   TEveTrackList *list = new TEveTrackList();
   list->SetElementName("MCTracks");
   
   Int_t propagator = 1; // 0 default, 1 RungeKutta, 2 kHelix
   TEveTrackPropagator* g_prop = 0;
   TEveTrackPropagator *prop = g_prop = list->GetPropagator(); //default propogator

   if (propagator==1)
   {
      prop->SetStepper(TEveTrackPropagator::kRungeKutta);
      list->SetName("RK Propagator");
   }
   else if (propagator==2)
   {
   	  prop->SetStepper(TEveTrackPropagator::kHelix);
      list->SetName("Heix Propagator");
   }

   prop->SetFitDaughters(kFALSE);
   prop->SetMaxZ(400);
   prop->SetMaxR(400);
   prop->SetMagField(0.,0.,3.0); // 3.0 Tesla in Z-direction
   prop->SetMaxStep(25.0);

   gEve->GetBrowser()->GetTabRight()->SetTab(0); // 0 default viewer, 1 R-Phi and 2 R-Z

  TEveTrack  *track; TEveRecTrackD *rc;  TEvePointSet *hits;
//-----------------Main part of code-----------------------------------
  for (Int_t trackno=startEv; trackno<endEv; trackno++)// Reading the entries in tree
  {
  Tracks->GetEntry(trackno);	
  rc = new TEveRecTrackD(); // Reconstructed Track
  rc->fV.Set(0, 0, 0);
  rc->fP.Set(px, py,pz);
  rc->fSign = 0.;

  Double_t prec = sqrt(px*px+py*py+pz*pz);
  Double_t etarec = -1.0*TMath::Log(TMath::Tan((TMath::ACos(pz/prec))/2));
//  if (etarec<1.1 || etarec>1.2) continue;
  track = new TEveTrack(rc, prop);
 // track->SetRnrPoints(kTRUE);
  track->SetMarkerStyle(4);
  track->SetLineWidth(2);
  track->SetName(Form("Track_No%d",trackno));
  track->SetElementTitle(Form("TrackNo.=%d,"
             "P  = (%.3f, %.3f, %.3f)\n"
             ,trackno, px,py,pz));
  list->AddElement(track);
  track->MakeTrack();
  Int_t nhits = hittrackId->size();
  hits = new TEvePointSet(nhits);
  for (Int_t hitno=0; hitno<nhits; hitno++)// Reading the entries in tree
  hits->SetPoint(hitno,x0->at(hitno),y0->at(hitno),z0->at(hitno));
  hits->SetName(Form("Hits_Track%d",trackno));
  hits->SetMarkerColor(kGreen);
  hits->SetMarkerStyle(47);
  hits->SetMarkerSize(1.4);
  gEve->AddElement(hits);
  } 
  gEve->AddElement(list); 
  list->SetLineColor(kMagenta);
  TEveViewer *ev = gEve->GetDefaultViewer();
  TGLViewer  *gv = ev->GetGLViewer();
  gv->GetClipSet()->SetClipType(TGLClip::kClipPlane); // Other options are kClipNone = 0, kClipPlane, kClipBox  
 // gv->SetGuideState(TGLUtil::kAxesOrigin, kTRUE, kFALSE, 0);
  gEve->Redraw3D(kTRUE);



}

void SetViewers(TEveViewer *RPhi, TEveViewer *RhoZ, TEveScene *fRPhiScene, TEveScene *fRhoZScene)
{
	
    RPhi->GetGLViewer()->SetCurrentCamera(TGLViewer::kCameraOrthoXOY);
    // set clip plane and camera parameters
    // RPhi->GetGLViewer()->GetClipSet()->SetClipType(TGLClip::kClipPlane);
    // RPhi->GetGLViewer()->GetClipSet()->SetClipState(TGLClip::kClipPlane, fRPhiPlane);
    RPhi->GetGLViewer()->GetCameraOverlay()->SetOrthographicMode(TGLCameraOverlay::kAxis);
    RPhi->GetGLViewer()->GetCameraOverlay()->SetShowOrthographic(kTRUE);
    // switch off left, right, top and bottom light sources
    RPhi->GetGLViewer()->GetLightSet()->SetLight(TGLLightSet::kLightLeft, false);
    RPhi->GetGLViewer()->GetLightSet()->SetLight(TGLLightSet::kLightRight, false);
    RPhi->GetGLViewer()->GetLightSet()->SetLight(TGLLightSet::kLightTop, false);
    RPhi->GetGLViewer()->GetLightSet()->SetLight(TGLLightSet::kLightBottom, false);

    RhoZ->GetGLViewer()->SetCurrentCamera(TGLViewer::kCameraOrthoZOY);
    // set clip plane and camera parameters
    // RhoZ->GetGLViewer()->GetClipSet()->SetClipType(TGLClip::kClipPlane);
    // RhoZ->GetGLViewer()->GetClipSet()->SetClipState(TGLClip::kClipPlane, fRhoZPlane);
    RhoZ->GetGLViewer()->GetCameraOverlay()->SetOrthographicMode(TGLCameraOverlay::kAxis);
    RhoZ->GetGLViewer()->GetCameraOverlay()->SetShowOrthographic(kTRUE);
    // switch off left, right and front light sources
    RhoZ->GetGLViewer()->GetLightSet()->SetLight(TGLLightSet::kLightLeft, false);
    RhoZ->GetGLViewer()->GetLightSet()->SetLight(TGLLightSet::kLightRight, false);
    RhoZ->GetGLViewer()->GetLightSet()->SetLight(TGLLightSet::kLightFront, false);

    RPhi->AddScene(fRPhiScene);
    RPhi->AddScene(gEve->GetGlobalScene());   
    RPhi->AddScene(gEve->GetEventScene());
    RhoZ->AddScene(fRhoZScene);
    RhoZ->AddScene(gEve->GetGlobalScene());
    RhoZ->AddScene(gEve->GetEventScene());

}
