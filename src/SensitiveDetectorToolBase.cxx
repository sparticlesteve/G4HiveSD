// Local includes
#include "G4HiveSD/SensitiveDetectorToolBase.h"

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
SensitiveDetectorToolBase::SensitiveDetectorToolBase(const std::string& type,
                                                     const std::string& name,
                                                     const IInterface* parent)
  : AthAlgTool(type, name, parent)
{
  // TODO: put logical volume names here?
}

//-----------------------------------------------------------------------------
// Initialize SD for current thread
//-----------------------------------------------------------------------------
StatusCode SensitiveDetectorToolBase::initializeSD()
{
  return StatusCode::SUCCESS;
}
