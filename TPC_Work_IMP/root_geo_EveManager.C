// Macro to draw the geometry of CEE experiment from parameter file 
// This will allow to set the transparency level for the volumes
void SetDelay(Int_t time);
void root_geo_EveManager()
{
 
  TEveManager::Create();
  
  TFile* file = new TFile("params.root");
   
    file->Get("FairBaseParSet");
    TGeoManager *gGeoMan = gGeoManager;
    if(!gGeoMan) {
      file->Get("FairGeoParSet");
      gGeoMan = gGeoManager;
      if(!gGeoMan) {
        std::cout<<"GeoManager not found"<<std::endl;
        return;
      }
   
  }
   TObjArray* allvolumes = gGeoMan->GetListOfVolumes();
   for(Int_t i=0; i<allvolumes->GetEntries();i++){
          TGeoVolume* vol= (TGeoVolume*)allvolumes->At(i);
          vol->SetTransparency(5); // set the transparency level for the volumes
  }
  
   TEveGeoTopNode* topnode = new TEveGeoTopNode(gGeoMan, gGeoMan->GetTopNode());
   topnode->SetVisLevel(3);
   gEve->AddGlobalElement(topnode);
   gEve->FullRedraw3D(kTRUE);
  // GLviewer setting you can choose different setting
   // 0 - no clip, 1 - clip plane, 2 - clip box
 
   TGLViewer *viewer = gEve->GetDefaultGLViewer();
   ((TGMainFrame *)((TGLSAViewer *)viewer)->GetFrame())->Resize(4000, 990);
   viewer->GetClipSet()->SetClipType(TGLClip::kClipNone); // Other options are kClipNone = 0, kClipPlane, kClipBox    
   viewer->ColorSet().Background().SetColor(kBlack);
   viewer->SetGuideState(TGLUtil::kAxesOrigin, kTRUE, kFALSE, 0); // EAxesType { kAxesNone, kAxesEdge, kAxesOrigin }
   viewer->UseDarkColorSet(); 
   viewer->RefreshPadEditor(viewer);	
   viewer->CurrentCamera().RotateRad(-0.5, 0.5);
   viewer->DoDraw();
   Double_t center[3] ={0.,0.,0.};
   Double_t *cent; cent = center;
   viewer->SetOrthoCamera(TGLViewer::kCameraOrthoZOX,1.0, 1.0, cent,1.0,0.); // kCameraPerspXOZ, kCameraPerspYOZ, kCameraPerspXOY, kCameraOrthoXOY,
   //kCameraOrthoXOZ, kCameraOrthoZOY, kCameraOrthoXnOY, kCameraOrthoXnOZ,kCameraOrthoZnOY
   
   TGLAutoRotator * autorot = new TGLAutoRotator(viewer);
   autorot->Start();
   autorot->RotateScene();
 //  autorot->Stop();
  // TStopwatch time; // Time to estimate the time for the macro
  // time.Start();
  // Double_t starttime = time.CpuTime();


 // autorot->StartImageAutoSaveAnimatedGif("anim.gif+"); 
// SetDelay(10000); 
  //if (starttime==1000000)autorot->Stop();
}


