void material_budget_Sim(Int_t nEvents = 1000, TString mcEngine = "TGeant4", Bool_t isMT = false)
{
  // Output file name
  Int_t MagOpt=1;  // Non-uniform mag field; 0 for uniform mag field of 0.6 T
 // Int_t MagOpt=0;  // Uniform mag field of 0.6 T
  TString outFile ="tpc_sim.root";
    
  // Parameter file name
  TString parFile="tpc_params.root";
  
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // -----   Create simulation run   ----------------------------------------
  FairRunSim* run = new FairRunSim();
  run->SetName(mcEngine);                               // Transport engine
  run->SetSink(new FairRootFileSink(outFile));          // Output file
  run->SetIsMT(isMT);
  FairRuntimeDb* rtdb = run->GetRuntimeDb();

  // ------------------------------------------------------------------------
  
  // -----   Create media   -------------------------------------------------
  run->SetMaterials("media.geo");       // Materials
  // ------------------------------------------------------------------------
  
  // -----   Create geometry   ----------------------------------------------

  FairModule* cave= new CeeCave("CAVE");
  cave->SetGeometryFileName("cave.geo");
  run->AddModule(cave);

  FairModule* magnet = new CeeMagnet("Magnet");
  run->AddModule(magnet);

  FairDetector* TPC= new CeeTpc("TPC", kTRUE);
  TPC->SetGeometryFileName("tpc_v2.root");
  run->AddModule(TPC);

 // ------------------------------------------------------------------------


    // -----   Magnetic field   -------------------------------------------
    // Constant Field
    if (MagOpt == 0){
       CeeFieldConst  *fMagField = new CeeFieldConst();
       fMagField->SetField(0., 5. ,0. ); // values are in kG
       fMagField->SetFieldRegion(-60, 60,-45, 45, -10, 90); // values are in cm
                             //  (xmin,xmax,ymin,ymax,zmin,zmax)
       run->SetField(fMagField);
       cout <<"------------------- "<< endl;
       cout <<" Uniform Magnet field B=0.5T is initialized for this run" << endl;
       cout <<"------------------- "<< endl;
    }
    else{
       TString fieldMap = "field_v1";//Field v1 is the CEE magnetic field and field v1.1  is the selected uniform part
       CeeFieldMap* fMagField = new CeeFieldMapSym3(fieldMap);
       fMagField->SetPosition(0., 0., 0);
       fMagField->SetScale(1);
       fMagField->SetAdjust(0, 0, -35, 20, 1., 0.9, 1.);
       run->SetField(fMagField);
       cout <<"------------------- "<< endl;
       cout <<" Non-Uniform Magnet field is initialized for this run" << endl;
       cout <<"------------------- "<< endl;
    }
    // --------------------------------------------------------------------

  
    
  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  
    // Add a box generator also to the run
    FairBoxGenerator* boxGen = new FairBoxGenerator(0, 5); // 0 = geantino; 5 = multipl.
    boxGen->SetPRange(0.10,1.50); // GeV/c
    boxGen->SetPhiRange(0., 360.); // Azimuth angle range [degree]
    boxGen->SetThetaRange(0., 90.); // Polar angle in lab system range [degree]
    boxGen->SetXYZ(0., 0., -16.); // cm
    primGen->AddGenerator(boxGen);
    run->SetGenerator(primGen);
// ------------------------------------------------------------------------
 
  //---Store the visualiztion info of the tracks, this make the output file very large!!
  //--- Use it only to display but not for production!
  run->SetStoreTraj(kTRUE);
  run->SetRadLenRegister(kTRUE);
   //--------------------Set some cuts for the visualization------------------
//  FairTrajFilter* trajFilter = FairTrajFilter::Instance(); 
//  trajFilter->SetStepSizeCut(0.001); //1 mm
  
    
  // -----   Initialize simulation run   ------------------------------------
  run->Init();

  //--------------------Set some cuts for the visualization------------------
  FairTrajFilter* trajFilter = FairTrajFilter::Instance(); 
  trajFilter->SetStepSizeCut(0.1); //1 mm
//  trajFilter->SetStorePrimaries(kTURE);
//  trajFilter->SetStoreSecondaries(kTURE);

  // ------------------------------------------------------------------------

  // -----   Runtime database   ---------------------------------------------

  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();
  // ------------------------------------------------------------------------
   
  // -----   Start run   ----------------------------------------------------
   run->Run(nEvents);
    
  // ------------------------------------------------------------------------
    //You can export your ROOT geometry to a separate file
 // run->CreateGeometryFile("geofile_full.root");
  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is "    << outFile << endl;
  cout << "Parameter file is " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime 
       << "s" << endl << endl;
  // ------------------------------------------------------------------------
}



