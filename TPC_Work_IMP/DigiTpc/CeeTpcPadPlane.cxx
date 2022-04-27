//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Implementation of class CeeTpcPadPlane
//      see CeeTpcPadPlane.h for details
//
// Environment:
//      Software developed for the PANDA Detector at FAIR.
//
// Author List:
//      Sebastian Neubert    TUM            (original author)
//
//
//-----------------------------------------------------------

// This Class' Header ------------------
#include "CeeTpcPadPlane.h"

// C/C++ Headers ----------------------
#include <iostream>
#include <exception>
#include <fstream>
#include "string.h"
#include "stdlib.h"
// Collaborating Class Headers --------
#include "TError.h"
#include "TString.h"
#include "CeeTpcPadShapePool.h"
#include "CeeTpcAbsPadShape.h"

// Class Member definitions -----------


CeeTpcPadPlane::CeeTpcPadPlane(int fnx, 
			 int fnz, 
			 double fxbin,
			 double fzbin,
			 double fx0,
			 double fz0,
			 double fxmax_padplane,
			 double fxmax_pad)
  : nx(fnx),nz(fnz),xbin(fxbin),zbin(fzbin),x0(fx0),z0(fz0), fnSectors(0),
	Indexer(fnx,fnz,fxbin,fzbin,fx0,fz0,fxmax_padplane,fxmax_pad)
{
  int nRegions=nx*nz;
  for(int iRegion=0; iRegion<nRegions; ++iRegion){
    Regions.push_back(new std::vector<CeeTpcPad*>);
  }
}

CeeTpcPadPlane::CeeTpcPadPlane(const char* const filename,
			 const CeeTpcPadShapePool* const pspool)
  : fnSectors(0), Indexer(0,0,0.,0.,0.,0.,0.,0.)
{
  if(pspool==NULL)std::cout<<"WARNING: no padshapes defined!"
			   <<" PadPlane will not be usable!"<<std::endl;
  //open file to construct Indexer
  std::ifstream infile(filename, std::fstream::in);
  TString meh(filename);
  Warning("CeeTpcPadPlane::CeeTpcPadPlane","Reading file...");
  if (!infile.good())
    Fatal("CeeTpcPadPlane::CeeTpcPadPlane","The file can not be found.");
  //search the file for the line describing the Indexer
  while (infile.good())
  {
    char line[256];
    infile.getline(line, 256);
    //cancel comments
    char* com=strchr(line, '*');
    if (com != 0)
      *com = '\0';
    //the line has to begin with "PadPlaneSubDivision"
    char* ppSub = strstr(line, "PadPlaneSubDivision");
    if (ppSub == 0) 
      continue;
    //reading the values
    ppSub = &(ppSub[strlen("PadPlaneSubDivision")]);
    char* rest;
    int fnx = (int) strtol(ppSub, &rest, 10);
    int fnz = (int) strtol(rest, &rest, 10);
    double fxbin = strtod(rest,&rest);
    double fzbin = strtod(rest,&rest);
    double fx0 = strtod(rest,&rest);
    double fz0 = strtod(rest,&rest);
    double fxmax_padplane = strtod(rest,&rest);
    char* end;
    double fxmax_pad = strtod(rest,&end);
    if (rest == end)//It was impossible to read double fz0
      Fatal("CeeTpcPadPlane::CeeTpcPadPlane","Too less arguments in input file.");

    //Create the Indexer and the regions (like in  the simple constructor)
    Indexer.nx = nx =fnx; // =100;
    Indexer.nz = nz =fnz; // =90
    Indexer.xbin = xbin =fxbin; //=1
    Indexer.zbin = zbin =fzbin; //=1
    Indexer.x0 = x0 =fx0; // =0
    Indexer.z0 = z0 =fz0; //=0
    Indexer.xmax_padplane = xmax_padplane =fxmax_padplane; //=0
    Indexer.xmax_pad = xmax_pad =fxmax_pad; //=0
    int nRegions=nx*nz; // Total 100*90 = 9000 regions of 1x1 cm^2
    for(int iRegion=0; iRegion<nRegions; ++iRegion)
    Regions.push_back(new std::vector<CeeTpcPad*>); // Assigning a separate pointer for each 9000 regions
    infile.unget();//if it was the last line=>eof=>without this !infile.good()
    break;
  }
  if (!infile.good())//e.g. eof
    Fatal("CeeTpcPadPlane::CeeTpcPadPlane","Error searching a line of the form:\n PadPlaneSubDivision int int double double double double \n Pad Plane can't be build.");
  infile.close();
  ReadFromFile(filename, pspool);
}

