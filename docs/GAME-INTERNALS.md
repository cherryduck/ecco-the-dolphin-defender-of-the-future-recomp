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
