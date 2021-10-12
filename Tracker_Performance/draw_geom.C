void draw_geom()
{
	
  TString rootfile="simple_geom.gdml"; // root geo or gdml will work Eic Tracker
  TEveManager::Create();
  gGeoManager = TGeoManager::Import(rootfile); // or use TGeoManager::Import(rootfile)
  if (gGeoManager == nullptr) return;
  TEveGeoTopNode *TNod = new TEveGeoTopNode(gGeoManager,gGeoManager->GetTopNode());
  
  gEve->AddGlobalElement(TNod);
  gEve->FullRedraw3D(kTRUE);
 
}


