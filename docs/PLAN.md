# Ecco the Dolphin: Defender of the Future port -- plan

## Standing constraints

- No game data in the repo (dreamcomp docs/LEGAL.md); audit pre-commit hook installed.
- One verified change at a time; enhancements behind settings, on once verified.
- No black bars; high frame rate never by changing game logic rate.

## Phases

| Phase | Entry gate | Exit criteria (visible behaviour) | Status |
|---|---|---|---|
| 00 Survey | disc present | identity, SDK, files recorded | |
| 01 Boot | config builds | menus reachable; 0 untranslated / 0 unmapped over 1800 frames | |
| 02 Audio | 01 | music + effects audible | |
| 03 Full play | 02 | complete playthrough, saves load | |
| 04 Enhancements | 03 | per dc-enhance | |

## Decisions

| Date | Decision | Options | Why | By |
|---|---|---|---|---|

## Verified checkpoints

| Date | Port commit | dreamcomp commit | What was confirmed, by whom |
|---|---|---|---|
