// Local includes
#include "SensitiveDetectorSvc.h"

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
SensitiveDetectorSvc::SensitiveDetectorSvc(const std::string& name,
                                           ISvcLocator* pSvcLocator)
  : AthService(name, pSvcLocator),
    m_sdTools(this)
{
  declareProperty("SensitiveDetectorTools", m_sdTools,
                  "Tool handle array of all sensitive detector tools");
}

//-----------------------------------------------------------------------------
// Initialize the SD service
//-----------------------------------------------------------------------------
StatusCode SensitiveDetectorSvc::initialize()
{
  ATH_MSG_DEBUG("Initialize");
  ATH_CHECK( m_sdTools.retrieve() );
  return StatusCode::SUCCESS;
}

//-----------------------------------------------------------------------------
// Initialize the SDs for current thread
//-----------------------------------------------------------------------------
StatusCode SensitiveDetectorSvc::makeSDs()
{
  for(auto tool : m_sdTools){
    ATH_MSG_DEBUG("Invoking SD creation for " << tool->name());
    ATH_CHECK( tool->initializeSD() );
  }
  return StatusCode::SUCCESS;
}

//-----------------------------------------------------------------------------
// Gaudi interface query
//-----------------------------------------------------------------------------
StatusCode SensitiveDetectorSvc::queryInterface(const InterfaceID& riid,
                                                void** ppvInterface)
{
  if(ISensitiveDetectorSvc::interfaceID().versionMatch(riid)){
    *ppvInterface = static_cast<ISensitiveDetectorSvc*>(this);
  }
  else{
    // Interface is not directly available; try out a base class
    return AthService::queryInterface(riid, ppvInterface);
  }
  addRef();
  return StatusCode::SUCCESS;
}
