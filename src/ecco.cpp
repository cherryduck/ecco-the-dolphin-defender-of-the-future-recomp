// Ecco the Dolphin: Defender of the Future (Europe, MK-5103350): what this port tells dreamcomp,
// and its game-specific enhancements. Addresses are guest addresses in the boot executable this
// port was translated from (game/ecco.toml, sha1_1st_read); docs/GAME-INTERNALS.md says how each
// was found.
#include <atomic>
#include <cstdint>

#include "dream/runtime/system.h"
#include "dreamcomp/hook.h"
#include "dreamcomp/port.h"

namespace {

// Widescreen (anamorphic, Hor+). Every camera projection goes through one builder,
// fn_8c040c04(camera r4, vec r5, x_scale fr4, y_scale fr5): it makes the projection matrix at
// camera+100 from the two scales and derives the camera's clip data from it. The callers compute
// x = cot(fov/2) * sqrt(3/4) and y = cot(fov/2) / sqrt(3/4) (fn_8c040c94, fn_8c040d84). Scaling
// only x by (4/3) / aspect widens the horizontal view and keeps the vertical one; the engine
// renders into a target of the window's shape, which undoes the squeeze at full resolution.
//
// Flycast's cheat for this disc gets close to the same picture at 16:9 only, by changing the
// aspect literals (0x8C040E68/0x8C040D1C, 0.75 -> 0.5625) and the constant term of the game's
// tan() rational approximation (0x8C275418), which alters tan() for every caller in the game.
// The hook leaves both alone.
std::atomic<float> g_x_scale{1.0f};

void widescreen(dream::System&, float aspect) {
    const bool wide = aspect > 4.0f / 3.0f + 0.01f;
    g_x_scale.store(wide ? (4.0f / 3.0f) / aspect : 1.0f, std::memory_order_relaxed);
}

const dreamcomp::PortInfo kInfo = [] {
    dreamcomp::PortInfo p;
    p.id = "ecco";
    p.title = "Ecco the Dolphin: Defender of the Future";
    p.widescreen = &widescreen;
    p.widescreen_anamorphic = true;
    p.max_aspect = 21.0f / 9.0f;  // verified shapes only (docs/GAME-INTERNALS.md)
    // The gameplay HUD (health/air bars and icons) is drawn as flat polygons, no sprites, at
    // 1/w 100 and 111.1; full-screen fades at 166.7 and 333.3; 3D geometry stays below 1.5
    // (ta_dump of gameplay frame 13100). Menus are different: their text and button icons are
    // quads in the 3D scene (1/w ~0.3), projected through the widened camera, so they already
    // keep their shape and must not be un-stretched again. Hence no shared-depth rule (it pulled
    // the menu prompt row apart at 21:9), only the overlay depth: 1/w >= 50 is the 2D HUD.
    p.hud.enabled = true;
    p.hud.polygons = true;
    p.hud.overlay_z = 50.0f;
    p.hud.min_shared = 1u << 30;
    p.players = 1;
    return p;
}();
dreamcomp::RegisterPort g_register(kInfo);

}  // namespace

DC_HOOK_ENTRY(projection) {
    // FPSCR.FR selects the bank fr[] mirrors; the game runs with FR = 0 (fr4 is the argument).
    if (!(c.fpscr & (1u << 21)))
        c.fr[4] *= g_x_scale.load(std::memory_order_relaxed);
    return false;  // run the builder with the adjusted scale
}
