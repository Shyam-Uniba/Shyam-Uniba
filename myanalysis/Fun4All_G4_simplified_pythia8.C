/*
   ================================================================================================================
   The purpose of this code is to have a version of the all-silicon tracker that is simplified so that we can study
   different variations of the geometry quickly. Specifically, I wrote this code to study the impact that changing
   the material budget of different regions of the detector would have on different resolutions.
   ================================================================================================================
   */
#pragma once
#include <phgenfit/Track.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllDummyInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllNoSyncDstInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/SubsysReco.h>
#include <g4detectors/PHG4DetectorSubsystem.h>
#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4histos/G4HitNtuple.h>
#include <phpythia8/PHPythia8.h>
#include <g4main/HepMCNodeReader.h>
#include <g4main/PHG4ParticleGenerator.h>
#include <g4main/PHG4ParticleGeneratorBase.h>
#include <g4main/PHG4Reco.h>
#include <g4main/PHG4TruthSubsystem.h>
#include <g4trackfastsim/PHG4TrackFastSim.h>
#include <g4trackfastsim/PHG4TrackFastSimEval.h>
#include <g4lblvtx/TrackFastSimEval.h>
#include <phool/recoConsts.h>
#include <g4lblvtx/SimpleNtuple.h>
#include <g4lblvtx/MyJetAnalysis_AllSi.h>
#include <g4lblvtx/EicFRichSubsystem.h>			// Forward RICH
#include <g4lblvtx/PHG4ParticleGenerator_flat_pT.h>	// Flat-pT generator
#include <g4lblvtx/AllSi_Al_support_Subsystem.h>	// Aluminum cone
#include "G4_BlackHole.C"				// Blackhole
#include "G4_Pipe_EIC.C"				// Beampipe
#include "G4_GEM.C"					// GEMs
#include "G4_DIRC_SMALL.C"				// DIRC
#include "G4_AllSi_Simp.C"
#include "G4_Jets.C"
#include "G4_Bbc.C"
#include "G4_Global.C"

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4detectors.so)
R__LOAD_LIBRARY(libg4lblvtx.so)
R__LOAD_LIBRARY(libg4trackfastsim.so)
R__LOAD_LIBRARY(libPHPythia8.so)

