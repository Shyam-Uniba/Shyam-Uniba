#include "AllSi_Al_support_Detector.h"
#include <phparameter/PHParameters.h>
#include <g4main/PHG4Detector.h>
#include <Geant4/G4Box.hh>
#include <Geant4/G4Polycone.hh>
#include <Geant4/G4Color.hh>
#include <Geant4/G4LogicalVolume.hh>
#include <Geant4/G4Material.hh>
#include <Geant4/G4PVPlacement.hh>
#include <Geant4/G4SystemOfUnits.hh>
#include <Geant4/G4VisAttributes.hh>
#include <cmath>
#include <iostream>

class G4VSolid;
class PHCompositeNode;
using namespace std;
//____________________________________________________________________________..
AllSi_Al_support_Detector::AllSi_Al_support_Detector(PHG4Subsystem *subsys,
		PHCompositeNode *Node,
		PHParameters *parameters,
		const std::string &dnam,
		double cone_theta_deg,
		double max_radius,
		double max_z
	)
	: PHG4Detector(subsys, Node, dnam)
	  , m_Params(parameters)
{
	cl_cone_theta_deg = cone_theta_deg;
	cl_max_radius = max_radius;
	cl_max_z = max_z;
}
//_______________________________________________________________
int AllSi_Al_support_Detector::IsInDetector(G4VPhysicalVolume *volume) const
{
	set<G4VPhysicalVolume *>::const_iterator iter = m_PhysicalVolumesSet.find(volume);
	if (iter != m_PhysicalVolumesSet.end())
	{
		return 1;
	}
	return 0;
}
//_______________________________________________________________
void AllSi_Al_support_Detector::ConstructMe(G4LogicalVolume *logicWorld)
{
	//begin implement your own here://
	// Do not forget to multiply the parameters with their respective CLHEP/G4 unit !
	addDetectorSection( logicWorld , "Al_pos_z" ,  1. );
	addDetectorSection( logicWorld , "Al_neg_z" , -1. );
	//end implement your own here://
	return;
}
// ======================================================================================================
void AllSi_Al_support_Detector::Print(const std::string &what) const
{
	cout << "AllSi_Al_support_ Detector:" << endl;
	if (what == "ALL" || what == "VOLUME")
	{
		cout << "Version 0.1" << endl;
		cout << "Parameters:" << endl;
		m_Params->Print();
	}
	return;
}
// ======================================================================================================
void  AllSi_Al_support_Detector::addDetectorSection( G4LogicalVolume *logicWorld , std::string name , double sign){

	double slope = tan(3.14159/180.*cl_cone_theta_deg);

	double z_det[3] = {0};
	double rin  [3] = {0};
	
	z_det[0] = 20.;
	rin  [0] = slope*z_det[0];
	rin  [1] = cl_max_radius;
	z_det[1] = rin[1]/slope;
	z_det[2] = cl_max_z;
	rin  [2] = rin[1];

	double rout [3] = {0};
	const int nzplanes = sizeof(z_det)/sizeof(*z_det);
	for(int i = 0 ; i < nzplanes ; i++){
		rout[i] = rin[i] + 0.5;
		z_det[i] *= 10.*sign/abs(sign);
		rin  [i] *= 10.;
		rout [i] *= 10.;
	}

	G4Material * G4_mat = G4Material::GetMaterial("G4_Al");

	G4RotationMatrix *rotm = new G4RotationMatrix();
	rotm->rotateX(0);
	rotm->rotateY(0);
	rotm->rotateZ(0);

	G4Color G4_color = G4Color(G4Colour::Yellow());

	G4VSolid *G4_polycone = new G4Polycone(name,0,360*degree,nzplanes,z_det,rin,rout);
	G4LogicalVolume *logical = new G4LogicalVolume(G4_polycone,G4_mat, "AllSi_Al_support_Logical");
	G4VisAttributes *vis_1 = new G4VisAttributes(G4_color);
	vis_1->SetForceSolid(true);
	logical->SetVisAttributes(vis_1);

	G4VPhysicalVolume *phy_1 = new G4PVPlacement(rotm, G4ThreeVector(0,0,0), logical , "AllSi_Al_support_", logicWorld, 0, false, OverlapCheck());

	// add it to the list of placed volumes so the IsInDetector method picks them up
	m_PhysicalVolumesSet.insert(phy_1);
}
