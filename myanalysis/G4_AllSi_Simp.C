#ifndef MACRO_ALLSI_SIMP_C
#define MACRO_ALLSI_SIMP_C

#include "GlobalVariables.C"
#include <g4detectors/PHG4SectorSubsystem.h>
#include <g4main/PHG4Reco.h>
#include <string>
#include <g4lblvtx/AllSi_Al_support_Subsystem.h>

// =================================================
// macro written by R. Cruz-Torres (reynier@lbl.gov)
// =================================================

void create_vertexing_layers(PHG4Reco *g4Reco, double mat_bud=0.05, double si_z_vtxlength=30., double r1=3.30, double r2 = 5.70, double r3=-1);
void create_barrel_layers(PHG4Reco *g4Reco, double mat_budget_barrel=0.55, double cone_theta_deg=36.5, double barrel_1_r=21.00, double barrel_2_r=22.68, double barrel_3_r=39.30, double barrel_4_r=43.23);
void create_disk_layers(PHG4Reco *g4Reco, double mat_budget_disks=0.24, int n_disks=5, double z_disk_1=25., double z_disk_f=121., double cone_theta_deg=36.5,double barrel_4_r=43.23);
void add_support_structure(PHG4Reco *g4Reco, double cone_theta_deg=36.5, double barrel_4_r=43.23, double z_disk_f=121.);

