// Local includes
#include "SctSD.h"

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
SctSD::SctSD(const std::string& name) : G4VSensitiveDetector(name)
{
}

//-----------------------------------------------------------------------------
// Initialize a G4 event
//-----------------------------------------------------------------------------
void SctSD::Initialize(G4HCofThisEvent* /*hce*/)
{
  // TODO: prepare hit collection here
}

//-----------------------------------------------------------------------------
// Process hits for this step
//-----------------------------------------------------------------------------
G4bool SctSD::ProcessHits(G4Step* /*aStep*/, G4TouchableHistory* /*hist*/)
{
  // TODO: fill hit collection here
  return true;
}

//-----------------------------------------------------------------------------
// Finalize a G4 event
//-----------------------------------------------------------------------------
void SctSD::EndOfEvent(G4HCofThisEvent* /*hce*/)
{
  // TODO: finalize hit collection here
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
SctSDTool::SctSDTool(const std::string& type, const std::string& name,
                     const IInterface* parent)
 : SensitiveDetectorToolBase(type, name, parent)
{
  declareInterface<ISensitiveDetectorTool>(this);
}

//-----------------------------------------------------------------------------
// Initialize the tool
//-----------------------------------------------------------------------------
StatusCode SctSDTool::initialize()
{
  ATH_MSG_DEBUG("Initializing");
  return StatusCode::SUCCESS;
}

//-----------------------------------------------------------------------------
// Create a sensitive detector and assign it to volumes
//-----------------------------------------------------------------------------
G4VSensitiveDetector* SctSDTool::makeSD()
{
  // TODO: sloppy for now. Use smart pointers later?
  // TODO: figure out how to handle the name. Maybe use same name as the tool.
  auto sd = new SctSD("SctSensorSD");
  return sd;
}
