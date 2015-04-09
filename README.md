# G4HiveSD
Testing code for the multi-threaded sensitive detector design for ATLAS simulation with GaudiHive.

The primary change w.r.t. the (new) existing ATLAS simulation code is that the Geant4 part of the SD is split from the Athena tool part. In fact, the tool maintains a thread-specific pointer to the SD.

Thread safety is ensured by mapping threads to SD objects in a concurrent unordered map.

Classes:

* [ISensitiveDetectorTool](G4HiveSD/ISensitiveDetectorTool.h)
* [SensitiveDetectorToolBase](G4HiveSD/SensitiveDetectorToolBase.h)
* [SctSD](src/SctSD.h)
* [SctSDTool](src/SctSD.h#L48)
