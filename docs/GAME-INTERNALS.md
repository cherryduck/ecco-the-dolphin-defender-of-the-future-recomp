# Ecco the Dolphin: Defender of the Future -- game internals

Facts about the game, measured by this port, with how each was found.

## Binary

| Fact | How found |
|---|---|
| Linked at `0x8C010000` (P1), not `0x0C010000` | word scan of `0.BIN`: 62,088 pointers into the image as 0x8C, 734 as 0x0C |
| Static constructors: ~81 functions called once each from `0x8C0171AE` during boot | `--suggest-config`, all "in the image, discovery never reached it" |
| Katana stubs copied to `0x8C000100`/`0x400` (56 bytes from `0x8C2BF304`) and `0x8C000600` (44 bytes from `0x8C2BF338`); VBR 0x8C000000 | `--suggest-config` relocation candidates; the 0x600 copy is under the suggester's 48-byte minimum and came from the report's "bytes match" line |
| Widescreen literals `0x8C040E68`, `0x8C040D1C` (0.75) are reached by `mova`, so the translator reads them at run time (not folded) | `gen/ecco.cpp`: `c.r[0] = 0x8c040e68u;` then a memory read |
| Every function's entry FPSCR mode is ambiguous (0 inferred / 17,604): 217k runtime FP branches, 4,808 entry-mode clones | `gen/ecco.report.txt`; performance candidate |
| One 32-bit write to `0x8BFFF1C0` (below RAM) at frame 275 from the block at `0x8C2BEC16` (a list walk with SR masking) | report "unmapped"; the interpreter makes the same write, so it is the game's, not the translator's |

## Flow (guest frames, fresh card)

| Frame | Screen |
|---|---|
| ~200-700 | SEGA, Appaloosa logos |
| ~1100 | title, PRESS START BUTTON |
| 1300 Start | Select VM (A) |
| 1500 A | Select a Game: New Game, then any saves (AAA on the owner's card; Down selects it) |
| 1700 A | Enter Your Initials; A x3, Start = END. An existing name is refused |
| ~2400 | main menu: Start Game / Load Game / Options / VM Menu |
| 2600 A | "prologue" film (letterboxed), ~2300-11000 |
| ~11500 | first controllable scene, HUD (health, air); tutorial dolphin dialogue from ~13000 (A advances) |

No 50/60 Hz prompt appeared on this PAL disc with the default (PAL) flash.

## Widescreen (2026-10-15)

| Fact | How found |
|---|---|
| All camera projections go through `fn_8c040c04(camera r4, vec r5, x_scale fr4, y_scale fr5)`; it builds the matrix at camera+100 and the clip data | callers `fn_8c040c94`/`fn_8c040d84` load the cheat's 0.75 literals and pass `cot(fov/2)*sqrt(0.75)` / `cot(fov/2)/sqrt(0.75)` (disasm; `0x8C0168B0` is `fsqrt`, `0x8C0170F4` the tan routine) |
| Flycast's `0x8C275418` is the constant term of the tan() rational approximation (two neighbouring 2027025.0 words), so its cheat changes tan() everywhere | disasm of `0x8C014B34` pool |
| Port: entry hook scales fr4 by (4/3)/aspect = Hor+; vertical FOV unchanged; nothing culled at the edges at 21:9 | `scenario.py play --aspect 21:9` vs 4:3, frames 11500-19000 |
| Gameplay HUD = flat polygons at 1/w 100 / 111.1 (fades 166.7, 333.3); 3D below 1.5 | `dreamcomp_ta_dump`, guest frame 13100 |
| Menu text and button prompts are quads in the 3D scene (1/w ~0.3) and follow the widened camera, so they must not be un-stretched: the shared-depth HUD rule is off (`min_shared` huge), only `overlay_z = 50` | 21:9 menu shot: with the rule, the A icon landed on "SELECT" |
| Menu cursor (rounded box, 1/w 100, texture `0x0c0d25f0`) is a 2D overlay over 3D-quad items; kept centred by `game/hud_overrides.ini` | cherryduck: box beside the outer letter columns of Enter Your Initials at widescreen; fixed and checked at 21:9 (A and G columns) |
| Terrain is many strips per texture, culled per strip; near rock walls are re-clipped every frame. Both broke geometry interpolation (seam flicker in turns at >60 Hz) until pairing used texture coordinates (dreamcomp playbook T29) | `DREAM_SHOT_INTERP=1` in-between frames of a fast turn |

