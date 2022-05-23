// Macro for event display; 
// shyam.kumar@ba.infn.it, shyam.kumar@cern.ch

void SetViewers(TEveViewer *RPhi, TEveViewer *RhoZ, TEveScene *fRPhiScene, TEveScene *fRhoZScene);
void Event_display_example(Double_t eta=0.0, Double_t mom=1.0)
{
	
   Bool_t enable_rectrack_draw = kFALSE;
   Bool_t enable_hit_draw = kTRUE;
   Bool_t drawname = kFALSE;

   TString rootfile="simple_geom.gdml"; // Eic Tracker
   TEveManager::Create();
   gGeoManager = TGeoManager::Import(rootfile); // or use TGeoManager::Import(rootfile)
    if (gGeoManager == nullptr) return;

   TObjArray* allvolumes = gGeoManager->GetListOfVolumes();
    for(Int_t i=0; i<allvolumes->GetEntries();i++){
          TGeoVolume* vol= (TGeoVolume*)allvolumes->At(i);
          TString volname = vol->GetIconName();
          if (volname.Contains("SVTX")) vol->SetLineColor(kRed); 	
          else if (volname.Contains("BARR")) vol->SetLineColor(kBlue); 
          vol->SetTransparency(20); // set the transparency level for the volumes
     }

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
   prop->SetMaxZ(500);
   prop->SetMaxR(500);
   prop->SetMagField(0.,0.,3.0); // 3.0 Tesla in Z-direction
   prop->SetMaxStep(25.0);

   gEve->GetBrowser()->GetTabRight()->SetTab(0); // 0 default viewer, 1 R-Phi and 2 R-Z

  TEveTrack  *track; TEveRecTrackD *rc; TEvePointSet *hits;

  Double_t theta = 2.0*TMath::ATan(TMath::Exp(-1.0*eta));
  Double_t sin_theta = fabs(TMath::Sin(theta));
  Double_t pz = mom*TMath::Cos(theta);
  Double_t pT = sqrt(mom*mom-pz*pz);
 
//-----------------Main part of code-----------------------------------
  for (Int_t trackno=0; trackno<500; trackno++)// Reading the entries in tree
  {
  Double_t phi = gRandom->Uniform(0.,2.0*TMath::Pi());	
  Double_t px = pT*TMath::Cos(phi);
  Double_t py = pT*TMath::Sin(phi);
  rc = new TEveRecTrackD(); // Reconstructed Track
  rc->fV.Set(0, 0, 0);
  rc->fP.Set(px, py,pz);
  rc->fSign = 1.;
  track = new TEveTrack(rc, prop);
 // track->SetRnrPoints(kTRUE);
  track->SetMarkerStyle(4);
  track->SetLineColor(int(gRandom->Uniform(2,10)));
  track->SetLineWidth(2);
  track->SetName(Form("Track_No%d",trackno));
  track->SetElementTitle(Form("TrackNo.=%d,"
             "P  = (%.3f, %.3f, %.3f)\n" "P  = %.3f GeV/c \n" "Eta =%.3f"
             ,trackno, px,py,pz, sqrt(px*px+py*py+pz*pz),eta));
  list->AddElement(track);
  track->MakeTrack();

  /* TEveVectorD vect1, vect2, vect3, vect4, vect5, vect6, vect7;
   prop->IntersectPlane( TEveVectorD(px,py,pz), TEveVectorD(5.,5.,30.), TEveVectorD(0.,0.,1.),vect1);	
   prop->IntersectPlane( TEveVectorD(px,py,pz), TEveVectorD(5.,5.,40.), TEveVectorD(0.,0.,1.),vect2);	
   prop->IntersectPlane( TEveVectorD(px,py,pz), TEveVectorD(5.,5.,50.), TEveVectorD(0.,0.,1.),vect3);	
   prop->IntersectPlane( TEveVectorD(px,py,pz), TEveVectorD(5.,5.,-30.), TEveVectorD(0.,0.,1.),vect4);	
   prop->IntersectPlane( TEveVectorD(px,py,pz), TEveVectorD(5.,5.,-40.), TEveVectorD(0.,0.,1.),vect5);	
   prop->IntersectPlane( TEveVectorD(px,py,pz), TEveVectorD(5.,5.,-50.), TEveVectorD(0.,0.,1.),vect6);	
  // prop->IntersectPlane( TEveVectorD(px,py,pz), TEveVectorD(sqrt(3.0/2),sqrt(3.0/2), 0.), TEveVectorD(1.,1.,0.),vect7);	
 // cout<<"Intersection: "<<vect.fX<<vect.fY<<vect.fZ<<endl;
  hits = new TEvePointSet(7);
  hits->SetPoint(0,vect1.fX,vect1.fY,vect1.fZ);
  hits->SetPoint(1,vect2.fX,vect2.fY,vect2.fZ);
  hits->SetPoint(2,vect3.fX,vect3.fY,vect3.fZ);
  hits->SetPoint(3,vect4.fX,vect4.fY,vect4.fZ);
  hits->SetPoint(4,vect5.fX,vect5.fY,vect5.fZ);
  hits->SetPoint(5,vect6.fX,vect6.fY,vect6.fZ);
 // hits->SetPoint(6,vect7.fX,vect7.fY,vect7.fZ);
  hits->SetName("Hits_Track");
  hits->SetMarkerColor(kGreen);
  hits->SetMarkerStyle(47);
  hits->SetMarkerSize(1.4);
  gEve->AddElement(hits);*/
  }
  gEve->AddElement(list); 
 // list->SetLineColor(int(gRandom->Uniform(2,10)));
  TGLViewer  *viewer = gEve->GetDefaultGLViewer();
  viewer->GetClipSet()->SetClipType(TGLClip::kClipPlane); // Other options are kClipNone = 0, kClipPlane, kClipBox  
  Double_t center[3] ={0.,0.,0.};
  Double_t *cent; cent = center;
  viewer->SetOrthoCamera(TGLViewer::kCameraOrthoZOX,2.0, 1.0, cent,1.0,0.); // kCameraPerspXOZ, kCameraPerspYOZ, kCameraPerspXOY, kCameraOrthoXOY,
 // viewer->SetGuideState(TGLUtil::kAxesOrigin, kTRUE, kFALSE, 0);
  viewer->RefreshPadEditor(viewer);	
  viewer->CurrentCamera().RotateRad(-0.5, 0.5);
  viewer->DoDraw();
  gEve->Redraw3D();

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
