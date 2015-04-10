#ifndef G4HIVESD_ISENSITIVEDETECTORSVC_H
#define G4HIVESD_ISENSITIVEDETECTORSVC_H

// Framework includes
#include "GaudiKernel/IService.h"

/// @class ISensitiveDetectorSvc
/// @brief Abstract interface for the SD service.
///
/// This service owns tools for each sub-detector.
/// The tools in turn own instances of the Geant4 SD objects.
/// SDs are created once per thread, though this service and its
/// tools are shared among threads.
///
/// This code is based on the ISensitiveDetectorSvc from
/// G4AtlasInterfaces-00-00-12 by Zach Marshall.
///
/// @author Steve Farrell <Steven.Farrell@cern.ch>
///
class ISensitiveDetectorSvc : public virtual IService
{

  public:

    /// Interface declaration
    static const InterfaceID& interfaceID() {
      static const InterfaceID iid_ISenDetSvc("ISensitiveDetectorSvc", 1, 0);
      return iid_ISenDetSvc;
    }

    /// Initialize the SDs for the current thread.
    /// This should only be called _once_ per thread.
    virtual StatusCode makeSDs() = 0;

}; // class ISensitiveDetectorSvc

#endif
