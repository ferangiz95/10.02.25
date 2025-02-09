#include "DetectorConstruction.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4RunManager.hh"

#include "G4PhysicalConstants.hh"
#include "G4Tubs.hh"

#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"

#include "G4Sphere.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
{
  DefineMaterials();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return ConstructVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{ 
  G4int ncomponents;
  G4bool isotopes = false;
  G4double fractionmass;
  
  G4NistManager* nist = G4NistManager::Instance();
  
  G4Element* C = new G4Element("Carbon", "C", 6, 12.011*g/mole);
  G4Element* Cr = new G4Element("Chromium","Cr", 24., 52.00*g/mole);
  G4Element* Fe = new G4Element("Ferrum","Fe", 26., 55.85*g/mole);
  G4Element* Ni = new G4Element("Nickel","Ni", 28., 58.70*g/mole);
  G4Element* Ti = new G4Element("Titanium", "Ti", 22., 47.88*g/mole);
  
  G4Element* Ox = new G4Element("Oxygen", "O", 8., 16.00*g/mole);
  G4Element* Ca = new G4Element("Calcium", "Ca", 20., 40.08*g/mole);
  
  auto TargetMater = 
  new G4Material("TargetMater", 0.176 * g / cm3, ncomponents=2, kStateSolid, 293.*kelvin, 1.*atmosphere);
  
  TargetMater->AddElement(Ox, fractionmass=0.03);
  TargetMater->AddElement(Ca, fractionmass=0.97);
  
  // build materials
  //

  auto steel_mat = 
  new G4Material("steel_mat", 7.9 * g / cm3, ncomponents=5, kStateSolid, 293.*kelvin, 1.*atmosphere);
  
    steel_mat->AddElement(C, fractionmass=0.008);
    steel_mat->AddElement(Cr, fractionmass=0.18);
  
    steel_mat->AddElement(Fe, fractionmass=0.707);
    steel_mat->AddElement(Ni, fractionmass=0.10);
    steel_mat->AddElement(Ti, fractionmass=0.005);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructVolumes()
{
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");
  
  G4double TargetLength = 0.75 * m;
  G4double CylinderRadius1 = 0.0777 * m;
  G4double TubeRadius = 0.1248 * m;
  G4double CylinderRadius2 = 0.125 * m;
  G4double EmptyRadius = 0.0775 * m;
  
  G4Material* steel_mat = nist->FindOrBuildMaterial("steel_mat");
  G4Material* TargetMater = nist->FindOrBuildMaterial("TargetMater");
  
  G4Box*
  sBox = new G4Box("Container",                         //its name
                   1.0*m , 1.0*m , 1.0*m);   //its dimensions

  G4LogicalVolume *LBox = new G4LogicalVolume(sBox,                     //its shape
                                              world_mat,                 //its material
                                              "Container");     //its name

  G4VPhysicalVolume *PBox = new G4PVPlacement(0,                          //no rotation
                                              G4ThreeVector(),            //at (0,0,0)
                                              LBox,                      //its logical volume
                                              "Container",       //its name
                                              nullptr,                          //its mother  volume
                                              false,                      //no boolean operation
                                              0);

  G4Tubs* 
  Target = new G4Tubs("Target",                                          //name
                      EmptyRadius, TubeRadius, TargetLength/2, 0., twopi); //dimensions


  G4LogicalVolume *LogTarget = new G4LogicalVolume(Target,           //shape
                                                   TargetMater,              //material
                                                   "Target");              //name
                               
  G4VPhysicalVolume *PTube =  new G4PVPlacement(0,                         //no rotation
                                                G4ThreeVector(),             //at (0,0,0)
                                                LogTarget,                //logical volume
                                                "Target",                //name
                                                LBox,                 //mother  volume
                                                false,                       //no boolean operation
                                                0);
  G4Tubs* 
  Cylinder1 = new G4Tubs("Cylinder1",                                   //name
                         EmptyRadius, CylinderRadius1, TargetLength/2, 0., twopi); //dimensions


  G4LogicalVolume *LogicCylinder1 = new G4LogicalVolume(Cylinder1,           //shape
                                                        steel_mat,              //material
                                                        "Cylinder1");                 //name
                               
  G4VPhysicalVolume  *PCylinder1 = new G4PVPlacement(0,                         //no rotation
                                                 G4ThreeVector(),             //at (0,0,0)a
                                                 LogicCylinder1,                //logical volume
                                                 "Cylinder1",                    //name
                                                 LBox,                 //mother  volume
                                                 false,                       //no boolean operation
                                                 0);
  G4Tubs* 
  Empty = new G4Tubs("Empty",                                   //name
                     0., EmptyRadius, TargetLength/2, 0., twopi); //dimensions


  G4LogicalVolume *LogicEmpty= new G4LogicalVolume(Empty,           //shape
                                                   world_mat,              //material
                                                   "Empty");                 //name
                               
  G4VPhysicalVolume  *PEmpty = new G4PVPlacement(0,                         //no rotation
                                                 G4ThreeVector(),             //at (0,0,0)a
                                                 LogicEmpty,                //logical volume
                                                 "Empty",                    //name
                                                 LBox,                 //mother  volume
                                                 false,                       //no boolean operation
                                                 0);                                             
                                                 
  G4Tubs* 
  Cylinder2 = new G4Tubs("Cylinder2",                                   //name
                        TubeRadius, CylinderRadius2, TargetLength/2, 0., twopi); //dimensions


  G4LogicalVolume *LogicCylinder2 = new G4LogicalVolume(Cylinder2,           //shape
                                                       steel_mat,              //material
                                                       "Cylinder2");                 //name
                               
  G4VPhysicalVolume  *PCylinder2 = new G4PVPlacement(0,                         //no rotation
                                                    G4ThreeVector(),             //at (0,0,0)a
                                                    LogicCylinder2,                //logical volume
                                                    "Cylinder2",                    //name
                                                    LBox,                 //mother  volume
                                                    false,                       //no boolean operation
                                                    0); 
   
  //always return the root volume
  //
  return PBox;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
