#include "Run.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4HadronicProcess.hh"
#include "G4Neutron.hh"
#include "G4ProcessTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Run::Run(DetectorConstruction* det) : fDetector(det)
{}

void Run::SetPrimary(G4ParticleDefinition* particle, G4double energy)
{
  fParticle = particle;
  fEkin = energy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::CountProcesses(G4VProcess* process)
{
  if (process == nullptr) return;
  G4String procName = process->GetProcessName();
  std::map<G4String, G4int>::iterator it = fProcCounter.find(procName);
  if (it == fProcCounter.end()) {
    fProcCounter[procName] = 1;
  }
  else {
    fProcCounter[procName]++;
  }
}

void Run::ParticleCount(G4String name, G4double Ekin)
{
  fTotalCount++;
  std::map<G4String, ParticleData>::iterator it = fParticleDataMap.find(name);
  if (it == fParticleDataMap.end()) {
    fParticleDataMap[name] = ParticleData(1, Ekin);
  }
  else {
    ParticleData& data = it->second;
    data.fCount++;
    data.fEmean += Ekin;
  }
}

void Run::Merge(const G4Run* run)
{
  const Run* localRun = static_cast<const Run*>(run);

  // primary particle info
  //
  fParticle = localRun->fParticle;
  fEkin = localRun->fEkin;

  // accumulate sums
  //
  fTotalCount += localRun->fTotalCount;

  // map: processes count
  std::map<G4String, G4int>::const_iterator itp;
  for (itp = localRun->fProcCounter.begin(); itp != localRun->fProcCounter.end(); ++itp) {
    G4String procName = itp->first;
    G4int localCount = itp->second;
    if (fProcCounter.find(procName) == fProcCounter.end()) {
      fProcCounter[procName] = localCount;
    }
    else {
      fProcCounter[procName] += localCount;
    }
  }

  // map: particles count
  std::map<G4String, ParticleData>::const_iterator itn;
  for (itn = localRun->fParticleDataMap.begin(); itn != localRun->fParticleDataMap.end(); ++itn) {
    G4String name = itn->first;
    const ParticleData& localData = itn->second;
    if (fParticleDataMap.find(name) == fParticleDataMap.end()) {
      fParticleDataMap[name] =
        ParticleData(localData.fCount, localData.fEmean);
    }
    else {
      ParticleData& data = fParticleDataMap[name];
      data.fCount += localData.fCount;
      data.fEmean += localData.fEmean;
    }
  }

  G4Run::Merge(run);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::EndOfRun(G4bool print)
{
  G4int prec = 5, wid = prec + 2;
  G4int dfprec = G4cout.precision(prec);

  G4String Particle = fParticle->GetParticleName();
  
  G4cout << "\n The run is " << numberOfEvent << " " << Particle << " of "
         << G4BestUnit(fEkin, "Energy") << G4endl;

  if (numberOfEvent == 0) {
    G4cout.precision(dfprec);
    return;
  }

  // frequency of processes
  //
  G4cout << "\n Process calls frequency:" << G4endl;
  G4int survive = 0;
  std::map<G4String, G4int>::iterator it;
  for (it = fProcCounter.begin(); it != fProcCounter.end(); it++) {
    G4String procName = it->first;
    G4int count = it->second;
    G4cout << "\t" << procName << "= " << count;
    if (procName == "Transportation") survive = count;
  }
  G4cout << G4endl;
  
  // particles count
  //
  G4cout << "\n List of generated particles:" << G4endl;

  std::map<G4String, ParticleData>::iterator itn;
  for (itn = fParticleDataMap.begin(); itn != fParticleDataMap.end(); itn++) {
    G4String name = itn->first;
    ParticleData data = itn->second;
    G4int count = data.fCount;
    G4double eMean = data.fEmean / count;
    if (print)
      G4cout << "  " << std::setw(13) << name << "; " << std::setw(7) << count
             << "  Emean = " << std::setw(wid) << G4BestUnit(eMean, "Energy") << G4endl;
  }

  // remove all contents in fProcCounter, fCount
  fProcCounter.clear();
  fParticleDataMap.clear();

  // restore default format
  G4cout.precision(dfprec);
}

