//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Implementation of class PndTpcPadShapePool
//      see PndTpcPadShapePool.h for details
//
// Environment:
//      Software developed for the PANDA Detector at FAIR.
//
// Author List:
//      Cristoforo Simonetto    TUM            (original author)
//
//
//-----------------------------------------------------------

// This Class' Header ------------------
#include "CeeTpcPadShapePool.h"

// C/C++ Headers ----------------------
#include <iostream>
#include <fstream>
#include "string.h"
#include "stdlib.h"

// Collaborating Class Headers --------
#include "CeeTpcAbsPadShape.h"
#include "CeeTpcPadShapeSquare.h"
#include "CeeTpcPadShapePolygon.h"
#include "CeeTpcPRLookupTable.h"
#include "TError.h"
// Class Member definitions -----------

CeeTpcPadShapePool::CeeTpcPadShapePool()
{;}

CeeTpcPadShapePool::CeeTpcPadShapePool(const char* const filename,
				 const CeeTpcGem& gem,
				 const double frange,      
				 const double fstep,
				 const double fintStep)
{
  ReadFromFile(filename);
  BuildLookupTable(gem, frange, fstep, fintStep);
}

// CTOR without GEMs will not build LookUpTable!!!
CeeTpcPadShapePool::CeeTpcPadShapePool(const char* const filename)
{
  ReadFromFile(filename);
  std::cout<<"CeeTpcPadShapePool: No lookup-table will be built!"<<std::endl;
}


CeeTpcPadShapePool::~CeeTpcPadShapePool()
{
  for(int i=0; i<references.size(); i++)
  {
    if (references[i] != 0)
      delete references[i];
  }
}

CeeTpcAbsPadShape*
CeeTpcPadShapePool::GetPadShape(const unsigned int fID) const
{
  if (references.size() <= fID)
    return(0);
  return(references[fID]);
}

void
CeeTpcPadShapePool::AddPadShape(CeeTpcAbsPadShape* padshape)
{
  if (TryAddingPadShape(padshape) == false)
    Fatal("CeeTpcPadShapePool::AddPadShape","The padshape could not be added, because the ID is already occupied.");
}

void
CeeTpcPadShapePool::RemovePadShape(const unsigned int fID)
{
  if (fID >= references.size() || references[fID] == 0)
  {
    Warning("CeeTpcPadShapePool::RemovePadShape","There is no CeeTpcAbsPadShape with the ID=%i",(int)fID);
    return;
  }
  delete references[fID];
  references[fID] = 0;
  if (fID == references.size()-1)
  {
    int i=fID;
    while (i>=0 && references[i] == 0)
      i--;
    references.resize(i+1,0);
  }    
}

void
CeeTpcPadShapePool::ReadFromFile(const char* const filename)
{
  //Reading the file
  bool noproblem = true;
  std::ifstream infile(filename, std::fstream::in);
  Warning("CeeTpcPadShapePool::ReadFromFile","Reading file %s",filename);
  if (!infile.good())
    Fatal("CeeTpcPadShapePool::ReadFromFile","The file %s can not be found.",filename);
  while(infile.good() && noproblem)
    noproblem = ReadLine(infile);
  if (!infile.eof())
    Fatal("CeeTpcPadShapePool::ReadFromFile","The file %s can not be read.",filename);
  infile.close();
  if (noproblem == false)
    Fatal("CeeTpcPadShapePool::ReadFromFile","The file %s does not correspond to the required format.",filename);
}

void
CeeTpcPadShapePool::BuildLookupTable(const CeeTpcGem& gem,
				  const double frange,      
				  const double fstep,
				  const double fintStep){
  for (int i=0; i<references.size(); i++)
    if (references[i] != 0 && !references[i]->GetLookupTable()->IsBuilt())
      references[i]->GetLookupTable()->BuildTable(gem, *references[i],
						  frange, fstep, fintStep);
}

bool
CeeTpcPadShapePool::TryAddingPadShape(CeeTpcAbsPadShape* padshape)
{
  unsigned int id = padshape->GetID();
  if (references.size() > id && references[id] != 0)
    return(false);
  else if (references.size() <= id)
    references.resize(id+1, 0);
  references[id]=padshape;
  return(true);
}

bool
CeeTpcPadShapePool::ReadLine(std::ifstream& infile)
{
  CeeTpcAbsPadShape* padshape=0;
  char line[256];
  int begin=0;
  infile.getline(line, 256);
  //remove blanks at begin of line
  while (isspace((int) line[begin]))
  {
    if (line[begin] == '\0') return(true);
    begin++;
  }
  //remove comments at end of line
  char* com=strchr(line, '*');//comment
  if (com != 0)
    *com = '\0';
  //decide which PadShape
  if (line[begin] == 's')
    padshape = ReadShape(&line[++begin]);
  else if (line[begin] == 'p')
    padshape = ReadPolygon(&line[++begin]);
  else if (line[begin] == '\0')
    return(true);
  else
  {
    Warning("CeeTpcPadShapePool::ReadLine","In line: %s \nFirst character in line has no meaning.",line);
    return(false);
  }
  //if it was not possible to create a padshape with th data
  if (padshape==0)
  {
    Warning("CeeTpcPadShapePool::ReadLine","In line: %s\nIt was not possible to create a CeeTpcPadShape.",line);
    return(false);
  }
  if (TryAddingPadShape(padshape)== true)
    return(true);
  //if the ID is already used
  Warning("CeeTpcPadShapePool::ReadLine","In line: %s\nThe padshape could not be added, because the ID %i is already occupied.",line,padshape->GetID());
  delete padshape;
  return(false);
}


CeeTpcAbsPadShape*
CeeTpcPadShapePool::ReadShape(const char* const line)
{
  char* rest;
  unsigned int id;
  double d;
  id = (unsigned int) strtoul(line,&rest,10);
  d = strtod(rest,&rest);
  if (d==0)
    Warning("CeeTpcPadShapePool::ReadShape","The size of the padshape wit ID=%i is 0.",id);
  CeeTpcPadShapeSquare* pads = new CeeTpcPadShapeSquare(d, id);
  return(pads);
}

CeeTpcAbsPadShape*
CeeTpcPadShapePool::ReadPolygon(const char* const line)
{
  char* rest;
  unsigned int id;
  std::vector<double> xCoord(0);
  std::vector<double> zCoord(0);
  id = (unsigned int) strtoul(line,&rest,10);
  for (int i=0; ;i++)
  {
    char* end;
    xCoord.push_back(strtod(rest,&rest));
    zCoord.push_back(strtod(rest,&end));
    if (rest == end)
    {
      xCoord.pop_back();
      zCoord.pop_back();
      break;
    }
    rest = end;
  }
  CeeTpcPadShapePolygon* padp = new CeeTpcPadShapePolygon(xCoord, zCoord, id);
  return(padp);
}

std::ostream& operator<< (std::ostream& s, const CeeTpcPadShapePool& me)
{
  return s <<"CeeTpcPadShapePool:\n"
           <<"max. ID of a PadShape="<<me.references.size()-1<<"\n";
}

ClassImp(CeeTpcPadShapePool)

