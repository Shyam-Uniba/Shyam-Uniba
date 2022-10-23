// Macro for event display; 
// shyam.kumar@ba.infn.it, shyam.kumar@cern.ch

void SetViewers(TEveViewer *RPhi, TEveViewer *RhoZ, TEveScene *fRPhiScene, TEveScene *fRhoZScene);
void Event_display_example(Double_t EtaMin=1.0, Double_t EtaMax=1.1, Double_t mom=5.0)
{
	
	TEveManager::Create();
   Bool_t enable_rectrack_draw = kFALSE;
   Bool_t enable_hit_draw = kTRUE;
   Bool_t drawname = kTRUE;
   
   TEveElementList *Athena_Tracker = new TEveElementList("Geometry");
 
   TEveGeoShape *b;

	// Vertexing
  	double si_vtx_r_pos[] = {3.3,4.35,5.4}; 
	  const int nVtxLayers = sizeof(si_vtx_r_pos)/sizeof(*si_vtx_r_pos);
	  double si_z_vtxlength[] = {28, 28, 28};

	// Barrel
 	 double si_r_pos[] = {13.34, 17.96};
   const int nTrckLayers = sizeof(si_r_pos)/sizeof(*si_r_pos);
	  double si_z_length[] = {34.34, 46.68};

	  double si_thick = 0.55/100.*9.37;

   for (int i=0; i<nVtxLayers+nTrckLayers; i++){
   if (i<3){
   b = new TEveGeoShape(Form("Vtx_%d",i));
   b->SetShape(new TGeoTube(si_vtx_r_pos[i], si_vtx_r_pos[i]+si_thick, si_z_vtxlength[i]));
   b->SetMainColor(kRed);  	
   }	
   else{
   b = new TEveGeoShape(Form("Barr_%d",i-3));
   b->SetShape(new TGeoTube(si_r_pos[i-3], si_r_pos[i-3]+si_thick, si_z_length[i-3]));
   b->SetMainColor(kGreen); 	
   }
   b->SetMainTransparency(80);
   Athena_Tracker->AddElement(b);
  }
  

 
   gEve->AddGlobalElement(Athena_Tracker);

   // Text setting
   TEveText *text = new TEveText("!! Happy Diwali !!");
   text->PtrMainTrans()->RotateLF(1, 3, TMath::PiOver2());
   text->SetMainColor(kGreen);
   text->SetFontSize(60);
   text->SetFontMode(TGLFont::kPixmap);
   text->SetLighting(kTRUE);
   text->RefMainTrans().SetPos(50.,100.,100.);
   gEve->AddElement(text);
   
      // Text setting
   TEveText *text1 = new TEveText("Shyam");
   text1->PtrMainTrans()->RotateLF(1, 3, TMath::PiOver2());
   text1->SetMainColor(kGreen);
   text1->SetFontSize(60);
   text1->SetFontMode(TGLFont::kPixmap);
   text1->SetLighting(kTRUE);
   text1->RefMainTrans().SetPos(100.,70.,100.);
   gEve->AddElement(text1);
   
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
    SetViewers(fRPhiView, fRhoZView,fRPhiScene,fRhoZScene);

   // Draw Reconstruced Tracks
   TEveTrackList *list[4]; 
   
   for (int i =0; i<4; i++){
   list[i]= new TEveTrackList();
   list[i]->SetElementName("MCTracks");  
   }
   

   gEve->GetBrowser()->GetTabRight()->SetTab(0); // 0 default viewer, 1 R-Phi and 2 R-Z

  TEveTrack  *track; TEveRecTrackD *rc; TEvePointSet *hits;
 
//-----------------Main part of code-----------------------------------
  for (Int_t trackno=0; trackno<500; trackno++)// Reading the entries in tree
  {
  
   int charge = 1;
   if (trackno%2==0) charge = 1;
   else charge = -1;
   Int_t propagator = 0; // 0 default, 1 RungeKutta, 2 kHelix
   TEveTrackPropagator* g_prop = 0;
   TEveTrackPropagator *prop = g_prop = list[0]->GetPropagator(); //default propogator

   prop->SetFitDaughters(kFALSE);
   prop->SetMaxZ(100);
   prop->SetMaxR(100);
   prop->SetMagField(0.,0.,10.); // 3.0 Tesla in Z-direction
   prop->SetMaxStep(25.0);
  
  Double_t eta = gRandom->Uniform(EtaMin,EtaMax);
  Double_t theta = 2.0*TMath::ATan(TMath::Exp(-1.0*eta));
  Double_t sin_theta = fabs(TMath::Sin(theta));
  Double_t pz = mom*TMath::Cos(theta);
  Double_t pT = sqrt(mom*mom-pz*pz);
  Double_t phi = gRandom->Uniform(0.,2.0*TMath::Pi());	
  Double_t px = pT*TMath::Cos(phi);
  Double_t py = pT*TMath::Sin(phi);
  rc = new TEveRecTrackD(); // Reconstructed Track
  rc->fV.Set(0, 0, 0);
  rc->fP.Set(px, py,pz);
  rc->fSign = charge;
  track = new TEveTrack(rc, prop);
 // track->SetRnrPoints(kTRUE);
  track->SetMarkerStyle(4);
  track->SetLineColor(int(gRandom->Uniform(2,10)));
  track->SetLineWidth(2);
  track->SetName(Form("Track_No%d",trackno));
  track->SetElementTitle(Form("TrackNo.=%d,"
             "P  = (%.3f, %.3f, %.3f)\n" "P  = %.3f GeV/c \n" "Eta =%.3f"
             ,trackno, px,py,pz, sqrt(px*px+py*py+pz*pz),eta));
  list[0]->AddElement(track);
  track->MakeTrack();
  
  
  // Tracks 2
  
   TEveTrackPropagator* g_prop1 = 0;
   TEveTrackPropagator *prop1 = g_prop1 = list[1]->GetPropagator(); //default propogator

   prop1->SetFitDaughters(kFALSE);
   prop1->SetMaxZ(100);
   prop1->SetMaxR(600);
   prop1->SetMagField(0.,0.,10.); // 3.0 Tesla in Z-direction
   prop1->SetMaxStep(25.0);
   rc->fV.Set(300., 0, 0);
   rc->fP.Set(px, py,pz);
   rc->fSign = charge;
   track = new TEveTrack(rc, prop1);
 // track->SetRnrPoints(kTRUE);
  track->SetMarkerStyle(4);
  track->SetLineColor(int(gRandom->Uniform(2,10)));
  track->SetLineWidth(2);
  track->SetName(Form("Track_No%d",trackno));
  track->SetElementTitle(Form("TrackNo.=%d,"
             "P  = (%.3f, %.3f, %.3f)\n" "P  = %.3f GeV/c \n" "Eta =%.3f"
             ,trackno, px,py,pz, sqrt(px*px+py*py+pz*pz),eta));
  list[1]->AddElement(track);
  track->MakeTrack();
  
  
    // Tracks 3
  
   TEveTrackPropagator* g_prop2 = 0;
   TEveTrackPropagator *prop2 = g_prop2 = list[2]->GetPropagator(); //default propogator

   prop2->SetFitDaughters(kFALSE);
   prop2->SetMaxZ(100);
   prop2->SetMaxR(600);
   prop2->SetMagField(0.,0.,10.); // 3.0 Tesla in Z-direction
   prop2->SetMaxStep(25.0);
   rc->fV.Set(0., 200., 0);
   rc->fP.Set(px, py,pz);
   rc->fSign = charge;
   track = new TEveTrack(rc, prop2);
 // track->SetRnrPoints(kTRUE);
  track->SetMarkerStyle(4);
  track->SetLineColor(int(gRandom->Uniform(2,10)));
  track->SetLineWidth(2);
  track->SetName(Form("Track_No%d",trackno));
  track->SetElementTitle(Form("TrackNo.=%d,"
             "P  = (%.3f, %.3f, %.3f)\n" "P  = %.3f GeV/c \n" "Eta =%.3f"
             ,trackno, px,py,pz, sqrt(px*px+py*py+pz*pz),eta));
  list[2]->AddElement(track);
  track->MakeTrack();
  
      // Tracks 4
  
   TEveTrackPropagator* g_prop3 = 0;
   TEveTrackPropagator *prop3 = g_prop3 = list[3]->GetPropagator(); //default propogator

   prop3->SetFitDaughters(kFALSE);
   prop3->SetMaxZ(100);
   prop3->SetMaxR(600);
   prop3->SetMagField(0.,0.,10.); // 3.0 Tesla in Z-direction
   prop3->SetMaxStep(25.0);
   rc->fV.Set(300., 200., 0);
   rc->fP.Set(px, py,pz);
   rc->fSign = charge;
   track = new TEveTrack(rc, prop3);
 // track->SetRnrPoints(kTRUE);
  track->SetMarkerStyle(4);
  track->SetLineColor(int(gRandom->Uniform(2,10)));
  track->SetLineWidth(2);
  track->SetName(Form("Track_No%d",trackno));
  track->SetElementTitle(Form("TrackNo.=%d,"
             "P  = (%.3f, %.3f, %.3f)\n" "P  = %.3f GeV/c \n" "Eta =%.3f"
             ,trackno, px,py,pz, sqrt(px*px+py*py+pz*pz),eta));
  list[3]->AddElement(track);
  track->MakeTrack();
  
  }
  
  gEve->AddElement(list[0]); 
  gEve->AddElement(list[1]); 
  gEve->AddElement(list[2]);  
  gEve->AddElement(list[3]);  

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
