// Virtual geometry of ALPIDE Assembly Shyam Kumar
// shyam.kumar@cern.ch

void virtual_geo(){
	
		gSystem->Load("libGeom");
		TGeoManager *gGeoMan = new TGeoManager("Beam_Test","ALPIDE Geometry");
		TGeoMedium *medium = 0;

		//--- define some materials
		TGeoMaterial *matVacuum = new TGeoMaterial("Vacuum", 0,0,0);
		TGeoMaterial *matSi = new TGeoMaterial("Silicon",28.0855,14,9.37);
		//   //--- define some media
		TGeoMedium *Vacuum = new TGeoMedium("Vacuum",1, matVacuum);
		TGeoMedium *Si = new TGeoMedium("Si",2, matSi);

		//==World Volume=============
		Double_t worldx = 250.;
		Double_t worldy = 250.;
		Double_t worldz = 250.;

		TGeoVolume *top = gGeoMan->MakeBox("Tracker",Vacuum,worldx,worldy,worldz);
		gGeoMan->SetTopVolume(top);

		// make shape components
		Double_t r[5] =  {3.0,5.0,8.0,10.,12.};
		Double_t Disks[6]={-50.,-40.,-30.,30.,40.,50.};

		TGeoTube *CylAlpide[7];         TGeoTube *PlaneAlpide[6]; 
		TGeoVolume* CylAlpide_Vol[7];   TGeoVolume* PlaneAlpide_Vol[6];

		for (int i=0; i<5; i++){ 
		CylAlpide[i] = new TGeoTube(Form("Barrel%d",i),r[i],r[i]+0.0025,20.);
		CylAlpide_Vol[i] = new TGeoVolume(Form("Barrel%d",i),CylAlpide[i],Si);
  CylAlpide_Vol[i]->SetLineColor(kMagenta);
		}

		for (int i=0; i<6; i++){ 
		PlaneAlpide[i] = new TGeoTube(Form("Disks%d",i),0.5,20.0,0.05);
		PlaneAlpide_Vol[i] = new TGeoVolume(Form("Disks%d",i+3),PlaneAlpide[i],Si);
  PlaneAlpide_Vol[i]->SetLineColor(kOrange);
		}
		//TGeoBBox *sbox  = new TGeoBBox("B",100,125,125);

		//TGeoVolume* vsbox = new TGeoVolume("Box",sbox,Si);
		//TGeoVolume* vstub = new TGeoVolume("TUB",stub,Si);

			TGeoVolume *combinedgeo = new TGeoVolumeAssembly("CombinedGeometry");
			combinedgeo->AddNode(CylAlpide_Vol[0],1);
			combinedgeo->AddNode(CylAlpide_Vol[1],2);
			combinedgeo->AddNode(CylAlpide_Vol[2],3);
			combinedgeo->AddNode(CylAlpide_Vol[3],4);
			combinedgeo->AddNode(CylAlpide_Vol[4],5);
			combinedgeo->AddNode(PlaneAlpide_Vol[0],6, new TGeoTranslation(0,0,Disks[0]));
			combinedgeo->AddNode(PlaneAlpide_Vol[1],7, new TGeoTranslation(0,0,Disks[1]));
			combinedgeo->AddNode(PlaneAlpide_Vol[2],8, new TGeoTranslation(0,0,Disks[2]));
			combinedgeo->AddNode(PlaneAlpide_Vol[3],9, new TGeoTranslation(0,0,Disks[3]));
			combinedgeo->AddNode(PlaneAlpide_Vol[4],10, new TGeoTranslation(0,0,Disks[4]));
			combinedgeo->AddNode(PlaneAlpide_Vol[5],11, new TGeoTranslation(0,0,Disks[5]));
		// combinedgeo->AddNode(vstub,2);
			top->AddNode(combinedgeo,0);
	  top->Voxelize("");

		// You can also write Individually
				// put it in the top volume
				//top->AddNode(vsbox,1);
				//top->AddNode(vstub,2);
	    gGeoMan->CheckOverlaps(0.001);
	    gGeoMan->PrintOverlaps();
     gGeoMan->SetVisLevel(4);
     gGeoMan->SetVisOption(0);
	   // gGeoMan->Test();
					gGeoMan->CloseGeometry();
			  gGeoMan->Export("Test_geo.root");
					gGeoMan->SetVisLevel(30);
					top->Draw("ogl"); 
}
