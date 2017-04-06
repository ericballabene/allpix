//For information, see the top of the corresponding .cc file.

#ifndef AllPixFEI4RadDamageDigitizer_h
#define AllPixFEI4RadDamageDigitizer_h 1

#include "AllPixDigitizerInterface.hh"
#include "AllPixFEI4RadDamageDigit.hh"
#include "G4PrimaryVertex.hh"
#include <map>
#include <vector>
#include "AllPixTrackerHit.hh"
#include "AllPixGeoDsc.hh"
#include "TString.h"
#include "TH2D.h"
#include "TH3F.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TMath.h"

using namespace std;

class AllPixFEI4RadDamageDigitizer : public  AllPixDigitizerInterface {

public:
  AllPixFEI4RadDamageDigitizer(G4String, G4String, G4String);
  virtual ~AllPixFEI4RadDamageDigitizer();

  void SetPrimaryVertex(G4PrimaryVertex * pv) {m_primaryVertex = pv;};
  void Digitize ();
//  void SetDetectorDigitInputs(G4double){};
  void SetDetectorDigitInputs(G4double);

private:

  // digitInput typedef is defined in AllPixDigitizerInterface.hh
  digitInput m_digitIn;

  AllPixFEI4RadDamageDigitsCollection * m_digitsCollection;
  vector<G4String> m_hitsColName;
  G4PrimaryVertex * m_primaryVertex; // information from EventAction

  TH3F *ramoPotentialMap;
  TH1F *ramoPotentialMap1D;
  TH2F *ramoPotentialMap2D;
  TH3F *eFieldMap;
  TH1F *m_eFieldMap1D;
  TH1F *timeMap_e;
  TH1F *timeMap_h;
  TH2F *distancemap_e;
  TH2F *distancemap_h;
  TH2F *lorentz_map_e;
  TH2F *lorentz_map_h;
  TH3F *charge_chunk_map_e;
  TH3F *charge_chunk_map_h;

  //Functions needed for the default Ramo potential:
  double betax(int n, int Nrep, double a){ return 2*TMath::Pi()*n/(Nrep*a); }
  double betay(int n, int Nrep, double b){  return 2*TMath::Pi()*n/(Nrep*b); }
  double An(int n, int Nrep, double a){  
    if (n==0) return 1./Nrep;
    else return sin(n*TMath::Pi()*a/(Nrep*a))/(TMath::Pi()*n); }
  double Bn(int n, int Nrep, double b){
    if (n==0) return 1./Nrep;
    else return sin(n*TMath::Pi()*b/(Nrep*b))/(TMath::Pi()*n); }
  double X(double x, int n, int Nrep, double a){  return An(n,Nrep,a)*cos(betax(n,Nrep,a)*x); }
  double Y(double y, int n, int Nrep, double b){  return Bn(n,Nrep,b)*cos(betay(n,Nrep,b)*y); }
  double Z(double z, int n, int m, int Nrep, double a, double b){
    double mynorm = sqrt(pow(betax(n,Nrep,a),2)+pow(betay(m,Nrep,b),2));
    if (n==0 && m==0) return 1-z;
    else return sinh(mynorm*(1-z))/sinh(mynorm);
  }
  double Phi3D(double x, double y, double z, int n, int m, int Nrep, double a, double b){
    //be warned that there is numerical instability if n and m are too large!  Suggest n ~ m ~ 10.
    double myout = 0.;
    for (int ii=-n; ii<=n; ii++){
      for (int jj=-m; jj<=m; jj++){
	myout+=Z(z,ii,jj,Nrep,a,b)*X(x,ii,Nrep,a)*Y(y,jj,Nrep,b);
      }
    }
    return myout;
  }

  G4double GetElectricField(G4double z);
  G4double GetElectricField(G4double x, G4double y, G4double z);
  G4double GetMobility(G4double electricField, G4bool isHole);
  G4double GetDriftVelocity(G4double electricField, G4double mobility, G4bool isHole);
  G4double GetMeanFreePath(G4double driftVelocity, G4bool isHole);
  G4double GetTrappingProbability(G4double z, G4double meanFreePath);
  G4double GetDriftTime(G4bool isHole);
  G4double GetTimeToElectrode(G4double z, G4bool isHole);
  G4double GetTanLorentz(G4double electricField, G4bool isHole);
  G4double GetTanLorentz(G4double z1, G4double z2, G4bool isHole);
  G4double Phi(G4double x, G4double z, G4double Lx, G4double Lz);
  G4double N(G4double z, G4double L);
  G4double elec;
  G4int count;

  // Variables for the charge sharing computation
  G4double mobility;
  G4double resistivity;
  G4bool   bulkType;

  G4double depVoltage;
  G4double deplationLenght;

  G4double detectorThickness;
  G4double biasVoltage;
  G4double temperature;
  G4double fluence;
  G4double trappingTimeElectrons;
  G4double trappingTimeHoles;
  G4double betaElectrons;
  G4double betaHoles;
  G4double bField;
  G4double chipNoise;
  G4double tuning;
  G4double threshold;
  G4double diffusion_length;
  
  G4double epsilon;
  G4double echarge;
  G4int precision;

  // Physics process switches
  G4bool doTrapping;
  G4bool doRamo;
  G4bool doDiff;
  G4bool doSimplifiedModel;
  G4bool doSlimEdge;
  G4bool isHole;

  //Defaults
  G4int defaultEfield;
  G4int defaultRamo;
  G4bool debug_maps;
  
  // Geometry-related constants
  G4double pitchX;
  G4double pitchY;
  G4int nPixX;
  G4int nPixY;
  G4double chargeSharingConstant;
  G4double GRShift;
  G4int FEIX;
  G4int Sensor;

  //Tuning of the chip and counters characteristics
  G4int MipTOT;
  G4int CounterDepth;
  G4int MipCharge;
  G4double Lv1Unit;
};

#endif
