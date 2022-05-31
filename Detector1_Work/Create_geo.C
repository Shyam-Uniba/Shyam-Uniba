void Create_geo()
{
  
   gGeoManager = TGeoManager::Import("geometry.root"); // or use TGeoManager::Import(rootfile)
   if (gGeoManager == nullptr) return;
   //gGeoManager->SetVisLevel(2);

   TObjArray* volumes = gGeoManager->GetListOfVolumes();
  
    const Int_t nvolumes = volumes->GetEntries();
    TCanvas *c[nvolumes]; 
    
    for(Int_t i=0; i<5;++i)
    {
    
      TGeoVolume* vol= (TGeoVolume*) volumes->At(i);
      TString volname = vol->GetName();
      //   if (volname.Contains("SVTX")) vol->SetLineColor(kRed); 	
      //   else if (volname.Contains("BARR")) vol->SetLineColor(kBlue); 
      //  vol->SetTransparency(20); // set the transparency level for the volumes
       gGeoManager->GetVolume(i)->Draw("ogl");
       
      TGLViewer *viewer = (TGLViewer *)gPad->GetViewer3D();
     ((TGMainFrame *)((TGLSAViewer *)viewer)->GetFrame())->Resize(2000, 990);
     viewer->GetClipSet()->SetClipType(TGLClip::kClipPlane); // Other options are kClipNone = 0, kClipPlane, kClipBox    
     viewer->ColorSet().Background().SetColor(kBlack);
     viewer->SetGuideState(TGLUtil::kAxesOrigin, kTRUE, kFALSE, 0); // EAxesType { kAxesNone, kAxesEdge, kAxesOrigin }
     viewer->UseDarkColorSet(); 
     viewer->RefreshPadEditor(viewer);	
     viewer->CurrentCamera().RotateRad(-0.5, 0.5);
     viewer->DoDraw();
     Double_t center[3] ={0.,0.,0.};
     Double_t *cent; cent = center;
     viewer->SetOrthoCamera(TGLViewer::kCameraOrthoZOX,10.0, 1.0, cent,1.0,0.); // kCameraPerspXOZ, kCameraPerspYOZ, kCameraPerspXOY, kCameraOrthoXOY,
     viewer->SavePicture(Form("%s.png",volname.Data()));
    
    c[i] = new TCanvas(Form("radlen_%d",i), "radiation length");
	  c[i]->SetRightMargin(0.18); 
    TH2F *X0 = gGeoManager->GetVolume(i)->LegoPlot(180,0.,180.,360,0.,360.);
    X0->SetTitle(Form("%s",volname.Data()));
    X0->SetXTitle( "#varphi (deg)");
    X0->SetYTitle( "#theta (deg)");
    X0->SetZTitle( "X/X0");
    X0->GetXaxis()->SetRangeUser(0.,360.);
    X0->GetZaxis()->SetTitleOffset(1.4);
	  gStyle->SetPalette(kInvertedDarkBodyRadiator);
    X0->SetStats(kFALSE);
    X0->Draw("colz");
    c[i]->Update();
    TPaletteAxis *palette = (TPaletteAxis*)X0->GetListOfFunctions()->FindObject("palette");
   double y1 = palette->GetY1NDC();
   double y2 = palette->GetY2NDC();
   double x1 = palette->GetX1NDC();
   double x2 = palette->GetX2NDC();
   palette->SetX1NDC(x1+0.02);
   palette->SetX2NDC(x2+0.02);
   c[i]->Print(Form("MB_%s.png",volname.Data()));
     
   //kCameraOrthoXOZ, kCameraOrthoZOY, kCameraOrthoXnOY, kCameraOrthoXnOZ,kCameraOrthoZnOY
  // v->CurrentCamera().RotateRad(-.7, 0.5);
         
      /*   TPaveText* t1=new TPaveText(0.80,0.90,0.99,0.99,"NDC");
         t1->SetFillStyle(0);
         t1->SetBorderSize(0);
         t1->SetTextSize(0.05);
         t1->SetTextFont(42);
         t1->SetTextAlign(13);
         t1->AddText(0.,0.,"Detector1"); 
         t1->SetTextColor(kWhite);
         t1->Draw();*/
   } 
  
}

