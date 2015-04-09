#include "GaudiKernel/DeclareFactoryEntries.h"

#include "../SctSD.h"

DECLARE_TOOL_FACTORY( SctSDTool )

DECLARE_FACTORY_ENTRIES( G4HiveSD ) {
  DECLARE_TOOL( SctSDTool )
}
