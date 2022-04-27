//-----------------------------------------------------------
// File and Version Information:
// $Id$
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

// Class Member definitions -----------


CeeTpcPadPlane::CeeTpcPadPlane():PadReferences(),fnSectors(0),
fSectorLists(),fPadId(),fRowMin0LocalZ(),fRowPitch(),fPadPitch()
{
}

CeeTpcPadPlane::CeeTpcPadPlane(const char* const filename)
  : fnSectors(0)
{
  ReadFromFile(filename);
}

void CeeTpcPadPlane::GetPadList(const double AvX, const double AvZ, const double range,
			std::vector<CeeTpcPad*>& buffer) const {

 // equation of line at 45 degree z-z1 = m(x-x1) ; m =1, (x1,z1) = (0,-35) target
  double localZTarget, localX, localZ;
  int iSector =-1;
  // Finding Sectors (0,1) for the Avalanche Position
  if(AvX>10 && AvX<58.62){
  if (AvZ>-44.7135 && AvZ<AvX-35) iSector = 0;
  else if (AvZ>AvX-35 && AvZ<45 ) iSector = 1;
  }
  // Finding Sectors (2,3) for the Avalanche Position
  if(AvX>-58.62 && AvX<-10){
  if (AvZ>-AvX-35 && AvZ<45 ) iSector = 2;
  else if (AvZ>-44.7135 && AvZ<-AvX-35) iSector = 3;
  }
  if (iSector<0)  return;
 // Transfer from global to local coordinates
  localX = AvX; localZ = AvZ; // Sector 1&2
 if (iSector == 0){localX = -AvZ; localZ = AvX;}
 if (iSector == 3) {localX =  AvZ; localZ = -AvX;}
 if ((iSector == 0) || (iSector == 3))	localZTarget = 0;
 if ((iSector == 1) || (iSector == 2))	localZTarget = -35;
 int row = fabs(floor((localZ-fRowMin0LocalZ.at(iSector))/fRowPitch.at(iSector)));
  // Storing 9-Pads in the array
 int MaxRowInSector = fPadId.at(iSector).size();
 std::vector<Double_t> lz_arr;
 if (row>0) lz_arr.push_back(localZ-fRowPitch.at(iSector)); lz_arr.push_back(localZ);
 if (row<MaxRowInSector-1) lz_arr.push_back(localZ+fRowPitch.at(iSector));
 CeeTpcPad* aPad;
 for (Int_t iRow =0; iRow<lz_arr.size(); iRow++){
 localZ = lz_arr[iRow];
 row = fabs(floor((localZ-fRowMin0LocalZ.at(iSector))/fRowPitch.at(iSector)));
 double localZInPad = localZ - fRowMin0LocalZ.at(iSector) - row*fRowPitch.at(iSector);
 double pad0MinLocalXAtLocalZ = (fPad0MinLocalXAtMinLocalZ.at(iSector)).at(row)*(fRowPitch.at(iSector) -localZInPad)+(fPad0MinLocalXAtMaxLocalZ.at(iSector)).at(row)*localZInPad;
 pad0MinLocalXAtLocalZ = pad0MinLocalXAtLocalZ/fRowPitch.at(iSector);
 double padPitchAtLocalZ = fPadPitch.at(iSector)*(localZ -localZTarget)/(fRowMin0LocalZ.at(iSector) + (row+0.5)*fRowPitch.at(iSector) - localZTarget);
 int padInRow = fabs(floor((localX - pad0MinLocalXAtLocalZ)/padPitchAtLocalZ));
 int MaxPadInRow = fPadId.at(iSector).at(row).size();
 if (padInRow>=MaxPadInRow-1) return; // Returning for the larger PadInRows numbers
 int padId = (((fPadId.at(iSector)).at(row)).at(padInRow));
 aPad= GetPad(padId);
 // Pushing the Pads in the same rows
 TVector2 centPos = aPad->GetGlobalCenterPosition(); // Get the Center coordinates of the fired Pad
 for (Int_t i = padId-1; i<padId+2; i++){ // Pushing the neighboruing Pads of the same row
	 if (i == -1) continue;
	 aPad = GetPad(i);
	 TVector2 diff_Pos =(aPad->GetGlobalCenterPosition()-centPos)*0.1; // convert in cm
	 Double_t dist = sqrt(diff_Pos.X()*diff_Pos.X()+diff_Pos.Y()*diff_Pos.Y());
	 if (dist<range) buffer.push_back(aPad);
 }

}

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
  unsigned int ID=pad->GetPadId();
  try {
    GetPad(ID);
  }
  catch (unknown_padID& e)//the desired case
  {
    if(PadReferences.size()<=ID)PadReferences.resize(ID+1,0);
    PadReferences[ID]=pad;
    if(fSectorLists[pad->GetSectorId()]==0)fSectorLists[pad->GetSectorId()]=new std::map<unsigned int, CeeTpcPad*>;
    (*fSectorLists[pad->GetSectorId()])[pad->GetPadId()]=pad;
    return;
  }
  catch (superior_unknown_padId& e)//the desired case
  {
    if(PadReferences.size()<=ID)PadReferences.resize(ID+1,0);
    PadReferences[ID]=pad;
    if(fSectorLists[pad->GetSectorId()]==0)fSectorLists[pad->GetSectorId()]=new std::map<unsigned int, CeeTpcPad*>;
    (*fSectorLists[pad->GetSectorId()])[pad->GetPadId()]=pad;
    return;
  }
  throw used_padID();
}

