// Macro to draw the geometry of CEE experiment from parameter file 
// This will allow to set the transparency level for the volumes
void root_geo()
{
 
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
   
   gGeoMan->SetVisLevel(3);
   TObjArray* allvolumes = gGeoMan->GetListOfVolumes();
   for(Int_t i=0; i<allvolumes->GetEntries();i++){
          TGeoVolume* vol= (TGeoVolume*)allvolumes->At(i);
          vol->SetTransparency(5); // set the transparency level for the volumes
  }

   gGeoMan->GetTopVolume()->Draw("ogl");
  }
 

   TGLViewer *viewer = (TGLViewer *)gPad->GetViewer3D();
   ((TGMainFrame *)((TGLSAViewer *)viewer)->GetFrame())->Resize(2000, 990);
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
  // v->CurrentCamera().RotateRad(-.7, 0.5);
   TView3D *v = new TView3D();
   v->ZoomView(gPad, 2.0); 	
   
   TGLAutoRotator * autorot = new TGLAutoRotator(viewer);
   autorot->Start();
   autorot->RotateScene();
   //autorot->StartImageAutoSaveAnimatedGif("anim.gif+");   //To do animations
   //autorot->Stop();
  }




