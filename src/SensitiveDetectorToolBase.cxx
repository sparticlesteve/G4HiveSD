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
  // Get current thread-ID. I believe it's ok to use std::thread::id even
  // though we're working with TBB.
  const auto id = std::this_thread::get_id();

  // Make sure there is not already an SD assigned to this thread
  if(m_sdThreadMap.find(id) != m_sdThreadMap.end()){
    ATH_MSG_ERROR("Already registered an SD for thread " << id);
    return StatusCode::FAILURE;
  }

  // Create a new SD and add it to the map
  G4VSensitiveDetector* sd = makeSD();
  m_sdThreadMap.insert( std::make_pair(id, sd) );

  return StatusCode::SUCCESS;
}
