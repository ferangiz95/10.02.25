#include "RunAction.hh"
#include "Run.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"
#include <iomanip>

#include "G4AnalysisManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* prim)
  : fDetector(det), fPrimary(prim)    
{

     auto analysisManager = G4AnalysisManager::Instance();
      analysisManager->SetActivation(true);
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
     analysisManager->CreateNtuple("Step", "Step");
     analysisManager->CreateNtupleSColumn("name");
     analysisManager->CreateNtupleSColumn("type");
     analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* RunAction::GenerateRun()
{ 
  fRun = new Run(fDetector); 
  return fRun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{    
  // show Rndm status
  if (isMaster) G4Random::showEngineStatus();
   
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance(); 
  
  // keep run condition
  if (fPrimary) { 
    G4ParticleDefinition* particle 
      = fPrimary->GetParticleGun()->GetParticleDefinition();
    fRun->SetPrimary(particle);
  } 
  
    
    G4String fileName = "Hadr03.root";
    analysisManager->OpenFile(fileName); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*)
{

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if (isMaster) fRun->EndOfRun(fPrint);    
      
  // show Rndm status
  if (isMaster) G4Random::showEngineStatus();
  
    //save histograms

    analysisManager->Write();
    analysisManager->CloseFile();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::SetPrintFlag(G4bool flag)
{ 
  fPrint = flag;
}