R__LOAD_LIBRARY(libg4detectors.so)
// ======================================================================================================================
int AllSi_Simp_Geometry(PHG4Reco *g4Reco        ,
		double mat_budget_vertex = 0.05 , // % X0 - material budget for vertexing layers
		double mat_budget_barrel = 0.55 , // % X0 - material budget for barrel layers
		double mat_budget_disks  = 0.24 , // % X0 - material budget for disk layers
		int n_disks              = 5    , // number of disks per side
		double z_disk_1          = 25.  , // cm - z position of first disk
		double z_disk_f          = 121. , // cm - z position of last disk
		double cone_theta_deg    = 36.5 , // deg - theta angle of support cone
		double barrel_1_r        = 21.00, // cm - first barrel layer (without counting vertexing layers)
		double barrel_2_r        = 22.68, // cm - second barrel layer (without counting vertexing layers)
		double barrel_3_r        = 39.30, // cm - third barrel layer (without counting vertexing layers)
		double barrel_4_r        = 43.23, // cm - fourth barrel layer (without counting vertexing layers)
		double vertex_length     = 30.00, // cm - length of vertexing layers
		double vertex_1_r        =  3.30, // cm - radius of first vertex layer
		double vertex_2_r        =  5.70, // cm - radius of second vertex layer
		double vertex_3_r        =  -1.0  // cm - radius of third vertex layer. If <0, this layer is not created   
		){
	create_vertexing_layers(g4Reco,mat_budget_vertex,vertex_length,vertex_1_r,vertex_2_r,vertex_3_r);
	create_barrel_layers   (g4Reco,mat_budget_barrel,cone_theta_deg,barrel_1_r,barrel_2_r,barrel_3_r,barrel_4_r);
	create_disk_layers     (g4Reco,mat_budget_disks,n_disks,z_disk_1,z_disk_f,cone_theta_deg,barrel_4_r);
	add_support_structure  (g4Reco,cone_theta_deg,barrel_4_r,z_disk_f);
	return 0;
}
// ======================================================================================================================
int add_AllSi_to_Kalman(
		PHG4TrackFastSim *kalman , // pointer to Kalman filter
		double pixel_pitch_um=10., // sensor pitch in um
		double hit_efficiency=1    // hit reconstruction efficiency
		){
	// add Vertexing Layers
	kalman->add_phg4hits(
			"G4HIT_SVTX",                           // const std::string& phg4hitsNames,
			PHG4TrackFastSim::Cylinder,
			999.,                                   // radial-resolution [cm]
			pixel_pitch_um/10000./sqrt(12.),        // azimuthal-resolution [cm]
			pixel_pitch_um/10000./sqrt(12.),        // z-resolution [cm]
			hit_efficiency,                         // efficiency,
			0                                       // noise hits
			);

	// add Barrel Layers
	kalman->add_phg4hits(
			"G4HIT_BARR",                           // const std::string& phg4hitsNames,
			PHG4TrackFastSim::Cylinder,
			999.,                                   // radial-resolution [cm]
			pixel_pitch_um/10000./sqrt(12.),        // azimuthal-resolution [cm]
			pixel_pitch_um/10000./sqrt(12.),        // z-resolution [cm]
			hit_efficiency,                         // efficiency,
			0                                       // noise hits
			);

	// add Disk Layers
	kalman->add_phg4hits(
			"G4HIT_FBST",                           // const std::string& phg4hitsNames,
			PHG4TrackFastSim::Vertical_Plane,
			pixel_pitch_um/10000./sqrt(12.),        // radial-resolution [cm]
			pixel_pitch_um/10000./sqrt(12.),        // azimuthal-resolution [cm]
			999.,                                   // z-resolution [cm]
			hit_efficiency,                         // efficiency,
			0                                       // noise hits
			);
	return 0;
}
// ======================================================================================================================
void create_vertexing_layers(
		PHG4Reco *g4Reco,
		double mat_bud       , // % X0
		double si_z_vtxlength, // cm - length of vertexing layers
		double r1            , // cm - radius of first vertex layer
		double r2            , // cm - radius of second vertex layer
		double r3              // cm - radius of third vertex layer. If <0, this layer is not created
		){
	PHG4CylinderSubsystem *cyl;
	double si_vtx_r_pos[3] = {0};
	si_vtx_r_pos[0] = r1;
	si_vtx_r_pos[1] = r2;
	si_vtx_r_pos[2] = r3;
	int nVtxLayers = 2;
	if(r3>0) nVtxLayers = 3; // If a third radius was specified, create a third vertexing layer
	double si_thick_vtx = mat_bud/100.*9.37;
	for (int ilayer = 0; ilayer < nVtxLayers ; ilayer++){
		cyl = new PHG4CylinderSubsystem("SVTX", ilayer);
		cyl->set_string_param("material" , "G4_Si"               );
		cyl->set_double_param("radius"   , si_vtx_r_pos[ilayer]  );
		cyl->set_double_param("thickness", si_thick_vtx          );
		cyl->set_double_param("place_z"  , 0                     );
		cyl->set_double_param("length"   , si_z_vtxlength        );
		cyl->SetActive();
		cyl->SuperDetector("SVTX");
		cyl->set_color(0,0.8,0.1);
		g4Reco->registerSubsystem(cyl);
	}
}
// ======================================================================================================================
void create_barrel_layers(
		PHG4Reco *g4Reco,
		double mat_budget_barrel, // % X0
		double cone_theta_deg   , // deg - theta angle of support cone
		double barrel_1_r       , // cm - first barrel layer (without counting vertexing layers)
		double barrel_2_r       , // cm - second barrel layer (without counting vertexing layers)
		double barrel_3_r       , // cm - third barrel layer (without counting vertexing layers)
		double barrel_4_r         // cm - fourth barrel layer (without counting vertexing layers)
		){
	PHG4CylinderSubsystem *cyl;

	double si_r_pos[] = {barrel_1_r,barrel_2_r,barrel_3_r,barrel_4_r};
	const int nTrckLayers = sizeof(si_r_pos)/sizeof(*si_r_pos);
	//double si_z_length[] = {54.,60.,105.,114.};
	double si_thick_bar = mat_budget_barrel/100.*9.37;

	for (int ilayer = 0; ilayer < nTrckLayers ; ilayer++){
		cyl = new PHG4CylinderSubsystem("BARR", ilayer);
		cyl->set_string_param("material" , "G4_Si"            );
		cyl->set_double_param("radius"   , si_r_pos[ilayer]   );
		cyl->set_double_param("thickness", si_thick_bar       );
		cyl->set_double_param("place_z"  , 0                  );
		//cyl->set_double_param("length"   , si_z_length[ilayer]);
		cyl->set_double_param("length"   , si_r_pos[ilayer]*2./tan(3.14159/180.*cone_theta_deg)/1.03);
		cyl->SetActive();
		cyl->SuperDetector("BARR");
		cyl->set_color(0,0.5,1);
		g4Reco->registerSubsystem(cyl);
	}
}
// ======================================================================================================================
void create_disk_layers(
		PHG4Reco *g4Reco,
		double mat_budget_disks, // % X0
		int n_disks            , // number of disks per side
		double z_disk_1        , // cm - z position of first disk
		double z_disk_f        , // cm - z position of last disk
		double cone_theta_deg  , // deg - theta angle of support cone
		double barrel_4_r        // cm - overall detector radius
		){
	PHG4CylinderSubsystem *cyl;
	const double disk_to_disk_distance = (z_disk_f-z_disk_1)/((float)n_disks-1.);
	const int total_n_disks = n_disks*2;
	double si_z_pos[20] = {0};
	double si_r_max[20] = {0};
	double si_r_min[20] = {0};
	double si_thick_disk = mat_budget_disks/100.*9.37;

	for(int i = 0        ; i < n_disks   ; i++) si_z_pos[i] = -z_disk_f + (float)i*disk_to_disk_distance;
	for(int i = total_n_disks-1 ; i > n_disks-1 ; i--) si_z_pos[i] = z_disk_1 + (float)(i-n_disks)*disk_to_disk_distance;

	for(int i = 0 ; i < total_n_disks ; i++){
		si_r_max[i] = TMath::Min(barrel_4_r,tan(3.14159/180.*cone_theta_deg)*abs(si_z_pos[i]));
		if(si_z_pos[i]>66.8&&si_z_pos[i]>0) si_r_min[i] = (0.05025461*si_z_pos[i]-0.180808);
		else if(si_z_pos[i]>0) si_r_min[i] = 3.18;
		else if(si_z_pos[i]<-79.8&&si_z_pos[i]<0) si_r_min[i] = (-0.0297039*si_z_pos[i]+0.8058281);
		else si_r_min[i] = 3.18;
		si_r_max[i] -= si_r_min[i];
	}

	for (int ilayer = 0; ilayer < total_n_disks ; ilayer++){
		cyl = new PHG4CylinderSubsystem("FBVS", ilayer);
		cyl->set_string_param("material" , "G4_Si"         );
		cyl->set_double_param("radius"   , si_r_min[ilayer]);
		cyl->set_double_param("thickness", si_r_max[ilayer]);
		cyl->set_double_param("place_z"  , si_z_pos[ilayer]);
		cyl->set_double_param("length"   , si_thick_disk   );
		cyl->SetActive();
		cyl->SuperDetector("FBST");
		cyl->set_color(1,0,0);
		g4Reco->registerSubsystem(cyl);
	}
}
// ======================================================================================================================
void add_support_structure(PHG4Reco *g4Reco, double cone_theta_deg, double barrel_4_r, double z_disk_f){
	AllSi_Al_support_Subsystem *Al_supp = new AllSi_Al_support_Subsystem("Al_supp",cone_theta_deg,barrel_4_r, z_disk_f);
	g4Reco->registerSubsystem(Al_supp);
}
// ======================================================================================================================
#endif
