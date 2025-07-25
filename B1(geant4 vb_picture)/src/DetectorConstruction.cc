//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1/src/DetectorConstruction.cc
/// \brief Implementation of the B1::DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Trd.hh"
#include "G4VisAttributes.hh"

#include "G4RotationMatrix.hh"
#include "G4PhysicalConstants.hh"
#include "G4Tubs.hh"
#include "G4UnitsTable.hh"

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Envelope parameters
  //
  /*G4double env_sizeXY = 20 * cm, env_sizeZ = 30 * cm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");*/

  G4Material* targetmater = nist->FindOrBuildMaterial("G4_Ca");
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  /*G4double world_sizeXY = 1.2 * env_sizeXY;
  G4double world_sizeZ = 1.2 * env_sizeZ;*/
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  auto solidWorld =
    new G4Box("World",  // its name
              3 * m, 3 * m, 3 * m);  // its size

  auto logicWorld = new G4LogicalVolume(solidWorld,  // its solid
                                        world_mat,  // its material
                                        "World");  // its name

  auto physWorld = new G4PVPlacement(nullptr,  // no rotation
                                     G4ThreeVector(),  // at (0,0,0)
                                     logicWorld,  // its logical volume
                                     "World",  // its name
                                     nullptr,  // its mother  volume
                                     false,  // no boolean operation
                                     0,  // copy number
                                     checkOverlaps);  // overlaps checking

  //
  // Envelope
  //
  /*auto solidEnv = new G4Box("Envelope",  // its name
                            0.5 * env_sizeXY, 0.5 * env_sizeXY, 0.5 * env_sizeZ);  // its size

  auto logicEnv = new G4LogicalVolume(solidEnv,  // its solid
                                      env_mat,  // its material
                                      "Envelope");  // its name

  new G4PVPlacement(nullptr,  // no rotation
                    G4ThreeVector(),  // at (0,0,0)
                    logicEnv,  // its logical volume
                    "Envelope",  // its name
                    logicWorld,  // its mother  volume
                    false,  // no boolean operation
                    0,  // copy number
                    checkOverlaps);  // overlaps checking

  //
  // Shape 1
  //
  G4Material* shape1_mat = nist->FindOrBuildMaterial("G4_A-150_TISSUE");
  G4ThreeVector pos1 = G4ThreeVector(0, 2 * cm, -7 * cm);

  // Conical section shape
  G4double shape1_rmina = 0. * cm, shape1_rmaxa = 2. * cm;
  G4double shape1_rminb = 0. * cm, shape1_rmaxb = 4. * cm;
  G4double shape1_hz = 3. * cm;
  G4double shape1_phimin = 0. * deg, shape1_phimax = 360. * deg;
  
  auto solidShape1 = new G4Cons("Shape1", shape1_rmina, shape1_rmaxa, shape1_rminb, shape1_rmaxb,
                                shape1_hz, shape1_phimin, shape1_phimax);

  auto logicShape1 = new G4LogicalVolume(solidShape1,  // its solid
                                         shape1_mat,  // its material
                                         "Shape1");  // its name

  new G4PVPlacement(nullptr,  // no rotation
                    pos1,  // at position
                    logicShape1,  // its logical volume
                    "Shape1",  // its name
                    logicEnv,  // its mother  volume
                    false,  // no boolean operation
                    0,  // copy number
                    checkOverlaps);  // overlaps checking

  //
  // Shape 2
  //
  G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_BONE_COMPACT_ICRU");
  G4ThreeVector pos2 = G4ThreeVector(0, -1 * cm, 7 * cm);

  // Trapezoid shape
  G4double shape2_dxa = 12 * cm, shape2_dxb = 12 * cm;
  G4double shape2_dya = 10 * cm, shape2_dyb = 16 * cm;
  G4double shape2_dz = 6 * cm;
  auto solidShape2 =
    new G4Trd("Shape2",  // its name
              0.5 * shape2_dxa, 0.5 * shape2_dxb, 0.5 * shape2_dya, 0.5 * shape2_dyb,
              0.5 * shape2_dz);  // its size

  auto logicShape2 = new G4LogicalVolume(solidShape2,  // its solid
                                         shape2_mat,  // its material
                                         "Shape2");  // its name

  new G4PVPlacement(nullptr,  // no rotation
                    pos2,  // at position
                    logicShape2,  // its logical volume
                    "Shape2",  // its name
                    logicEnv,  // its mother  volume
                    false,  // no boolean operation
                    0,  // copy number
                    checkOverlaps); */ // overlaps checking
                    
  G4Tubs* 
  Target = new G4Tubs("Target",                                          //name
                      50.0 * cm, 125 * cm, 175 * cm, -90. * deg, 180. * deg); //dimensions


  G4LogicalVolume *LogTarget = new G4LogicalVolume(Target,           //shape
                                                  targetmater,              //material
                                                   "Target");              //name
                               
  G4VPhysicalVolume *PTube =  new G4PVPlacement(0,                         //no rotation
                                                G4ThreeVector(),             //at (0,0,0)
                                                LogTarget,                //logical volume
                                                "Target",                //name
                                               logicWorld,                 //mother  volume
                                                false,                       //no boolean operation
                                                0);
                                                
                                                
  G4Tubs* 
  Empty = new G4Tubs("Empty",                                   //name
                     0., 50.0 * cm, 175 * cm, -90. * deg, 180. * deg); //dimensions


  G4LogicalVolume *LogicEmpty= new G4LogicalVolume(Empty,           //shape
                                                   world_mat,              //material
                                                   "Empty");                 //name
                               
  G4VPhysicalVolume  *PEmpty = new G4PVPlacement(0,                         //no rotation
                                                 G4ThreeVector(),             //at (0,0,0)a
                                                 LogicEmpty,                //logical volume
                                                 "Empty",                    //name
                                                 logicWorld,                 //mother  volume
                                                 false,                       //no boolean operation
                                                 0);
                                                 
  G4VisAttributes* red = new G4VisAttributes(G4Colour::Red());
  red->SetVisibility(true);
  red->SetForceWireframe(true);
  logicWorld->SetVisAttributes(red);


  G4VisAttributes* blue = new G4VisAttributes(G4Colour::Blue());
  blue->SetVisibility(true);
  blue->SetForceWireframe(true);
  LogTarget->SetVisAttributes(blue);
  
  G4VisAttributes* white = new G4VisAttributes(G4Colour::White());
  white->SetVisibility(true);
  white->SetForceWireframe(true);
  LogicEmpty->SetVisAttributes(white);
  
  // Set Shape2 as scoring volume
  //
  fScoringVolume = LogTarget;

  //
  // always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B1
