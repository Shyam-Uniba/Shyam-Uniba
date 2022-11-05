// Macro for event display; 
// shyam.kumar@ba.infn.it, shyam.kumar@cern.ch

void Event_display_example()
{

   Bool_t my_colors = 1;

   TString rootfile="epic_tracking_only.root"; // Eic Tracker
   TEveManager::Create();
   gGeoManager = TGeoManager::Import(rootfile); // or use TGeoManager::Import(rootfile)
    if (gGeoManager == nullptr) return;
    
    TObjArray* allvolumes = gGeoManager->GetListOfVolumes();   
      // Draw vertex layers only
    for(Int_t i=0; i<allvolumes->GetEntries();i++){
          TGeoVolume* vol= (TGeoVolume*)allvolumes->At(i);
          TString volname = vol->GetIconName();
          if (my_colors){
         // if (i>69 && i<79) vol->SetLineColor(kBlue); // color of vertex layers
          if (i>78 && i<91) vol->SetLineColor(kMagenta); // color of Sagitta layers
          if (i>122 && i<179) vol->SetLineColor(kGreen); 	// Silicon disk layers
          vol->SetTransparency(20);
          if (i>222) {vol->SetLineColor(kWhite); vol->SetTransparency(0); }// Beam Pipe
          // set the transparency level for the volumes
          cout<<"No: "<<i<<" Vol Name: "<<volname<<endl;
          }
     }
     
 
    TEveGeoTopNode *TNod = new TEveGeoTopNode(gGeoManager,gGeoManager->GetTopNode());
    TGeoBBox *box = dynamic_cast<TGeoBBox *>(gGeoManager->GetTopNode()->GetVolume()->GetShape());
    gEve->AddGlobalElement(TNod);    
    
    gEve->FullRedraw3D(kTRUE);
    gEve->GetDefaultGLViewer()->ResetCurrentCamera();
    gEve->GetDefaultGLViewer()->RequestDraw(TGLRnrCtx::kLODHigh);
 

  TGLViewer  *viewer = gEve->GetDefaultGLViewer();
  viewer->GetClipSet()->SetClipType(TGLClip::kClipPlane); // Other options are kClipNone = 0, kClipPlane, kClipBox  
  Double_t center[3] ={0.,30.,0.};
  Double_t *cent; cent = center;
  viewer->SetOrthoCamera(TGLViewer::kCameraOrthoZOX,2.0, 1.0, cent,1.0,0.); // kCameraPerspXOZ, kCameraPerspYOZ, kCameraPerspXOY, kCameraOrthoXOY,
 // viewer->SetGuideState(TGLUtil::kAxesOrigin, kTRUE, kFALSE, 0);
  viewer->RefreshPadEditor(viewer);
  viewer->CurrentCamera().Configure(15., 0, center, -1.0, 0.5);	
  //viewer->CurrentCamera().RotateRad(-1.0, 0.5);
  viewer->DoDraw();
  viewer->RequestDraw(TGLRnrCtx::kLODHigh);
  viewer->SavePicture("epic_geo.pdf");
  gEve->Redraw3D();
  }
