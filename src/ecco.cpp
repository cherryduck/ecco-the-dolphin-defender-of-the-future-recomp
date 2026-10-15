// Ecco the Dolphin: Defender of the Future: what this port tells dreamcomp. Game facts with sources: docs/GAME-INTERNALS.md.
#include "dreamcomp/hook.h"
#include "dreamcomp/port.h"

namespace {

const dreamcomp::PortInfo kInfo = [] {
    dreamcomp::PortInfo p;
    p.id = "ecco";
    p.title = "Ecco the Dolphin: Defender of the Future";
    return p;
}();
dreamcomp::RegisterPort g_register(kInfo);

}  // namespace
