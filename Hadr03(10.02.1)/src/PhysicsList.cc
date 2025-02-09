#include "PhysicsList.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4NuclideTable.hh"

#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronPhysicsQGSP_BIC_AllHP.hh"
//#include "G4RadioactiveDecayPhysics.hh"
//#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"

// particles

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList()
{
  G4int verb = 1;  
  SetVerboseLevel(verb);
    
  // Hadron Elastic scattering
  RegisterPhysics( new G4HadronElasticPhysicsHP(verb));  

  // Hadron Inelastic physics
  RegisterPhysics( new G4HadronPhysicsQGSP_BIC_AllHP(verb));
  
     // Radioactive decay
 //RegisterPhysics(new G4RadioactiveDecayPhysics());

    // EM physics
  RegisterPhysics(new G4EmStandardPhysics());

    // Decay
  //RegisterPhysics(new G4DecayPhysics());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructParticle()
{
  G4BosonConstructor  pBosonConstructor;
  pBosonConstructor.ConstructParticle();

  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();

  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();  
}