// PadPlane Takes Ownership over Pads!
CeeTpcPadPlane::~CeeTpcPadPlane(){
  int n=PadReferences.size();
  for(int i=0;i<n;++i){
    delete PadReferences[i];
  }
  for(int ir=0; ir<nx*nz; ++ir){
    delete Regions[ir];
  }
}

void CeeTpcPadPlane::GetPadList(const double x,
			const double z, 
			const double r, 
			std::vector<CeeTpcPad*>& buffer) const {
  
  for(int iz=-1; iz<=1; ++iz){
    for(int ix=-1; ix<=1; ++ix){
      double xin = x+ix*xbin;
      double zin = z+iz*zbin;
      if (xin<x0 || xin>nx*xbin+x0 || zin<z0 ||  zin>nz*zbin+z0) continue;
      //Get Regions
      int iRegion=Indexer(xin,zin);
      int nPads=Regions[iRegion]->size();
      for(int iPad=0; iPad<nPads; ++iPad){
	//Look which Pads lz inside range
	CeeTpcPad* aPad=(*Regions[iRegion])[iPad];
	if(aPad->CircleIntersection(x,z,r))buffer.push_back(aPad);
      }
    }
  }
}

void CeeTpcPadPlane::GetPadXZ(const unsigned int PadID,
		      double& x, double& z) const
{
  CeeTpcPad* pad=GetPad(PadID);
  x=pad->x();
  z=pad->z();
}

CeeTpcPad* CeeTpcPadPlane::GetPad(const unsigned int PadID) const
{
  if (PadID >= PadReferences.size()) throw superior_unknown_padId(PadID);
  CeeTpcPad* pad=PadReferences[PadID];
  if(pad==NULL) throw superior_unknown_padId(PadID);
  return(pad);
}

unsigned int CeeTpcPadPlane::GetNPads() const
{
  unsigned int np=0;
  for (unsigned int i=0; i<PadReferences.size(); ++i)
    if (PadReferences[i] != 0)
      np++;
  return(np);
}

// Pad Plane will take Ownership!
void CeeTpcPadPlane::AddPad(CeeTpcPad* pad)
{
  //std::cout<<*pad<<std::endl;
  int i=Indexer(pad);
  try {
    Regions.at(i)->push_back(pad);
    fSectors[pad->sectorId()].minx=std::min(fSectors[pad->sectorId()].minx,pad->x());
    fSectors[pad->sectorId()].minz=std::min(fSectors[pad->sectorId()].minz,pad->z());
    fSectors[pad->sectorId()].maxx=std::max(fSectors[pad->sectorId()].maxx,pad->x());
    fSectors[pad->sectorId()].maxz=std::max(fSectors[pad->sectorId()].maxz,pad->z());
    // this assumes continually nummbered sectors:
    if(pad->sectorId()>=fnSectors)fnSectors=pad->sectorId()+1;
  } 
  catch (std::exception& e) {
    std::cerr<<"Index="<<i<<std::endl;
    std::cerr<<e.what()<<std::endl;
    throw;
  }

  unsigned int ID=pad->id();
  try {
    GetPad(ID);
  }
  catch (unknown_padID& e)//the desired case
  {
    if(PadReferences.size()<=ID)PadReferences.resize(ID+1,0);
    PadReferences[ID]=pad;
    if(fSectorLists[pad->sectorId()]==0)fSectorLists[pad->sectorId()]=new std::map<unsigned int, CeeTpcPad*>;
    (*fSectorLists[pad->sectorId()])[pad->padId()]=pad;
    return;
  }
  catch (superior_unknown_padId& e)//the desired case
  {
    if(PadReferences.size()<=ID)PadReferences.resize(ID+1,0);
    PadReferences[ID]=pad;
    if(fSectorLists[pad->sectorId()]==0)fSectorLists[pad->sectorId()]=new std::map<unsigned int, CeeTpcPad*>;
    (*fSectorLists[pad->sectorId()])[pad->padId()]=pad;
    return;
  }
  //the padID is not unknown=> some other pad has this ID
  Regions.at(i)->pop_back();
  throw used_padID();
}

void CeeTpcPadPlane::ReadFromFile(const char* const filename,
			  const CeeTpcPadShapePool* const pspool)
{
  bool noproblem = true;
  std::ifstream infile(filename, std::fstream::in);
  Warning("CeeTpcPadPlane::ReadFromFile","Reading file %s",filename);
  if (!infile.good())
    Fatal("CeeTpcPadPlane::ReadFromFile","The file %s can not be found.",filename);
  while (infile.good() && noproblem)
  {
    char line[256];
    infile.getline(line, 256);
    noproblem = ReadLine(line, pspool);
  }
  // if (!infile.eof())
  // Fatal("CeeTpcPadPlane::ReadFromFile","The file %s can not be read.",filename);
  infile.close();
  if (noproblem == false)
    Fatal("CeeTpcPadPlane::ReadFromFile","The file %s does not correspond to the required format.",filename);
}

