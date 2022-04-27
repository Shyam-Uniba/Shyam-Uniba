#include <vector>
#include <fstream>

void createPadPlane(){
// prepare file
std::ofstream outfile("padplane.dat");

// subdivisions:
int nx=84; // Total 84 cm (-42->42)
int nz=84; // Total 84 cm (-42->42)
double sizex=1.;
double sizez=1.;
double cornerx=-42.;
double cornerz=-42.;

//First Line
outfile<<"PadPlaneSubDivision "
<<nx<<" "<<nz<<" "<<sizex<<" "<<sizez<<" "<<cornerx<<" "<<cornerz<<std::endl;

// define pad-layout (Pad size 2mmx2mm)
double stepx=0.2;
double stepz=0.2;

// define sectors (for now as rectangles)
int sectnx=16;
int sectnz=16;

double x=cornerx;
double z=cornerz;

int nxsteps=(int)floor(nx*sizex/stepx);
int nzsteps=(int)floor(nz*sizez/stepz);

// reduce number of pads to multiple of sectorsize
nxsteps=nxsteps-nxsteps%sectnx; // Remove fractional pads
nzsteps=nzsteps-nzsteps%sectnz; // Reamove fractional pads

cout<<"Number of steps"<<nxsteps%sectnx<<endl;

assert(nxsteps%sectnx==0);
assert(nzsteps%sectnz==0);

int nsectorsx=nxsteps/sectnx; // No. of pads in each sectors X-direction = 26;
int nsectorsz=nzsteps/sectnz; // No. of pads in each sectors in Z-direction = 26;
int nsectors=nsectorsx*nsectorsz; // Total pads in one sector 26*26=676;
//Total Pads = 16*16*676=173056

int shapeId=0;

unsigned int id=0;

// build up padplane from sectors
for(int isz=0;isz<nsectorsz;++isz){
  for(int isx=0;isx<nsectorsx;++isx){
    double sectorx=isx*sectnx*stepx+cornerx;
    double sectorz=isz*sectnz*stepz+cornerz;
    double sectorfarx=sectorx+sectnx*stepx;
    double sectorfarz=sectorz+sectnz*stepz;
    double r1=sqrt(sectorx*sectorx+sectorz*sectorz);
    double r2=sqrt(sectorfarx*sectorfarx + sectorfarz*sectorfarz);
    double r3=sqrt(sectorx*sectorx+sectorfarz*sectorfarz);
    double r4=sqrt(sectorfarx*sectorfarx + sectorz*sectorz);
  //  if sector not inside tpc continue;
    if(max(max(r1,r2),max(r3,r4))<15)continue;
    if(min(min(r1,r2),min(r3,r4))>42)continue;

    x=sectorx;
    z=sectorz;
    for(int iz=0;iz<sectnx;++iz){
	for(int ix=0;ix<sectnz;++ix){
	  //determine neighbours
	  std::vector<unsigned int> neighb;
	  // check left col
	  bool left=ix>0;
	  // check right col
	  bool right=ix<sectnx-1;
	  // check bottom row
	  bool bottom=iz>0;
	  // check top row
	  bool top=iz<sectnz-1;
	  
	  if(left)neighb.push_back(id-1);
	  if(right)neighb.push_back(id+1);
	  if(bottom)neighb.push_back(id-sectnx);
	  if(top)neighb.push_back(id+sectnx);
	  if(left && bottom)neighb.push_back(id-sectnx-1);
	  if(left && top)neighb.push_back(id+sectnx-1);
	  if(right && bottom)neighb.push_back(id-sectnx+1);
	  if(right && top)neighb.push_back(id+sectnx+1);

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
	}
	z+=stepz;
	x=sectorx;
      }
  }
}

outfile.close();

}

