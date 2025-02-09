#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
    DetectorConstruction();
   ~DetectorConstruction() override;

  public:
    G4VPhysicalVolume* Construct() override;

    /*G4Material* 
    MaterialWithSingleIsotope(G4String, G4String, G4double, G4int, G4int);
         
    void SetRadius (G4double);              
    void SetMaterial (G4String);        

  public:           
     G4double GetRadius() {return fRadius;};      
     G4Material* GetMaterial() {return fMaterial;};
                       
  private:
     G4double fRadius;
     G4LogicalVolume* fLTarget;
     G4VPhysicalVolume* fPTarget;
     G4VPhysicalVolume* fPBox;
     G4LogicalVolume* fLBox;
     
     G4double fBoxSize;
     G4Material* fMaterial;*/
  private:
    
     void               DefineMaterials();
     G4VPhysicalVolume* ConstructVolumes();   
     //G4bool fCheckOverlaps = true;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
