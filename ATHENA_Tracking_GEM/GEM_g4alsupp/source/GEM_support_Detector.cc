#include "GEM_support_Detector.h"
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
GEM_support_Detector::GEM_support_Detector(PHG4Subsystem *subsys,
		PHCompositeNode *Node,
		PHParameters *parameters,
		const std::string &dnam)
	: PHG4Detector(subsys, Node, dnam)
	  , m_Params(parameters)
{
}
//_______________________________________________________________
int GEM_support_Detector::IsInDetector(G4VPhysicalVolume *volume) const
{
	set<G4VPhysicalVolume *>::const_iterator iter = m_PhysicalVolumesSet.find(volume);
	if (iter != m_PhysicalVolumesSet.end())
	{
		return 1;
	}
	return 0;
}
//_______________________________________________________________
void GEM_support_Detector::ConstructMe(G4LogicalVolume *logicWorld)
{
	//begin implement your own here://
	// Do not forget to multiply the parameters with their respective CLHEP/G4 unit !
	addDetectorSection( logicWorld , "Al_pos_z" ,  1. );
	addDetectorSection( logicWorld , "Al_neg_z" , -1. );
	//end implement your own here://
	return;
}
// ======================================================================================================
void GEM_support_Detector::Print(const std::string &what) const
{
	cout << "GEM_support_ Detector:" << endl;
	if (what == "ALL" || what == "VOLUME")
	{
		cout << "Version 0.1" << endl;
		cout << "Parameters:" << endl;
		m_Params->Print();
	}
	return;
}
// ======================================================================================================
void  GEM_support_Detector::addDetectorSection( G4LogicalVolume *logicWorld , std::string name , double sign){

	//double z_det[3] = {16.8, 69, 121.};
        //double rin  [3] = {13.38, 50.75, 85.20};
	
	//First Pass projective
	//double z_det[3] = {16.8, 69, 143.};
        //double rin  [3] = {13.38, 50.75, 105.08};
	
	//Symmetric Cone Only
	//double z_det[2] = {16.8, 130.};
        //double rin  [2] = {13.38, 94.0};
	//double rout [2] = {0};
	
	double z_det[3] = {0, 0, 0};
        double rin  [3] = {0, 0, 0};
	
	//double z_det_temp[6] = {16.8, 130., 130.5, -16.8, -130, -130.5};
        //double rin_temp  [6] = {13.38, 94.0, 94.0, 13.38, 94.0, 94.0};
	
	//Asymmetric
	//double z_det_temp[6] = {16.8, 130., 153., -16.8, -130, -198.};
        //double rin_temp  [6] = {13.38, 94.0, 94.0, 13.38, 94.0, 94.0};

	//Symmetric B2P2N2, projective 1.1
	//double z_det_temp[6] = {16.8, 130., 170., -16.8, -130, -150.};
        //double rin_temp  [6] = {13.38, 94.0, 94.0, 13.38, 94.0, 94.0};
	//asymmetric fixed
	//double z_det_temp[6] = {16.8, 125., 170., -16.8, -125, -150.};
        //double rin_temp  [6] = {12.58, 93.59, 93.59, 12.58, 93.59, 93.59};
	
	//Baseline 2
	double z_det_temp[6] = {16.8, 58.42, 165., -16.8, -58.42, -145.};
        double rin_temp  [6] = {12.58, 43.23, 43.23, 12.58, 43.23, 43.23};
	
	
	double rout [3] = {0};
	
	const int nzplanes = sizeof(z_det)/sizeof(*z_det);
	//for(int i = 0 ; i < nzplanes ; i++){
	for(int i = 0 ; i < 3 ; i++){
		int index = i + (1-sign)*1.5;
		rout[i] = rin_temp[index] + 0.2;
		z_det[i] = z_det_temp[index]*10.;
		rin  [i] = rin_temp[index]*10.;
		rout [i] = rout[i]*10.;
		
		//rout[i] = rin[i] + 0.5;
		//z_det[i] *= 10.*sign/abs(sign);
		//rin  [i] *= 10.;
		//rout [i] *= 10.;
	}

	G4Material * G4_mat = G4Material::GetMaterial("G4_Al");

	G4RotationMatrix *rotm = new G4RotationMatrix();
	rotm->rotateX(0);
	rotm->rotateY(0);
	rotm->rotateZ(0);

	G4Color G4_color = G4Color(G4Colour::Yellow());

	G4VSolid *G4_polycone = new G4Polycone(name,0,360*degree,nzplanes,z_det,rin,rout);
	G4LogicalVolume *logical = new G4LogicalVolume(G4_polycone,G4_mat, "GEM_support_Logical");
	G4VisAttributes *vis_1 = new G4VisAttributes(G4_color);
	vis_1->SetForceSolid(true);
	logical->SetVisAttributes(vis_1);

	G4VPhysicalVolume *phy_1 = new G4PVPlacement(rotm, G4ThreeVector(0,0,0), logical , "GEM_support_", logicWorld, 0, false, OverlapCheck());

	// add it to the list of placed volumes so the IsInDetector method picks them up
	m_PhysicalVolumesSet.insert(phy_1);
}
