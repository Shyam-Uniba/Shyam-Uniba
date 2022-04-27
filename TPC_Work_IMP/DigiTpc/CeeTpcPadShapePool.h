//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Owns the PndTpcPadShapes, can read them from a file and returns the 
//      right shape after getting its ID.
//
// Environment:
//      Software developed for the PANDA Detector at FAIR.
//
// Author List:
//      Cristoforo Simonetto    TUM            (original author)
//
//
//-----------------------------------------------------------

#ifndef CEETPCPADSHAPEPOOL_HH
#define CEETPCPADSHAPEPOOL_HH

// Base Class Headers ----------------

// Collaborating Class Headers -------
#include <vector>
#include <ostream>
#include <Rtypes.h>
// Collaborating Class Declarations --
class CeeTpcAbsPadShape;
class CeeTpcGem;

class CeeTpcPadShapePool {
public:

  // Constructors/Destructors ---------
  CeeTpcPadShapePool();
  CeeTpcPadShapePool(const char* const filename,
		    const CeeTpcGem& gem, 
		    const double frange,      
		    const double fstep,
		    const double fintStep);
  CeeTpcPadShapePool(const char* const filename);
  ~CeeTpcPadShapePool();

  // Operators
  friend std::ostream& operator<< (std::ostream& s, const CeeTpcPadShapePool& me);
  

  // Accessors -----------------------
  CeeTpcAbsPadShape* GetPadShape(const unsigned int fID) const;
  unsigned int numPadShapes() const {return references.size();}
  // Modifiers -----------------------
  //PadShapePool will take ownership
  // The user is responsible for Building the PRLookupTable
  void AddPadShape(CeeTpcAbsPadShape* padshape);
  void RemovePadShape(const unsigned int fID);//deletes it
  //PadShapePool will take ownership
  // PadShapePool will build the PRLookupTables
  void ReadFromFile(const char* const filename);
  void BuildLookupTable(const CeeTpcGem& gem,
			const double frange,      
			const double fstep,
			const double fintStep);
private:

  // Private Data Members ------------
  std::vector<CeeTpcAbsPadShape*> references;

  // Private Methods -----------------
  bool TryAddingPadShape(CeeTpcAbsPadShape* padshape);
  bool ReadLine(std::ifstream& infile);
  CeeTpcAbsPadShape* ReadShape(const char* const line);
  CeeTpcAbsPadShape* ReadPolygon(const char* const line);

public:

  ClassDef(CeeTpcPadShapePool,1)

};

#endif
