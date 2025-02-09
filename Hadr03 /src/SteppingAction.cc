#include "SteppingAction.hh"
#include "Run.hh"

#include "G4ParticleTypes.hh"
#include "G4RunManager.hh"
#include "G4HadronicProcess.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
 G4int trackID = aStep->GetTrack()->GetTrackID();
 G4int stepNb  = aStep->GetTrack()->GetCurrentStepNumber();
 /*if (trackID * stepNb == 1) return;*/
 
  // count processes
  // 
  const G4StepPoint* endPoint = aStep->GetPostStepPoint();
  G4StepStatus stepStatus = endPoint->GetStepStatus();
  Run* run = static_cast<Run*>(
        G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  G4VProcess* process   = const_cast<G4VProcess*>(endPoint->GetProcessDefinedStep());
  run->CountProcesses(process);
  
  const G4StepPoint* prePoint = aStep->GetPreStepPoint();
  G4double Q             = - prePoint->GetKineticEnergy();
  
  //secondaries
  //
  const std::vector<const G4Track*>* secondary 
                                    = aStep->GetSecondaryInCurrentStep();  
  
  G4ParticleDefinition* particle = aStep->GetTrack()->GetDefinition();
  G4String partName = particle->GetParticleName();                             
   
   // Get volume of the current step
  //auto volume = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume(); 
   
  //for (size_t lp=0; lp<(*secondary).size(); lp++) {
    //if (volume == fDetConstruction->GetGasPV()
    //particle = (*secondary)[lp]->GetDefinition(); 
    G4String name   = particle->GetParticleName();
    G4String type   = particle->GetParticleType();      
    //G4double energy = (*secondary)[lp]->GetKineticEnergy();
    run->ParticleCount(name);
    
    //particle flag
    //fParticleFlag[particle]++;
  //}
              
  // kill event after first interaction
  //
  //G4RunManager::GetRunManager()->AbortEvent();  
}

