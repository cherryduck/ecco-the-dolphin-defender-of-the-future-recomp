# Ecco the Dolphin: Defender of the Future port -- plan

## Standing constraints

- No game data in the repo (dreamcomp docs/LEGAL.md); audit pre-commit hook installed.
- One verified change at a time; enhancements behind settings, on once verified.
- No black bars; high frame rate never by changing game logic rate.
- Target: Linux (CachyOS, g++ 16, Vulkan, Wayland), dreamcomp fork `cherryduck/dreamcomp` branch `linux-ecco`.

## Phases

| Phase | Entry gate | Exit criteria (visible behaviour) | Status |
|---|---|---|---|
| 00 Survey | disc present | identity, SDK, files recorded | done 2026-10-15 (`findings/phase-00.md`) |
| 01 Boot | config builds | menus reachable; 0 untranslated / 0 unmapped over menus and first gameplay | done 2026-10-15 except 1 unmapped write, which the interpreter makes too (guest behaviour, see GAME-INTERNALS) |
| 02 Audio | 01 | music + effects audible | done 2026-10-15 (cherryduck: green) |
| 03 Full play | 02 | all 39 levels (CONFIG.TXT) completable, saves load | done 2026-10-15 (cherryduck: playthrough green) |
| 04 Enhancements | 03 | widescreen, scale, idle skip, interpolation | widescreen Expanded up to 21:9 (projection hook, Hor+), HUD fix; scale works (engine); idle skip, interpolation not started |

## Decisions

| Date | Decision | Options | Why | By |
|---|---|---|---|---|
| 2026-10-15 | `link_address = 0x8C010000` | template's 0x0C010000 | CodeWarrior links at P1: 62,088 0x8C pointers vs 734 0x0C in the image | agent |
| 2026-10-15 | Split generated code 32 ways (`DREAM_GEN_SHARDS`, dreamcomp) | single 244 MB file | fresh build 514 s -> 70 s | agent |
| 2026-10-15 | Scenarios use the new-game path on a fresh card | select an existing save | scenario.py gives every run its own card; on the player's card AAA exists and the game refuses a duplicate | agent, after cherryduck's report |

## Verified checkpoints

| Date | Port commit | dreamcomp commit | What was confirmed, by whom |
|---|---|---|---|
| 2026-10-15 | (this commit) | linux-ecco | `scenario.py play --port ecco`: logos, title, Select VM, New Game, initials, main menu, prologue film, first controllable scene with HUD; 0 untranslated targets over 20000 frames; translated and `--interpret` TA dumps byte-identical at frame 13100 (gameplay). Agent, screenshots |
| 2026-10-15 | | | cherryduck watched the windowed runs; reported the duplicate-AAA refusal on his card |
| 2026-10-15 | | | Expanded aspect: Hor+ via `projection@entry`; agent checked menus and gameplay at 16:9 and 21:9 (shots), 19000 frames without a fault; in-game dialogue text at 21:9 not caught in a shot. cherryduck: Expanded was greyed out before (no `PortInfo::widescreen`) |
| 2026-10-15 | | ad01af3 | cherryduck reported: name-entry cursor off at the edge columns (widescreen), seam flicker when turning. Agent: cursor override, verified at 21:9; flicker traced to 120+ Hz interpolation, fixed in the engine, in-between frames of a fast turn clean. Not yet confirmed by cherryduck on his 240 Hz display |
| 2026-10-15 | | per-triangle sort | cherryduck: above-sea rock walls visible from under the sea, varying with camera. Agent: translucent sort order (engine), cliffs gone in A/B shots of the same frames. Not yet confirmed by cherryduck |
