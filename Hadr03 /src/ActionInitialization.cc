#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::ActionInitialization(DetectorConstruction* detector)
 : fDetector(detector)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::BuildForMaster() const
{
  RunAction* runAction = new RunAction(fDetector, 0);
  SetUserAction(runAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::Build() const
{
  PrimaryGeneratorAction* primary = new PrimaryGeneratorAction(fDetector);
  SetUserAction(primary);
    
  RunAction* runAction = new RunAction(fDetector, primary );
  SetUserAction(runAction);
  
  SteppingAction* steppingAction = new SteppingAction();
  SetUserAction(steppingAction);
}  

