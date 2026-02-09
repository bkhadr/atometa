#include "Atometa/Python/PyAtometa.h"
#include "Atometa/Core/Logger.h"
#include "Atometa/Chemistry/PeriodicTable.h"
#include <structmember.h>

namespace Atometa {
namespace Python {

void PyAtometa::Shutdown() {
    // Cleanup if needed
}

} // namespace Python
} // namespace Atometa

// ============================================================================
// Module Initialization (C interface)
// ============================================================================

PyMODINIT_FUNC PyInit_atometa(void) {
    return Atometa::Python::PyAtometa::Initialize();
}