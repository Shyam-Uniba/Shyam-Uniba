// -----------------------------------------------------------------
// $Id: CeeTpcGas.hh,v 1.10 2006/03/10 14:45:24 sneubert Exp $
//
// Description:
//      Data class which provides access to various gas-parameters 
//      for drift detectors
//
// Environment:
//      Software developed for the PANDA Detector at FAIR.
//
// Author List:
//      Sebastian Neubert         TUM            Original Author
//      Cristoforo Simonetto      TUM
//      Salmin Roman                                    Bugfix
//
// -----------------------------------------------------------------

#ifndef CEETPCGAS_HH
#define CEETPCGAS_HH

#include <assert.h>
#include <ostream>
#include <vector>
#include <string>

enum gascomponents {Ne,Ar,CO2,CH4};

class CeeTpcGas {
public:
  // constructors
  CeeTpcGas();
  ~CeeTpcGas();
  
  CeeTpcGas(double const E,
	 double const B,
	 double const T,
	 double const p,
	 double const VDrift,
	 double const Dl,
	 double const Dt,
	 double const k,
	 double const W,
	 const std::vector<double>& CSD,
	 double const CSDEpol);

  CeeTpcGas(const std::string& Filename,
	 double const E);

  // accessors
  double VDrift() const {return fVDrift;} 
  double Dl() const {return fDl;}      
  double Dt() const {return fDt;}  
  double VDrift(double const Ee, double const Bb) const {return fVDrift;} 
  double Dl(double const Ee, double const Bb) const {return fDl;}      
  double Dt(double const Ee, double const Bb) const {return fDt;}      
  double k() const {return fk;}       
  double W() const {return fW;}       
  double CSD(int i) const {return fCSD.at(i);}
  const std::vector<double>& CSD() const {return fCSD;}
  int nCSD() const {return fCSD.size();}
  double CSDNorm() const {return fCSDNorm;}
  double CSDEpol() const {return fCSDEpol;} 

  double E() const {return fE;}
  double B() const {return fB;}
  double T() const {return fT;}       
  double p() const {return fp;}
    
  int GetRandomCS(double const r) const;   //has problem with not normalized table
  int GetRandomCSUniform() const;

  void PrintAll(std::ostream& s) const {s<<*this;}

  void operator=(const CeeTpcGas& GasToCopy);
  friend std::ostream& operator<< (std::ostream&, const CeeTpcGas&);

  // modifiers
  void SetE(double const Ee){fE=Ee;}  // later this method should also retrieve
                                    // updated gas values for the new field!
  void SetB(double const Bb){fB=Bb;}
  void SetT(double const Tt){fT=Tt;}
  void Setp(double const pp){fp=pp;}
  void SetCSD(const std::vector<double>& CSD);
  void SetCSDEpol(double const CSDEpoll){fCSDEpol=CSDEpoll;}

private:
  double fE;        // electric field [V/cm](some gas parameters depend on it)
  double fB;        // B field [T] assumption: B || E !!! 
  double fT;        // Temperature [K]
  double fp;        // pressure [mbar]

  double fVDrift;   // electron Drift velocity [cm/ns]
  double fDl;       // longitudinal diffusion coefficient [sqrt(cm)]
  double fDt;       // transversal diffusion coefficient [sqrt(cm)]
  double fk;        // attachment coefficient[1/cm]
  double fW;        // effective ionisation energy [eV]
  std::vector<double> fCSD;     // Cluster size distribution
  double fCSDNorm;                 // Intergral of CSD must be 1 but actually not. Used for correction.
  double fCSDEpol;  // Constant used for the inverse quadratic extrapolation
                    // of Cluster Sizes bigger than fnCSD

  const double LinExpolation(double const inTable, const double* const table,
		       int const nTable);
  int ReadGasBegin(std::ifstream* const pinfile);
  int ReadGasArrays(std::ifstream* const pinfile, int const noent,
		    double* const e, double* const vdrift, double* const dt,
		    double* const dl, double* const k); 
  const double GetPositionOfE(int const noent, const double* const e);
 

};

#endif
