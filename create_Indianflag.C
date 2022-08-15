// Virtual geometry of ALPIDE Assembly Shyam Kumar
// shyam.kumar@cern.ch

void create_Indianflag(){
	
		gSystem->Load("libGeom");
		TGeoManager *gGeoMan = new TGeoManager("Beam_Test","ALPIDE Geometry");
		TGeoMedium *medium = 0;

		//--- define some materials
		TGeoMaterial *matVacuum = new TGeoMaterial("Vacuum", 0,0,0);
		// Silicon density of 2.32 g/cm^3 but there is some issue with Radition length so ,ultiplied by 0.1
		TGeoMaterial *matSi = new TGeoMaterial("Silicon",28.0855,14,0.2329,9.37); 
		//   //--- define some media
		TGeoMedium *Vacuum = new TGeoMedium("Vacuum",1, matVacuum);
		TGeoMedium *Si = new TGeoMedium("Si",2, matSi);

		//==World Volume=============
		Double_t worldx = 250.;
		Double_t worldy = 250.;
		Double_t worldz = 250.;

		TGeoVolume *top = gGeoMan->MakeBox("ALPIDE",Vacuum,worldx,worldy,worldz);
		gGeoMan->SetTopVolume(top);

		// make shape components
		Double_t r[3] = {5.0,24.0,30.0};
		Double_t Disks[6]={-100.,-75.,-50.,50.,75.,100.};

		TGeoTube *CylAlpide[3];         TGeoBBox *PlaneAlpide[20]; 
		TGeoVolume* CylAlpide_Vol[3];   TGeoVolume* PlaneAlpide_Vol[20];



		for (int i=0; i<3; i++){ 
		PlaneAlpide[i] = new TGeoBBox(Form("ALPIDE%d",i),20.,5.,0.0025);
		PlaneAlpide_Vol[i] = new TGeoVolume(Form("ALPIDE%d",i),PlaneAlpide[i],Si);
		}
   PlaneAlpide_Vol[0]->SetLineColor(kOrange+7);
   PlaneAlpide_Vol[1]->SetLineColor(kWhite);
   PlaneAlpide_Vol[2]->SetLineColor(kGreen);
   
   		for (int i=0; i<1; i++){ 
		CylAlpide[i] = new TGeoTube(Form("ALPIDE%d",i+6),r[i],r[i]+0.0050,0.1);
		CylAlpide_Vol[i] = new TGeoVolume(Form("ALPIDE%d",i+6),CylAlpide[i],Si);
		CylAlpide_Vol[i]->SetLineColor(kBlue+2);
		}

             
             
             		for (int i=3; i<15; i++){ 
		PlaneAlpide[i] = new TGeoBBox(Form("ALPIDE%d",i),5.,0.1,0.1);
		PlaneAlpide_Vol[i] = new TGeoVolume(Form("ALPIDE%d",i),PlaneAlpide[i],Si);
		   PlaneAlpide_Vol[i]->SetLineColor(kBlue+2);
		}

               top->AddNode(CylAlpide_Vol[0],1, new TGeoTranslation(0.,-10.,0.));
			top->AddNode(PlaneAlpide_Vol[0],2, new TGeoTranslation(0.,0.,0.));
			top->AddNode(PlaneAlpide_Vol[1],3, new TGeoTranslation(0.,-10.,0.));
			top->AddNode(PlaneAlpide_Vol[2],4, new TGeoTranslation(0.,-20.,0.));
			
			
	         TGeoVolumeAssembly* chakra = new TGeoVolumeAssembly("chakra");
			for (int i=3; i<15; i++){
			double angle = 15*(i-3); 
		     chakra->AddNode(PlaneAlpide_Vol[i],i+2, new TGeoRotation("rot", angle, 0., 0.));
		     chakra->SetLineColor(kBlue+2);
		     }
		     top->AddNode(chakra,5, new TGeoTranslation(0.,-10.,0.));
			top->Voxelize("");

	    gGeoMan->CheckOverlaps(0.001);
	    gGeoMan->PrintOverlaps();
     gGeoMan->SetVisLevel(4);
     gGeoMan->SetVisOption(0);
	   // gGeoMan->Test();
					gGeoMan->CloseGeometry();
			  gGeoMan->Export("Indianflag.root");
					gGeoMan->SetVisLevel(30);
					top->Draw("ogl"); 
}
