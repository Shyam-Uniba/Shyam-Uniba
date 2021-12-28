void draw_geom()
{
	
  TString rootfile="simple_geom.gdml"; // root geo or gdml will work Eic Tracker
  TEveManager::Create();
  gGeoManager = TGeoManager::Import(rootfile); // or use TGeoManager::Import(rootfile)
  if (gGeoManager == nullptr) return;
  TEveGeoTopNode *TNod = new TEveGeoTopNode(gGeoManager,gGeoManager->GetTopNode());

   TObjArray* allvolumes = gGeoManager->GetListOfVolumes();
    for(Int_t i=0; i<allvolumes->GetEntries();i++){
          TGeoVolume* vol= (TGeoVolume*)allvolumes->At(i);
          TString volname = vol->GetIconName();
          if (volname.Contains("SVTX")) vol->SetLineColor(kRed); 	
          else if (volname.Contains("BARR")) vol->SetLineColor(kBlue); 
          vol->SetTransparency(20); // set the transparency level for the volumes
  }
  
  gEve->AddGlobalElement(TNod);
  gEve->FullRedraw3D(kTRUE);
 
}