void Fun4All_G4_simplified_pythia8(
			int nEvents        = 5   ,	// number of events
			bool include_RICH  = false ,	// if true, RICH material will be included
			double GEM_res     = 50.   ,	// um, if > 0 forward, backward GEMs will be included
			int nDircSectors   = -1    ,	// Number of Quartz bars in the DIRC
			int magnetic_field = 5,	// Magnetic field setting
			bool do_projections = false,	// Projections
			TString out_name   = "out" )	// output filename
{	
	// ======================================================================================================
	// Input from the user
	double vtx_matBud  = 0.05; // % X/X0 (material budget of vertexing layers)
	double barr_matBud = 0.55; // % X/X0 (material budget of middle layers)
	double disk_matBud = 0.24; // % X/X0 (material budget of disk layers)
	const int particle_gen = 4;// 1 = particle generator, 2 = particle gun, 3 = simple event generator, 4 = pythia8 e+p collision, 5 = particle generator flat in pT
	double si_pix_size = 10.; // um - size of pixels in vertexing layers
	bool use_blackhole = false;
	const int nDisks_per_side = 5;
	double pmin =  0.; // GeV/c
	double pmax = 30.; // GeV/c
	double eta_min =-3.7;
	double eta_max = 3.7;
	// ======================================================================================================
	// Parameters for projections
	string projname1   = "DIRC";            // Cylindrical surface object name
	double projradius1 = 82.;               // [cm] 
	double length1     = 290.;              // [cm]
	// ---
	double thinness    = 0.1;               // black hole thickness, needs to be taken into account for the z positions
	// ---
	string projname2   = "FOR";             // Forward plane object name
	double projzpos2   = 145+thinness/2.;   // [cm]
	double projradius2 = 100.;              // [cm]
	// ---
	string projname3   = "BACK";            // Backward plane object name
	double projzpos3   = -(138+thinness/2.);// [cm]
	double projradius3 = 130.;              // [cm]
	// ======================================================================================================
	// Make the Server
	Fun4AllServer *se = Fun4AllServer::instance();
	// If you want to fix the random seed for reproducibility
	// recoConsts *rc = recoConsts::instance();
	// rc->set_IntFlag("RANDOMSEED", 12345);
	// ======================================================================================================
	if(nDircSectors>0)
		DIRCInit();
	// ======================================================================================================
//http://fismed.ciemat.es/GAMOS/GAMOS_doc/GAMOS.5.1.0/x11519.html
	// Particle Generator Setup
	PHG4ParticleGenerator *gen = new PHG4ParticleGenerator();
	gen->set_name(std::string("proton"));	// geantino, pi-, pi+, mu-, mu+, e-., e+, proton, ... (currently passed as an input)
	gen->set_vtx(0,0,0);			// Vertex generation range
	gen->set_mom_range(pmin,pmax);		// Momentum generation range in GeV/c
	gen->set_z_range(0.,0.);
	gen->set_eta_range(eta_min,eta_max);
	gen->set_phi_range(0,2.*TMath::Pi());
	// --------------------------------------------------------------------------------------
	// Particle generator flat in pT
	PHG4ParticleGenerator_flat_pT *gen_pT = new PHG4ParticleGenerator_flat_pT();
	gen_pT->set_name(std::string("pi-"));     // geantino, pi-, pi+, mu-, mu+, e-., e+, proton, ... (currently passed as an input)
	gen_pT->set_vtx(0,0,0);                    // Vertex generation range
	gen_pT->set_pT_range(pmin,pmax);         // Momentum generation range in GeV/c
	gen_pT->set_z_range(0.,0.);
	gen_pT->set_eta_range(eta_min,eta_max);               // Detector coverage corresponds to |η|< 4
	gen_pT->set_phi_range(0.,2.*TMath::Pi());
	// ======================================================================================================
 gSystem->Setenv("PYTHIA8DATA",Form("%s/xmldoc",getenv("PYTHIA8")));
	bool do_pythia8_jets = false;	
	if(particle_gen==1){se->registerSubsystem(   gen); cout << "Using particle generator"             << endl;}
	else if(particle_gen==5){se->registerSubsystem(gen_pT); cout << "Using particle generator flat in pT"  << endl;}
 else if(particle_gen==4){
		do_pythia8_jets = true;
		gSystem->Load("libPHPythia8.so");

		PHPythia8 *pythia8 = new PHPythia8(); // see coresoftware/generators/PHPythia8 for example config
		pythia8->set_config_file("phpythia8.cfg"); // example configure files : https://github.com/sPHENIX-Collaboration/coresoftware/tree/master/generators/PHPythia8 
  pythia8->print_config();
	 pythia8->Verbosity(10);
		se->registerSubsystem(pythia8);

		// read-in HepMC events to Geant4 if there are any pythia8 produces hepmc records, so this is needed to read the above generated pythia8 events
		HepMCNodeReader *hr = new HepMCNodeReader();
		se->registerSubsystem(hr);
	}
	else{ cout << "Particle generator option requested has not been implemented. Bailing out!" << endl; exit(0); }
	// ======================================================================================================
	PHG4Reco *g4Reco = new PHG4Reco();
	//g4Reco->SetWorldMaterial("G4_Galactic");	
	// ======================================================================================================
	// Magnetic field setting
	TString B_label;
	if(magnetic_field==1){          // uniform 1.5T
		B_label = "_B_1.5T";
		g4Reco->set_field(1.5);
	}
	else if(magnetic_field==2){     // uniform 3.0T
		B_label = "_B_3.0T";
		g4Reco->set_field(3.0);
	}
	else if(magnetic_field==3){     // sPHENIX 1.4T map
		B_label = "_B_BaBar";
		g4Reco->set_field_map(string(getenv("CALIBRATIONROOT")) + string("/Field/Map/sPHENIX.2d.root"), PHFieldConfig::kField2D);
		g4Reco->set_field_rescale(-1.4/1.5);
	}
	else if(magnetic_field==4){     // Beast 3.0T map
		B_label = "_B_BeAST";
		g4Reco->set_field_map(string(getenv("CALIBRATIONROOT")) + string("/Field/Map/mfield.4col.dat"), PHFieldConfig::kFieldBeast);
	}
	else if(magnetic_field==5){
		B_label = "_B_ATHENA_210507";
		TString path_to_map = "/home/shyam/Singularity/myanalysis/magfield/EIC_Magnetic_Field_Map_2021_05_07_radial_coords_[cm]_[T].120000.lines.Bmap";
		g4Reco->set_field_map(string(path_to_map), PHFieldConfig::kFieldBeast);
	}
	else if(magnetic_field==6){
		B_label = "_B_ATHENA_210528";
		TString path_to_map = "/home/shyam/Singularity/g4lblvtx/macros/auxiliary_studies/simplified_geometry/magfield/EIC_v.0.1.0_Magnetic_Field_Map_2021_05_28_radial_coords_[cm]_[T].401301.line.Bmap";
		g4Reco->set_field_map(string(path_to_map), PHFieldConfig::kFieldBeast);
	}
	else{                           // The user did not provide a valid B field setting
		cout << "User did not provide a valid magnetic field setting. Set 'magnetic_field'. Bailing out!" << endl;
	}	
	// ======================================================================================================
	// Detector setup
	PHG4CylinderSubsystem *cyl;
	//---------------------------
	double allsi_overall_radius = 43.23; // cm
	double allsi_overall_z = 121.;// cm

	AllSi_Simp_Geometry(
                        g4Reco               ,
                        vtx_matBud           , // % X0 - vertex material budget
                        barr_matBud          , // % X0 - barrel material budget
                        disk_matBud          , // % X0 - disk material budget
                        nDisks_per_side      , // number of disks per side
                        25.                  , // cm - z position of first disk
                        allsi_overall_z      , // cm - z position of last disk
                        36.5                 , // deg - theta angle of support cone
                        21.00                , // cm - first barrel layer (without counting vertexing layers)
                        22.68                , // cm - second barrel layer (without counting vertexing layers)
                        39.30                , // cm - third barrel layer (without counting vertexing layers)
                        allsi_overall_radius , // cm - fourth barrel layer (without counting vertexing layers)
                        30.00                , // cm - length of vertexing layers
                        3.30                 , // cm - radius of first vertex layer
                        5.70                 , // cm - radius of second vertex layer
                        -1.0                   // cm - radius of third vertex layer. If <0, this layer is not created
                        );
	//---------------------------
	// Black hole to suck loopers out of their misery
	double BH_r = allsi_overall_radius;
	double BH_zmin = -(allsi_overall_z+2);
	double BH_zmax = allsi_overall_z;
	if(use_blackhole)
		wrap_with_cylindrical_blackhole(g4Reco,BH_r,BH_zmin,BH_zmax);
	//---------------------------
	// Beam pipe
	PipeInit(); // Load beampipe from Fun4All rather than from gdml file
	double pipe_radius = 0;
	pipe_radius = Pipe(g4Reco,pipe_radius,true);
	//---------------------------
	// DIRC
	if(nDircSectors>0)
		double dirc_out_skin = DIRCSetup(g4Reco,nDircSectors);	
	//---------------------------
	// forward RICH
	if(include_RICH){
		EicFRichSubsystem *RICH = new EicFRichSubsystem("RICH");
		g4Reco->registerSubsystem(RICH);
	}
	//---------------------------
	// Forward - Backward GEMs
	if(GEM_res>0){
		EGEM_Init();                            // Loading backward GEM geometry
		FGEM_Init();                            // Loading forward GEM geometry
		EGEMSetup(g4Reco);
		FGEMSetup(g4Reco);
	}
	//---------------------------
	if(do_projections){
		cyl = new PHG4CylinderSubsystem(projname1);
                cyl->set_double_param("radius", projradius1);
                cyl->set_double_param("thickness", 0.01);  // does not matter (but > 0)
                cyl->SuperDetector(projname1);
                cyl->set_double_param("length", 90.);
                cyl->set_color(0.7,0.8,1,0.3);
                cyl->SetActive();
                cyl->SaveAllHits();  // save all hits, also zero energy hits (which are normally discarded)
                g4Reco->registerSubsystem(cyl);

                cyl = new PHG4CylinderSubsystem(projname2);
                cyl->set_double_param("radius", 0);
                cyl->set_double_param("thickness",projradius2); // for a cylindrical plane thickness is diameter
                cyl->set_double_param("length", 0.01);          // for a cylindrical plane length is depth
                cyl->set_double_param("place_z", projzpos2 );   // position in z, 1/2 depth needs to be added to put front at exactly 100 cm
                cyl->SuperDetector(projname2);
                cyl->set_color(0.7,0.8,1,0.3);
                cyl->SetActive();
                cyl->SaveAllHits();
                g4Reco->registerSubsystem(cyl);

                cyl = new PHG4CylinderSubsystem(projname3);
                cyl->set_double_param("radius", 0);
                cyl->set_double_param("thickness",projradius3); // for a cylindrical plane thickness is diameter
                cyl->set_double_param("length", 0.01);          // for a cylindrical plane length is depth
                cyl->set_double_param("place_z", projzpos3);    // position in z, 1/2 depth needs to be added to put front at exactly 100 cm
                cyl->SuperDetector(projname3);
                cyl->set_color(0.7,0.8,1,0.3);
                cyl->SetActive();
                cyl->SaveAllHits();
                g4Reco->registerSubsystem(cyl);
	}
	//---------------------------
	PHG4TruthSubsystem *truth = new PHG4TruthSubsystem();
	g4Reco->registerSubsystem(truth);

	se->registerSubsystem(g4Reco);

	// ======================================================================================================
	if(do_pythia8_jets){
		Bbc_Reco();
		Global_Reco();
	}

	// ======================================================================================================
	// ======================================================================================================
	// fast pattern recognition and full Kalman filter
	// output evaluation file for truth track and reco tracks are PHG4TruthInfoContainer
	//---------------------------
	PHG4TrackFastSim *kalman = new PHG4TrackFastSim("PHG4TrackFastSim");
	kalman->set_use_vertex_in_fitting(false);
	kalman->set_sub_top_node_name("BARR");
	kalman->set_trackmap_out_name("SvtxTrackMap");

	add_AllSi_to_Kalman(
                kalman ,     // pointer to Kalman filter
                si_pix_size, // sensor pitch in um
                1            // hit reconstruction efficiency
                );

	// add forward and backward GEMs
	if(GEM_res>0){
		// BACKWARD GEM
		kalman->add_phg4hits("G4HIT_EGEM",              // const std::string& phg4hitsNames,
				PHG4TrackFastSim::Vertical_Plane,               // const DETECTOR_TYPE phg4dettype,
				GEM_res/10000.,                                 // const float radres,
				GEM_res/10000.,                                 // const float phires,
				999.,                                           // longitudinal (z) resolution [cm] (this number is not used in vertical plane geometry)
				1,                                              // const float eff,
				0                                               // const float noise
				);
		// FORWARD GEM2
		kalman->add_phg4hits("G4HIT_FGEM",              // const std::string& phg4hitsNames,
				PHG4TrackFastSim::Vertical_Plane,               // const DETECTOR_TYPE phg4dettype,
				GEM_res/10000.,                                 // const float radres,
				GEM_res/10000.,                                 // const float phires,
				999.,                                           // longitudinal (z) resolution [cm] (this number is not used in vertical plane geometry)
				1,                                              // const float eff,
				0                                               // const float noise
				);	
	}

	// Projections  
	if(do_projections){
		kalman->add_cylinder_state(projname1, projradius1);     // projection on cylinder (DIRC)
		kalman->add_zplane_state  (projname2, projzpos2  );     // projection on vertical planes
		kalman->add_zplane_state  (projname3, projzpos3  );     // projection on vertical planes
	}

	kalman->Verbosity(0);
	kalman->set_use_vertex_in_fitting(false);
	kalman->set_vertex_xy_resolution(0);
	kalman->set_vertex_z_resolution(0);
	kalman->enable_vertexing(false); // this is false by default
	kalman->set_vertex_min_ndf(2);

	se->registerSubsystem(kalman);

	SimpleNtuple *hits = new SimpleNtuple("Hits");
 hits->AddNode("SVTX", 0);
 hits->AddNode("BARR", 1);
 hits->AddNode("FBST", 2);
 hits->AddNode("EGEM", 3);
 hits->AddNode("FGEM", 4);
	se->registerSubsystem(hits);
	// ======================================================================================================
	TString label_mat = Form("_AllSi_vbd_%.2f_%.2f_%.2f",vtx_matBud,barr_matBud,disk_matBud);
	TString label_RICH = "";
	TString label_GEM  = "";
	TString label_DIRC = "";

	if(include_RICH)
		label_RICH = "_RICH";

	if(GEM_res>0)
		label_GEM = Form("_GEM_res_%.1fum",GEM_res);

	if(nDircSectors>0)
		label_DIRC = Form("_DIRC_%i_sect",nDircSectors);

	TString outputFile = (std::string)(out_name)+std::string(label_mat)+std::string(label_RICH)+std::string(label_GEM)+std::string(label_DIRC)+std::string(B_label)+"_FastSimEval.root";
	TrackFastSimEval *fast_sim_eval = new TrackFastSimEval("FastTrackingEval");
	fast_sim_eval->set_filename(outputFile);
	if(do_projections){
		fast_sim_eval->AddProjection(projname1);
		fast_sim_eval->AddProjection(projname2);
		fast_sim_eval->AddProjection(projname3);
	}
	se->registerSubsystem(fast_sim_eval);

	// resonstruct jets after the tracking
	if(do_pythia8_jets) Jet_Reco();
	if(do_pythia8_jets) Jet_Eval(string(outputFile) + "_g4jet_eval.root");

	// ======================================================================================================
	// IOManagers...
	const std::string dst_name = std::string(out_name)+std::string(B_label)+"_G4LBLVtx.root";	
	Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT",dst_name);
	out->Verbosity(0);
	se->registerOutputManager(out);

	Fun4AllInputManager *in = new Fun4AllDummyInputManager("JADE");
	se->registerInputManager(in);

	if(do_pythia8_jets){
		gSystem->Load("libmyjetanalysis");
		std::string jetoutputFile = std::string(outputFile) + std::string("_electrons+jets.root");
		MyJetAnalysis_AllSi *myJetAnalysis = new MyJetAnalysis_AllSi("AntiKt_Track_r10","AntiKt_Truth_r10",jetoutputFile.data());	
	//	MyJetAnalysis_AllSi *myJetAnalysis = new MyJetAnalysis_AllSi("AntiKt_Track_r04","AntiKt_Truth_r04",jetoutputFile.data());
	//	MyJetAnalysis_AllSi *myJetAnalysis = new MyJetAnalysis_AllSi("AntiKt_Track_r02","AntiKt_Truth_r02",jetoutputFile.data());
		se->registerSubsystem(myJetAnalysis);
	}
	if (nEvents <= 0) return;
	se->run(nEvents);
	se->End();
// g4Reco->Dump_GDML("simple_geom.gdml");
//	delete se;

//	gSystem->Exit(0);
}
