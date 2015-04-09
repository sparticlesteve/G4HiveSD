// Local includes
#include "SctSD.h"

// Framework includes
#include "CxxUtils/make_unique.h"
#include "MCTruth/TrackHelper.h"

// Geant4 includes
#include "G4Step.hh"
#include "G4Geantino.hh"
#include "G4ChargedGeantino.hh"

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
SctSD::SctSD(const std::string& name)
  : G4VSensitiveDetector(name),
    m_hitColl("SCT_Hits")
{
}

//-----------------------------------------------------------------------------
// Initialize a G4 event
//-----------------------------------------------------------------------------
void SctSD::Initialize(G4HCofThisEvent* /*hce*/)
{
  // Prepare the hit collection. Is the correct usage??
  m_hitColl = CxxUtils::make_unique<SiHitCollection>();
}

//-----------------------------------------------------------------------------
// Process hits for this step
//-----------------------------------------------------------------------------
G4bool SctSD::ProcessHits(G4Step* aStep, G4TouchableHistory* /*hist*/)
{
  // The following overly large code was copied from SctSensorSD
  // in InnerDetector/InDetG4/SCT_G4_SD

  double edep = aStep->GetTotalEnergyDeposit();
  // WARNING: double precision comparison
  if(edep == 0.) {
    // Do we really use geantinos for something?
    if(aStep->GetTrack()->GetDefinition()!=G4Geantino::GeantinoDefinition() &&
       aStep->GetTrack()->GetDefinition()!=G4ChargedGeantino::ChargedGeantinoDefinition())
      return false;
  }
  edep *= CLHEP::MeV;
  //use the global time. i.e. the time from the beginning of the event
  //
  // Get the Touchable History:
  //
  G4TouchableHistory*  myTouch =
    (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());
  //
  // Get the hit coordinates. Start and End Point
  //
  G4ThreeVector coord1 = aStep->GetPreStepPoint()->GetPosition();
  G4ThreeVector coord2 = aStep->GetPostStepPoint()->GetPosition();
  //
  // Calculate the local step begin and end position.
  // From a G4 FAQ:
  // http://geant4-hn.slac.stanford.edu:5090/HyperNews/public/get/geometry/17/1.html
  //
  const G4AffineTransform transformation = myTouch->GetHistory()->GetTopTransform();
  G4ThreeVector localPosition1 = transformation.TransformPoint(coord1);
  G4ThreeVector localPosition2 = transformation.TransformPoint(coord2);

  HepGeom::Point3D<double> lP1,lP2;
  lP1[SiHit::xEta] = localPosition1[2]*CLHEP::mm;
  lP1[SiHit::xPhi] = localPosition1[1]*CLHEP::mm;
  lP1[SiHit::xDep] = localPosition1[0]*CLHEP::mm;

  lP2[SiHit::xEta] = localPosition2[2]*CLHEP::mm;
  lP2[SiHit::xPhi] = localPosition2[1]*CLHEP::mm;
  lP2[SiHit::xDep] = localPosition2[0]*CLHEP::mm;

  //
  // Get it into a vector in local coords and with the right units:


  // Now Navigate the history to know in what detector the step is:
  // and finally set the ID of det element in which the hit is.
  //
  //G4int History;
  //
  // Is it TB, barrel or endcap?
  //
  int brlEcap = 0;
  int layerDisk = 0;
  int etaMod = 0;
  int phiMod = 0;
  int side = 0;
  //
  // In the case of the TB the positioning integers won't be initialized
  // and the identifying integer will be zero. There is no need to do
  // anything else

  int sensorCopyNo =  myTouch->GetVolume()->GetCopyNo();

  // TODO: implement messaging
  /*if (msgLvl(MSG::VERBOSE)) {
    for (int i=0;i<myTouch->GetHistoryDepth();i++){
      std::string detname=myTouch->GetVolume(i)->GetLogicalVolume()->GetName();
      int copyno=myTouch->GetVolume(i)->GetCopyNo();
      ATH_MSG_VERBOSE( "Volume "<<detname << " Copy Nr. " << copyno );
    }
  }*/

  // Barrel geometry from GeoModel. Flag it with a 1000 in the IdTag
  //
  if(sensorCopyNo/1000) {
    // Barrel
    if(sensorCopyNo == 1000) {
      side = myTouch->GetVolume(1)->GetCopyNo();
      etaMod = myTouch->GetVolume(2)->GetCopyNo();
      phiMod = myTouch->GetVolume(3)->GetCopyNo();
      layerDisk = myTouch->GetVolume(5)->GetCopyNo();
    } else if(sensorCopyNo == 1100) {
      // SLHC stave layout
      int etaModTmp = myTouch->GetVolume(1)->GetCopyNo();
      int sign = (etaModTmp>=0)? +1 : -1;
      side = std::abs(etaModTmp)%2;
      etaMod = sign * std::abs(etaModTmp)/2;
      phiMod = myTouch->GetVolume(2)->GetCopyNo();
      layerDisk = myTouch->GetVolume(4)->GetCopyNo();
    } else if(sensorCopyNo/100 == 12) {
      // Segmented sensor (SLHC only)
      int iSegment = sensorCopyNo%100;
      int sensorEnvCopyNo = myTouch->GetVolume(1)->GetCopyNo();
      if (sensorEnvCopyNo == 1000) {
        side = myTouch->GetVolume(2)->GetCopyNo();
        etaMod = myTouch->GetVolume(3)->GetCopyNo() + iSegment;
        phiMod = myTouch->GetVolume(4)->GetCopyNo();
        layerDisk = myTouch->GetVolume(6)->GetCopyNo();
      } else if  (sensorEnvCopyNo == 1100) {
        // Stave layout
        int etaModTmp = myTouch->GetVolume(2)->GetCopyNo();
        int sign = (etaModTmp>=0)? +1 : -1;
        side = std::abs(etaModTmp)%2;
        etaMod = sign * std::abs(etaModTmp)/2 + iSegment;
        phiMod = myTouch->GetVolume(3)->GetCopyNo();
        layerDisk = myTouch->GetVolume(5)->GetCopyNo();
      } else {
        // TODO: messaging
        //ATH_MSG_ERROR( "Unrecognized geometry in SCT sensistive detector. " <<
        //               "Please contact maintainer of SCT Detector Description." );
      }
    } else {
      // TODO: messaging
      //ATH_MSG_ERROR( "Unrecognized geometry in SCT sensistive detector. " <<
      //               "Please contact maintainer of SCT Detector Description." );
    }
    // TODO: messaging
    //if (msgLvl(MSG::VERBOSE)) {
    //  ATH_MSG_VERBOSE( "In the SCT Barrel" );
    //  ATH_MSG_VERBOSE( "----- side       : " << side );
    //  ATH_MSG_VERBOSE( "----- eta_module : " << etaMod );
    //  ATH_MSG_VERBOSE( "----- phi_module : " << phiMod );
    //  ATH_MSG_VERBOSE( "----- layer      : " << layerDisk );
    //  ATH_MSG_VERBOSE( "----- barrel_ec  : " << brlEcap);
    //}
  } else {
    // Endcap geometry from GeoModel. Flag it with a 500 or 600 in the IdTag
    // The level in the hierarchy is different for different geometries
    // 500 For pre DC3
    // 600 For DC3 and later (including SLHC)
    int sensorCopyNoTest =  sensorCopyNo/100;
    if(sensorCopyNoTest == 5 || sensorCopyNoTest == 6) {
      int signZ = (coord1.z() > 0) ? 1 : -1;
      brlEcap = 2 * signZ;
      side      = myTouch->GetVolume(0)->GetCopyNo() % 2;
      if (sensorCopyNoTest == 5) { // DC2 and Rome
        etaMod    = myTouch->GetVolume(3)->GetCopyNo();
        layerDisk = myTouch->GetVolume(4)->GetCopyNo();
      } else { // DC3 and later and SLHC
        etaMod    = myTouch->GetVolume(2)->GetCopyNo();
        layerDisk = myTouch->GetVolume(3)->GetCopyNo();
      }
      int phiNumber = myTouch->GetVolume(1)->GetCopyNo();
      // Number of modules in ring and module which becomes 0 is
      // encoded in the copy number. This is in order to recreate
      // the correct id in the negative endcap.
      int maxModules = (phiNumber & 0x00ff0000) >> 16;
      // When phiMod = moduleZero -> 0 after inverting
      int moduleZero = (phiNumber & 0xff000000) >> 24;
      phiMod = phiNumber & 0x0000ffff;
      // If -ve endcap then invert numbering
      // maxModules is non-zero, but check for maxModules != 0  safeguards
      // against divide by zero (in case we create a geometry without the encoding).
      if (maxModules && signZ < 0)
        phiMod = (maxModules + moduleZero - phiMod) % maxModules;

      // Some printout
      // TODO: messaging
      //ATH_MSG_VERBOSE( "In the SCT EndCap" );
      //ATH_MSG_VERBOSE( "----- side       : " << side );
      //ATH_MSG_VERBOSE( "----- phi_module : " << phiMod);
      //ATH_MSG_VERBOSE( "----- eta_module : " << etaMod );
      //ATH_MSG_VERBOSE( "----- layerDisk  : " << layerDisk );
      //ATH_MSG_VERBOSE( "----- barrel_ec  : " << brlEcap );
    } else {
      // Do not expect other numbers.
      // Need to fix SCT_GeoModel or SCT_SLHC_GeoModel if this occurs.
      // TODO: messaging
      //ATH_MSG_ERROR( "Unrecognized geometry in SCT sensistive detector. " <<
      //               "Please contact maintainer of SCT Detector Description." );
      throw "UnrecognizedGeometryInSCTSD";
    }
  }

  // get the barcode from the trackhelper
  TrackHelper trHelp(aStep->GetTrack());
  int barcode = trHelp.GetBarcode();
  m_hitColl->Emplace(lP1,
                     lP2,
                     edep,
                     aStep->GetPreStepPoint()->GetGlobalTime(),
                     barcode,
                     1, brlEcap, layerDisk, etaMod, phiMod, side);

  return true;
}

//-----------------------------------------------------------------------------
// Finalize a G4 event
//-----------------------------------------------------------------------------
void SctSD::EndOfEvent(G4HCofThisEvent* /*hce*/)
{
  // TODO: finalize hit collection here.
  // Or maybe it's done automatically?
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
  // Default list of volumes
  m_volumeNames = {"SCT::BRLSensor","SCT::BRLSensorSS","SCT::BRLSensorMS",
                   "SCT::ECSensor0","SCT::ECSensor1","SCT::ECSensor2",
                   "SCT::ECSensor3","SCT::ECSensor4","SCT::ECSensor5"};
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
