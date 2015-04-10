#ifndef G4HIVESD_ISENSITIVEDETECTORTOOL_H
#define G4HIVESD_ISENSITIVEDETECTORTOOL_H

#include "GaudiKernel/IAlgTool.h"


/// Forward declaration
class G4VSensitiveDetector;


/// @class ISensitiveDetectorTool
///
/// @brief Abstract interface for AthAlgTools that wrap Geant4 SD objects
///
/// In this design, thread safety is achieved with a concurrent map of
/// thread ID to SD object. This design should be re-evaluated after
/// the implementation is proven to work, both for performance and for
/// design principle considerations. In other words, it's just a first-pass.
///
/// @author Steve Farrell <Steven.Farrell@cern.ch>
///
class ISensitiveDetectorTool : virtual public IAlgTool
{

  public:

    /// Interface declaration
    static const InterfaceID& interfaceID() {
      static const InterfaceID iid_ISDTool("ISensitiveDetectorTool", 1, 0);
      return iid_ISDTool;
    }

    /// Initialize the SD for the current thread.
    /// This method is implemented in the base class.
    virtual StatusCode initializeSD() = 0;

  protected:

    /// Create a new derived SD for current thread.
    /// This method must be implemented by the concrete SD tool.
    virtual G4VSensitiveDetector* makeSD() = 0;

}; // class ISensitiveDetectorTool

#endif
