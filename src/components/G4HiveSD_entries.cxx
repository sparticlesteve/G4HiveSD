#include "GaudiKernel/DeclareFactoryEntries.h"

#include "../SctSD.h"
#include "../SensitiveDetectorSvc.h"

DECLARE_TOOL_FACTORY( SctSDTool )

DECLARE_SERVICE_FACTORY( SensitiveDetectorSvc )

DECLARE_FACTORY_ENTRIES( G4HiveSD ) {
  DECLARE_TOOL( SctSDTool )
  DECLARE_SERVICE( SensitiveDetectorSvc )
}