void CeeTpcPadPlane::ReadFromFile(const char* const filename)
{
  bool noproblem = true;
  std::ifstream infile(filename, std::fstream::in);
  Warning("CeeTpcPadPlane::ReadFromFile","Reading file %s",filename);
  if (!infile.good())
    Fatal("CeeTpcPadPlane::ReadFromFile","The file %s can not be found.",filename);
  while (infile.good() && noproblem)
  {
    char line[265];
    // the size of the array means input cannot cross this size (265) from text file
    infile.getline(line, 265);
    noproblem = ReadLine(line);
  }
  infile.close();
  if (noproblem == false)
    Fatal("CeeTpcPadPlane::ReadFromFile","The file %s does not correspond to the required format.",filename);
}

bool CeeTpcPadPlane::ReadLine(char* const line)
{
  //remove comments at end of line
  char* com=strchr(line, '*');//comment
  if (com != 0)
    *com = '\0';
  if (line[0] == '\0') return(true);
  //Is either read by the right constructor or ignored
  if (strstr(line,"PadID")!= 0) return(true);
  char* rest;
  unsigned int PadID, SectorID, Row, PadInRow;
  double gCentX, gCentZ, gx0, gx1, gx2, gx3, gz0, gz1, gz2, gz3;
  double lCentX, lCentZ, lx0, lx1, lx2, lx3, lz0, lz1, lz2, lz3;
  double Angle = 0.;
  double gx[4], gz[4], lx[4], lz[4];
  double localXmin, localXmax;

  PadID = (unsigned int) strtoul(line, &rest, 10);
  SectorID = (unsigned int)strtod(rest,&rest);
  Row = (unsigned int)strtod(rest,&rest);
  PadInRow = (unsigned int)strtod(rest,&rest);

// Coordinates are multiplied by 10 convert in mm
  gCentX =10*strtod(rest,&rest);  gCentZ = 10*strtod(rest,&rest);
  gx0 = 10*strtod(rest,&rest);  gz0 = 10*strtod(rest,&rest);
  gx1 = 10*strtod(rest,&rest);  gz1 = 10*strtod(rest,&rest);
  gx2 = 10*strtod(rest,&rest);  gz2 = 10*strtod(rest,&rest);
  gx3 = 10*strtod(rest,&rest);  gz3 = 10*strtod(rest,&rest);

  gx[0] = gx0; gx[1] = gx1; gx[2] = gx2; gx[3] = gx3;
  gz[0] = gz0; gz[1] = gz1; gz[2] = gz2; gz[3] = gz3;

  lCentX = 10*strtod(rest,&rest);  lCentZ = 10*strtod(rest,&rest);
  lx0 = 10*strtod(rest,&rest);  lz0 = 10*strtod(rest,&rest);
  lx1 = 10*strtod(rest,&rest);  lz1 = 10*strtod(rest,&rest);
  lx2 = 10*strtod(rest,&rest);  lz2 = 10*strtod(rest,&rest);
  lx3 = 10*strtod(rest,&rest);
  char* end;
  lz3 = 10*strtod(rest,&end);

  lx[0] = lx0; lx[1] = lx1; lx[2] = lx2; lx[3] = lx3;
  lz[0] = lz0; lz[1] = lz1; lz[2] = lz2; lz[3] = lz3;

  // RowPitch and PadPitch
  fRowPitch[0] = 1.43; fRowPitch[1] = 1.25; fRowPitch[2] = 1.25; fRowPitch[3] = 1.43;
  fPadPitch[0] = 0.5; fPadPitch[1] = 0.4; fPadPitch[2] = 0.4; fPadPitch[3] = 0.5;
  fPadId[SectorID][Row][PadInRow] = PadID;

  if(Row==0) fRowMin0LocalZ[SectorID] = lz0*0.1; // converted in to cm
  if(PadInRow==0){
	if (SectorID==0){localXmin = std::min(lx0*0.1,lx1*0.1); localXmax = std::max(lx0*0.1,lx1*0.1);}
	else if (SectorID==1){localXmin = std::min(lx0*0.1,lx1*0.1); localXmax = std::max(lx0*0.1,lx1*0.1);}
	else if (SectorID==2){localXmin = std::max(lx0*0.1,lx1*0.1); localXmax = std::min(lx0*0.1,lx1*0.1);}
	else if (SectorID==3){localXmin = std::max(lx0*0.1,lx1*0.1); localXmax = std::min(lx0*0.1,lx1*0.1);}
	fPad0MinLocalXAtMinLocalZ[SectorID][Row] = localXmin;
	fPad0MinLocalXAtMaxLocalZ[SectorID][Row] = localXmax;
  }
  if (rest == line)//No reading happened (e.g. only blanks)
    return(true);
  if (rest == end)//It was impossible to read angle
  {  
    Warning("CeeTpcPadPlane::ReadLine","In line: %s \n Too less arguments.",line);
    return(false);
  }

  CeeTpcPad* pad = new CeeTpcPad(gx0, gz0, gx1, gz1, gx2, gz2, gx3, gz3, SectorID, PadID, Angle);

  if (!pad)
  {
    Warning("CeeTpcPadPlane::ReadLine","Pad can't be added check text file!!!");
    return(false);
  }
  //==setting Pad Properties using Modifiers======
  pad->SetRowId(Row);
  pad->SetPadInRowId(PadInRow);
  pad->SetGlobalCoordinates(gx,gz);
  pad->SetLocalCoordinates(lx,lz);
  pad->SetGlobalCenterPosition(gCentX, gCentZ);
  pad->SetLocalCenterPosition(lCentX, lCentZ);

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

ClassImp(CeeTpcPadPlane)


