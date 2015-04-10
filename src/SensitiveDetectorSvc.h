#ifndef G4HIVESD_SENSITIVEDETECTORSVC_H
#define G4HIVESD_SENSITIVEDETECTORSVC_H

// Framework includes
#include "AthenaBaseComps/AthService.h"
#include "GaudiKernel/ToolHandle.h"
#include "G4HiveSD/ISensitiveDetectorSvc.h"
#include "G4HiveSD/ISensitiveDetectorTool.h"

/// @class SensitiveDetectorSvc
/// @brief Top-level service which manages the sensitive detector tools.
///
/// This class delegates the creation of the SDs for each thread to its tools.
/// Each sub-detector SD tool has a corresponding set of G4 SD objects;
/// one per thread.
///
/// This code is based on the SensitiveDetectorSvc from
/// G4AtlasServices-00-00-10 by Zach Marshall.
///
/// @author Steve Farrell <Steven.Farrell@cern.ch>
///
class SensitiveDetectorSvc : public virtual ISensitiveDetectorSvc,
                             public AthService
{

  public:

    /// Standard constructor
    SensitiveDetectorSvc(const std::string& name, ISvcLocator* pSvcLocator);

    /// Destructor
    virtual ~SensitiveDetectorSvc(){};

    /// Initialize the service
    StatusCode initialize() override;

    /// Create and attach the SDs for current thread.
    /// Should only be called _once_ per thread.
    StatusCode makeSDs() override final;

    /// Gaudi interface query
    virtual StatusCode queryInterface(const InterfaceID& riid, void** ppvInterface);

  private:

    /// Array of sensitive detector tools
    ToolHandleArray<ISensitiveDetectorTool> m_sdTools;

}; // class SensitiveDetectorSvc

#endif
