#ifndef G4HIVESD_SCTSD_H
#define G4HIVESD_SCTSD_H

// Framework includes
#include "G4HiveSD/SensitiveDetectorToolBase.h"

// External includes
#include "G4VSensitiveDetector.hh"


// Forward declarations
class G4Step;
class G4HCofThisEvent;


/// @class SctSD
/// @brief ATLAS G4 sensitive detector for the SCT
///
/// @author Steve Farrell <Steven.Farrell@cern.ch>
///
class SctSD : public G4VSensitiveDetector
{

  public:

    /// Standard Geant4 constructor (almost)
    SctSD(const std::string& name);

    /// Initialize a G4 event.
    /// This method is called by Geant4 at the start of every event.
    void Initialize(G4HCofThisEvent*) override final;

    /// Process hits for this step.
    G4bool ProcessHits(G4Step*, G4TouchableHistory*) override final;

    /// Finalize a G4 event.
    /// This method is called by Geant4 at the end of every event.
    void EndOfEvent(G4HCofThisEvent*) override final;

  private:

    /// TODO: hits collection goes here

}; // class SctSD



/// @class SctSDTool
/// @brief Athena tool which manages the SCT sensitive detectors
///
/// @author Steve Farrell <Steven.Farrell@cern.ch>
///
class SctSDTool : public SensitiveDetectorToolBase
{

  public:

    /// Standard Athena constructor
    SctSDTool(const std::string& type, const std::string& name,
              const IInterface* parent);

    /// Initialize the tool
    StatusCode initialize() override final;

    /// Create a sensitive detector object and assign it to volumes
    G4VSensitiveDetector* makeSD() override final;

}; // class SctSDTool

#endif
