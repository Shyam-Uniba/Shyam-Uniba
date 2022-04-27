#include <vector>
#include <fstream>

void createPadPlane_CEE_TPC(){
// prepare file
std::ofstream outfile("padplane_cee_tpc.dat");

// subdivisions:
int nx=120; // Total 120 cm in X-direction (-60->60)
int nz=90; // Total 90 cm in Z-direction (-45->45)
double sizex=1.;
double sizez=1.;
double cornerx=-60.; // Start postion of plane
double cornerz=-45.;
// define region size
double region_size_x = 1.0; // 1 cm in x-region
double region_size_z = 1.2; // 1.2 cm in x-region

// define pad-layout (Pad size 5mmx12mm) // Realistic for CEE
double stepx=0.5;
double stepz=1.2;
// Total Regions = PadPlane_Area/Region_Area
outfile<<"PadPlaneSubDivision "
<<nx/region_size_x<<" "<<nz/region_size_z<<" "<<region_size_x<<" "<<region_size_z<<" "<<cornerx<<" "<<cornerz<<" "<<nx<<" "<<stepx<<std::endl;

// define sectors (for now as rectangles) consider only one sector in CEE
int sectnx=1;
int sectnz=1;

double x=cornerx;
double z=cornerz;

int nxsteps=(int)floor(nx*sizex/stepx);
int nzsteps=(int)floor(nz*sizez/stepz);
// reduce number of pads to multiple of sectorsize
nxsteps=nxsteps-nxsteps%sectnx; // Remove fractional pads for larger than one sector
nzsteps=nzsteps-nzsteps%sectnz; // Reamove fractional pads for larger than one sectors
//cout<<"Number of steps"<<nxsteps%sectnx<<endl;

assert(nxsteps%sectnx==0);
assert(nzsteps%sectnz==0);

int nsectorsx=nxsteps/sectnx; // No. of pads in each sectors X-direction = 240;
int nsectorsz=nzsteps/sectnz; // No. of pads in each sectors in Z-direction = 75;
int nsectors=nsectorsx*nsectorsz; // Total pads in one sector 240*75=676;
//Total Pads = 240*75 = 18000
int shapeId=0;

unsigned int id=0;

// build up padplane from sectors
for(int isz=0;isz<nsectorsz;++isz){
  for(int isx=0;isx<nsectorsx;++isx){
    double sectorx=isx*sectnx*stepx+cornerx;
    double sectorz=isz*sectnz*stepz+cornerz;
    x=sectorx;
    z=sectorz;
	  //determine neighbours
	  std::vector<unsigned int> neighb;
	  // check: left neighbour will exist for x>0
	  bool left=isx>0;
	  // check: right neighbour will exist for x<sectnx-1
	  bool right=isx<nsectorsx-1;
	  // check: bottom neighbour will exist for z>0
	  bool bottom=isz>0;
	  // check: top neighbour will exist for z<sectnz-1
	  bool top=isz<nsectorsz-1;
	  
	  if(left)neighb.push_back(id-1);
	  if(right)neighb.push_back(id+1);
	  if(bottom)neighb.push_back(id-nsectorsx);
	  if(top)neighb.push_back(id+nsectorsx);
	  if(left && bottom)neighb.push_back(id-nsectorsx-1);
	  if(left && top)neighb.push_back(id+nsectorsx-1);
	  if(right && bottom)neighb.push_back(id-nsectorsx+1);
	  if(right && top)neighb.push_back(id+nsectorsx+1);

	  // determine sectorid
	  unsigned int sectorId=isz*nsectorsx+isx;
	  outfile<<id<<" "
		 <<sectorId<<" "
		 <<shapeId<<" "
		 <<x<<" "
		 <<z<<" "
		 <<0; // angle;
	  for(int inei=0;inei<neighb.size();++inei)outfile<<" "<<neighb[inei];
	  outfile<<std::endl;

	  x+=stepx;
	  ++id;
	z+=stepz;
	x=sectorx;
  }
}

outfile.close();

}

