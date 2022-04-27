//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Pad plane in the CeeTpc: Shyam Kumar
//-----------------------------------------------------------

#ifndef CEETPCPADPLANETRAP_HH
#define CEETPCPADPLANETRAP_HH

// Base Class Headers ----------------



// Collaborating Class Headers -------
#include "CeeTpcPad_Trap.h"
#include <vector>
#include <map>
#include "math.h"
#include <stdio.h>
#include <Rtypes.h>


class CeeTpcPadPlane_Trap {
public:

  // Constructors/Destructors ---------
  CeeTpcPadPlane_Trap();
  CeeTpcPadPlane_Trap(const char* const filename);
  ~CeeTpcPadPlane_Trap();


  // Accessors -----------------------
  CeeTpcPad_Trap* GetPad(const unsigned int PadID) const;
  unsigned int GetNPads() const;//the number of pads
  unsigned int GetPadIdfromArray(unsigned int sectorId, unsigned int row, unsigned int padinrow) {return fPadId[sectorId][row][padinrow];}
  double GetRowMin0LocalZ(unsigned int sectorId) {return fRowMin0LocalZ[sectorId];}
  double GetPad0MinLocalXAtMinLocalZ(unsigned int sectorId, unsigned int row) {return fPad0MinLocalXAtMinLocalZ[sectorId][row];}
  double GetPad0MinLocalXAtMaxLocalZ(unsigned int sectorId, unsigned int row) {return fPad0MinLocalXAtMaxLocalZ[sectorId][row];}
  const std::vector<CeeTpcPad_Trap*>& GetPads() const {return PadReferences;}
  const unsigned int GetNSectors() const {return fnSectors;}
  std::map<unsigned int,CeeTpcPad_Trap*>* GetSectorList(unsigned int sectorId) {return fSectorLists[sectorId];}
  std::vector<unsigned int> GetSectorIds()const;

  // Modifiers -----------------------
  void ReadFromFile(const char* const filename);

private:

  std::vector<CeeTpcPad_Trap*> PadReferences;
  std::map<unsigned int,std::map<unsigned int,CeeTpcPad_Trap*>* > fSectorLists;
  std::map<unsigned int, std::map<unsigned int, std::map<unsigned int, unsigned int>>> fPadId;
  std::map<unsigned int, double> fRowMin0LocalZ;
  std::map<unsigned int, double> fRowPitch;
  std::map<unsigned int, double> fPadPitch;
  std::map<unsigned int, std::map<unsigned int, double>> fPad0MinLocalXAtMinLocalZ;
  std::map<unsigned int, std::map<unsigned int, double>> fPad0MinLocalXAtMaxLocalZ;
  unsigned int fnSectors; // assume Sectors are enumerated 0...(n-1)
                          // a sector represents the collection of pads
                          // that is read out by one front end processor


  // Private Methods -----------------
  // Pad Plane will take Ownership!
  void AddPad(CeeTpcPad_Trap* pad);
  bool ReadLine(char* const line);
public:

  ClassDef(CeeTpcPadPlane_Trap,1)

};

// exceptions thrown by this class
#include <exception>
struct unknown_padID : std::exception{
  virtual const char* what() const throw() {
    return "Unkown Pad ID";
  }
};
struct superior_unknown_padId : std::exception{
  superior_unknown_padId(const int padId) : fPadId(padId) {};
  virtual const char* what() const throw() {
    char* bla = new char[50];
    sprintf(bla, "Unknown Pad ID %d", fPadId);
    return bla;
  }
  private:
  int fPadId;
};
struct used_padID : std::exception{
  virtual const char* what() const throw() {
    return "The Pad ID is already assigned.";
  }
}; 



#endif

