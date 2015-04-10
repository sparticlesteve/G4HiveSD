#ifndef G4HIVESD_SENSITIVEDETECTORTOOLBASE_H
#define G4HIVESD_SENSITIVEDETECTORTOOLBASE_H

// Framework includes
#include "AthenaBaseComps/AthAlgTool.h"
#include "G4HiveSD/ISensitiveDetectorTool.h"

// External includes
#include "G4VSensitiveDetector.hh"
#include "tbb/concurrent_unordered_map.h"

// System includes
#include <vector>
#include <string>
#include <thread>


/// @class SensitiveDetectorToolBase
/// @brief Base class for the AthAlgTools that wrap G4 SDs
///
/// This class handles the thread safety functionality of Geant4-MT by
/// organizing thread-private SDs into a concurrent map keyed by thread ID.
///
/// @author Steve Farrell <Steven.Farrell.cern.ch>
///
class SensitiveDetectorToolBase : public virtual ISensitiveDetectorTool,
                                  public AthAlgTool
{

  public:

    /// Standard Athena constructor
    SensitiveDetectorToolBase(const std::string& type, const std::string& name,
                              const IInterface* parent);

    /// Destructor cleans up the map memory (for now)
    ~SensitiveDetectorToolBase();

    /// Initialize SD for current thread.
    /// Adds the SD to the map keyed by thread ID.
    /// Returns failure if called more than once in a thread.
    StatusCode initializeSD() override final;

  protected:

    /// List of logical volume names associated with this SD
    std::vector<std::string> m_volumeNames;

  private:

    /// Thread-to-SD concurrent map type
    typedef tbb::concurrent_unordered_map < std::thread::id,
                                            G4VSensitiveDetector*,
                                            std::hash<std::thread::id> > SDThreadMap_t;

    /// Concurrent map of SDs, one for each thread
    SDThreadMap_t m_sdThreadMap;

}; // class SensitiveDetectorToolBase

#endif