bool CeeTpcPadPlane::ReadLine(char* const line,
		      const CeeTpcPadShapePool* const pspool)
{
  //remove comments at end of line
  char* com=strchr(line, '*');//comment
  if (com != 0)
    *com = '\0';
  if (line[0] == '\0')
    return(true);
  //Is either read by the right constructor or ignored
  if (strstr(line, "PadPlaneSubDivision") != 0)
    return(true);
  char* rest;
  unsigned int ID;
  unsigned int sectorID;
  unsigned int shapeID;
  double x;
  double z;
  double angle;
  ID = (unsigned int) strtoul(line, &rest, 10);
  sectorID = (unsigned int) strtoul(rest, &rest, 10);
  shapeID = (unsigned int) strtoul(rest, &rest, 10);
  x = strtod(rest,&rest);
  z = strtod(rest,&rest);
  char* end;
  angle = strtod(rest,&end);
  if (rest == line)//No reading happened (e.g. only blanks)
    return(true);
  if (rest == end)//It was impossible to read angle
  {  
    Warning("CeeTpcPadPlane::ReadLine","In line: %s \n Too less arguments.",line);
    return(false);
  }

  CeeTpcAbsPadShape* shape=0; 
  if(pspool!=NULL){
    shape=pspool->GetPadShape(shapeID);
    if (shape == 0)//shapeId does not exist
      {
	Warning("CeeTpcPadPlane::ReadLine","In line: %s \n There is no CeeTpcPadShape with ID= %i",line,shapeID);
	return(false);
      }
  }
  

  CeeTpcPad* pad = new CeeTpcPad(x, z, angle,
			   shape, sectorID, ID);
  // read in neighbour ids if available
  char* end2;
  unsigned int nid= (unsigned int) strtoul(end, &end2, 10);
  while(end!=end2){
    pad->addNeighbour(nid);
    end=end2;
    nid= (unsigned int) strtoul(end, &end2, 10);
  }
  try {
    AddPad(pad);
  }
  catch (std::exception& e)
  {
    Warning("CeeTpcPadPlane::ReadLine","The pad from line: %s \ncould not be added.Reason: \n %s",line,e.what());
    return(false);
  }
  return(true);
}

 

std::vector<unsigned int> CeeTpcPadPlane::GetSectorIds()const {
  std::map<unsigned int,std::map<unsigned int,CeeTpcPad*>*>::const_iterator sIt=fSectorLists.begin();
  std::vector<unsigned int> ids;
  while(sIt!=fSectorLists.end()){
    ids.push_back(sIt->first);
    ++sIt;
  }
  return ids;
}




// Operators
std::ostream& operator<< (std::ostream& s, const CeeTpcPadPlane& me){
  return s << "CeeTpcPadPlane:\n"
	   << "      nx="<<me.GetNX()
	   << "      nz="<<me.GetNZ()<<"\n"
	   << "      nRegions="<<me.Regions.size()<<"\n"
	   << "      Covered Area: ("<<me.GetX0()<<","<<me.GetZ0()
	   << ")->("<<me.GetNX()*me.GetXBin()+me.GetX0()
	   << ","<<me.GetNZ()*me.GetZBin()+me.GetZ0()<< ")\n"
	   << "      nPads="<<me.PadReferences.size()<<"\n";
}

void CeeTpcPadPlane::WriteToStream(std::ostream& stream) const {
  stream << "PadPlaneSubDivision "
	 <<GetNX()<<" "
	 <<GetNZ()<<" "
	 <<GetXBin()<<" "
	 <<GetZBin()<<" "
	 <<GetX0()<<" "	 
	 <<GetZ0()<<std::endl;

  unsigned int npads=GetNPads();
  for(unsigned int ipad=0;ipad<npads;++ipad){
    CeeTpcPad* pad=GetPad(ipad);
    stream << pad->id() <<" "
	   << pad->sectorId() <<" "
	   << pad->shape()->GetID()<<" "
	   << pad->x()<<" "
	   << pad->z()<<" "
	   << pad->angle()<<" ";
          
    unsigned int nn=pad->nNeighbours();
    for(unsigned int in=0;in<nn;++in){
      stream << pad->getNeighbour(in) <<" ";
    }

    stream << std::endl;
  }
  
  stream.flush();
}

ClassImp(CeeTpcPadPlane)

