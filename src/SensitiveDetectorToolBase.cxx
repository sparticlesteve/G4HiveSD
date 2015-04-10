// Local includes
#include "G4HiveSD/SensitiveDetectorToolBase.h"

// Geant4 includes
#include "G4SDManager.hh"
#include "G4LogicalVolumeStore.hh"

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
SensitiveDetectorToolBase::SensitiveDetectorToolBase(const std::string& type,
                                                     const std::string& name,
                                                     const IInterface* parent)
  : AthAlgTool(type, name, parent)
{
  declareProperty("LogicalVolumeNames" , m_volumeNames);
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
SensitiveDetectorToolBase::~SensitiveDetectorToolBase()
{
  // Delete SDs from the concurrent map
  for(auto sdPair : m_sdThreadMap)
    delete sdPair.second;
}

//-----------------------------------------------------------------------------
// Initialize SD for current thread
//-----------------------------------------------------------------------------
StatusCode SensitiveDetectorToolBase::initializeSD()
{
  // Get current thread-ID. I believe it's ok to use std::thread::id
  // even though we're working with TBB.
  const auto id = std::this_thread::get_id();

  // Make sure there is not already an SD assigned to this thread
  if(m_sdThreadMap.find(id) != m_sdThreadMap.end()){
    ATH_MSG_ERROR("Already registered an SD for thread " << id);
    return StatusCode::FAILURE;
  }

  // Create a new SD and add it to the map
  // TODO: memory management of the SD
  ATH_MSG_DEBUG("Creating and registering SD for thread " << id);
  G4VSensitiveDetector* sd = makeSD();
  m_sdThreadMap.insert( std::make_pair(id, sd) );

  // Add the SD to the SD manager
  G4SDManager* sdManager = G4SDManager::GetSDMpointer();
  sdManager->AddNewDetector(sd);

  // Go through the logical volumes and attach this SD
  G4LogicalVolumeStore * logicalVolumeStore = G4LogicalVolumeStore::GetInstance();
  ATH_MSG_INFO("Logical volumes: " << logicalVolumeStore->size());
  bool gotOne = false;
  for (auto myvol : m_volumeNames){
    ATH_MSG_INFO("Trying to attach SD to " << myvol);
    int found = 0;
    for (auto ilv : *logicalVolumeStore){
      if (ilv->GetName() == myvol.data()){
        // Do not break on found; protect against duplicate names
        ++found;
        ilv->SetSensitiveDetector(sd);
        gotOne = true;
      } // Found a volume!
    } // Loop over all the volumes in the geometry
    // Give notice if we have missed a volume in here
    if (found == 0){
      ATH_MSG_WARNING("Volume " << myvol << " not found in G4LogicalVolumeStore.");
    } else {
      ATH_MSG_VERBOSE(found << " copies of LV " << myvol << " found; SD " <<
                      name() << " assigned.");
    }
  } // Loop over my volumes

  // Crash out if we have failed to assign a volume - this is bad news!
  if (!gotOne){
    ATH_MSG_ERROR( "Failed to assign *any* volume to SD " << name() );
    return StatusCode::FAILURE;
  }

  return StatusCode::SUCCESS;
}
