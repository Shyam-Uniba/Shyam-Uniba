//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
//-----------------------------------------------------------

// This Class' Header ------------------
#include "CeeTpcPadPlane_Trap.h"
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


CeeTpcPadPlane_Trap::CeeTpcPadPlane_Trap():PadReferences(),fnSectors(0),
fSectorLists(),fPadId(),fRowMin0LocalZ(),fRowPitch(),fPadPitch()
{
}

CeeTpcPadPlane_Trap::CeeTpcPadPlane_Trap(const char* const filename)
  : fnSectors(0)
{
  ReadFromFile(filename);
}

CeeTpcPad_Trap* CeeTpcPadPlane_Trap::GetPad(const unsigned int PadID) const
{
  if (PadID >= PadReferences.size()) throw superior_unknown_padId(PadID);
  CeeTpcPad_Trap* pad=PadReferences[PadID];
  if(pad==NULL) throw superior_unknown_padId(PadID);
  return(pad);
}

unsigned int CeeTpcPadPlane_Trap::GetNPads() const
{
  unsigned int np=0;
  for (unsigned int i=0; i<PadReferences.size(); ++i)
    if (PadReferences[i] != 0)
      np++;
  return(np);
}

// Pad Plane will take Ownership!
void CeeTpcPadPlane_Trap::AddPad(CeeTpcPad_Trap* pad)
{
  unsigned int ID=pad->GetPadId();
  try {
    GetPad(ID);
  }
  catch (unknown_padID& e)//the desired case
  {
    if(PadReferences.size()<=ID)PadReferences.resize(ID+1,0);
    PadReferences[ID]=pad;
    if(fSectorLists[pad->GetSectorId()]==0)fSectorLists[pad->GetSectorId()]=new std::map<unsigned int, CeeTpcPad_Trap*>;
    (*fSectorLists[pad->GetSectorId()])[pad->GetPadId()]=pad;
    return;
  }
  catch (superior_unknown_padId& e)//the desired case
  {
    if(PadReferences.size()<=ID)PadReferences.resize(ID+1,0);
    PadReferences[ID]=pad;
    if(fSectorLists[pad->GetSectorId()]==0)fSectorLists[pad->GetSectorId()]=new std::map<unsigned int, CeeTpcPad_Trap*>;
    (*fSectorLists[pad->GetSectorId()])[pad->GetPadId()]=pad;
    return;
  }
  throw used_padID();
}

void CeeTpcPadPlane_Trap::ReadFromFile(const char* const filename)
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

bool CeeTpcPadPlane_Trap::ReadLine(char* const line)
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
  fRowPitch[0] = 14.3; fRowPitch[1] = 12.5; fRowPitch[2] = 12.5; fRowPitch[3] = 14.3;
  fPadPitch[0] = 5.0; fPadPitch[1] = 4.0; fPadPitch[2] = 4.0; fPadPitch[3] = 5.0;
  fPadId[SectorID][Row][PadInRow] = PadID;
  if(SectorID == 0 && Row==0) fRowMin0LocalZ[SectorID] = lz0;
  if(SectorID == 1 && Row==0) fRowMin0LocalZ[SectorID] = lz0;
  if(SectorID == 2 && Row==0) fRowMin0LocalZ[SectorID] = lz0;
  if(SectorID == 3 && Row==0) fRowMin0LocalZ[SectorID] = lz0;


  if (rest == line)//No reading happened (e.g. only blanks)
    return(true);
  if (rest == end)//It was impossible to read angle
  {  
    Warning("CeeTpcPadPlane::ReadLine","In line: %s \n Too less arguments.",line);
    return(false);
  }

  CeeTpcPad_Trap* pad = new CeeTpcPad_Trap(gx0, gz0, gx1, gz1, gx2, gz2, gx3, gz3, SectorID, PadID, Angle);

  if (!pad)
  {
    Warning("CeeTpcPadPlane_Trap::ReadLine","Pad can't be added check text file!!!");
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

ClassImp(CeeTpcPadPlane_Trap)


