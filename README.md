# G4HiveSD
Testing code for the multi-threaded sensitive detector design for ATLAS simulation with GaudiHive.

The primary change w.r.t. the (new) existing ATLAS simulation code is that the Geant4 part of the SD is split from the Athena tool part. In fact, the tool maintains a thread-specific pointer to the SD.

Thread safety is ensured by mapping threads to SD objects in a concurrent unordered map.

Classes:

* [ISensitiveDetectorTool](G4HiveSD/ISensitiveDetectorTool.h) - Tool interface
  for the SD tools
* [SensitiveDetectorToolBase](G4HiveSD/SensitiveDetectorToolBase.h) - Base class
  for the SD tools; holds common functionality for concurrency.
* [SctSD](src/SctSD.h) - Example G4 SD implementation taken from existing code.
* [SctSDTool](src/SctSD.h#L48) - SD tool wrapper for the SCT.
* [ISensitiveDetectorSvc](G4HiveSD/ISensitiveDetectorSvc.h) - Interface for the
  top-level SD service.
* [SensitiveDetectorSvc](src/SensitiveDetectorSvc.h) - Top-level SD service
  that manages the SD tools and delegates creation of the G4 SD objects.
