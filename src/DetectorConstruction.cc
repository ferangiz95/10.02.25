#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Trd.hh"
#include "G4Orb.hh"
#include "Randomize.hh"
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

  G4Material* targetmater = nist->FindOrBuildMaterial("G4_Ca");

  G4bool checkOverlaps = true;

  //
  // World
  //
  
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  auto solidWorld =
    new G4Box("World",  // its name
              5. * m, 5. * m, 5. * m);  // its size

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

                    
  G4Tubs* 
  Target = new G4Tubs("Target",                                          //name
                      125 * mm,  127 * mm, 375 * mm, 0. * deg, 180. * deg); //dimensions


  G4LogicalVolume *LogTarget = new G4LogicalVolume(Target,           //shape
                                                  targetmater,              //material
                                                   "Target");              //name
                               
  G4VPhysicalVolume *PTube =  new G4PVPlacement(0,                         //no rotation
                                                G4ThreeVector(),             //at (0,0,0)
                                                LogTarget,                //logical volume
                                                "Target",                //name
                                                logicWorld,                 //mother  volume
                                                5,                       //no boolean operation
                                                0);
  G4Tubs* 
  Target1 = new G4Tubs("Target1",                                          //name
                      75.5 * mm,  77.5 * mm, 375 * mm, 0. * deg, 180. * deg); //dimensions


  G4LogicalVolume *LogTarget1 = new G4LogicalVolume(Target1,           //shape
                                                   world_mat,              //material
                                                   "Target1");              //name
                               
  G4VPhysicalVolume *PTube1 =  new G4PVPlacement(0,                         //no rotation
                                                G4ThreeVector(),             //at (0,0,0)
                                                LogTarget1,                //logical volume
                                                "Target1",                //name
                                                logicWorld,                 //mother  volume
                                                5,                       //no boolean operation
                                                0);
   
  G4Tubs* 
  Empty = new G4Tubs("Empty",                                   //name
                     0., 77.5 * mm, 375 * mm, 0. * deg, 180. * deg); //dimensions


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
 //5*20*0.5 
 constexpr G4int NP = 100;        // number of spheres                                                                
 constexpr G4double r = 2.285*mm;   // radius of a sphere                                                                 
 constexpr G4double DMIN = 2*mm;   // min distance between two spheres                                                  
 constexpr G4double Rcyl = 1*mm; // radius of the cylinder                                                             
 constexpr G4double Zcyl = 2*mm; // half length of the cylinder                                                        



 // create a logical volumes for cylinder                                                                               
 //G4Material* cyl_material = nist->FindOrBuildMaterial("G4_WATER");
 G4Tubs* cylinder =
   new G4Tubs("Cylinder", 77.5 * mm, 125 * mm, 375 * mm, 0. * deg, 180. * deg);
 G4LogicalVolume* cyl_logical =
   new G4LogicalVolume(cylinder, targetmater, "Cylinder");

 // generate positions inside cylinder                                                                                  
 G4ThreeVector bmin, bmax;
 cylinder->BoundingLimits(bmin, bmax); // get bounding box of the cylinder                                              
 std::vector<G4ThreeVector> position;
 while (position.size() < NP) {
   G4double x = bmin.x() + (bmax.x() - bmin.x())*G4UniformRand();
   G4double y = bmin.y() + (bmax.y() - bmin.y())*G4UniformRand();
   G4double z = bmin.z() + (bmax.z() - bmin.z())*G4UniformRand();
   G4ThreeVector p(x, y, z);
   // check position                                                                                                    
   if (cylinder->Inside(p) != kInside) continue;
   if (cylinder->DistanceToOut(p) < 0.5*DMIN) continue;
   G4int size = position.size();
   G4int k = 0;
   for ( ; k < size; ++k) {
     if ((position[k] - p).mag() < DMIN) break;
   }
   if (k == size) position.push_back(p); // add position to the list                                                    
 }

 // create a logical volume for spheres                                                                                 
 G4Material* sph_material = nist->FindOrBuildMaterial("G4_AU");
 G4Orb* sphere = new G4Orb("Sphere", r);
 G4LogicalVolume* sph_logical =
   new G4LogicalVolume(sphere,targetmater,"Sphere");

 // create physical volumes for spheres                                                                                 
 for (G4int i = 0; i < NP; ++i) {
   new G4PVPlacement(0, position[i], sph_logical, "Partical", cyl_logical,
                     false, 0, checkOverlaps);
 }

 // create physical volume for cylinder                                                                                 
 new G4PVPlacement(0, G4ThreeVector(), cyl_logical, "Cylinder", logicWorld,
                   false, 0, checkOverlaps);
                   
  G4VisAttributes* grey = new G4VisAttributes(G4Colour::Grey());
  grey->SetVisibility(true);
  grey->SetForceWireframe(true);
  sph_logical->SetVisAttributes(grey);
  
  G4VisAttributes* gray = new G4VisAttributes(G4Color(0.5, 0.5, 0.5, 2.0));
  gray->SetVisibility(true);
  gray->SetForceWireframe(true);
  cyl_logical->SetVisAttributes(gray);

  G4VisAttributes* grey1 = new G4VisAttributes(G4Colour::Red());
  grey1->SetVisibility(true);
  grey1->SetForceSolid(true);
  LogTarget->SetVisAttributes(grey1);

  G4VisAttributes* grey2 = new G4VisAttributes(G4Colour::Red());
  grey2->SetVisibility(true);
  grey2->SetForceSolid(true);
  LogTarget1->SetVisAttributes(grey2);
  
  //
  fScoringVolume = cyl_logical;
  //
  // always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B1
