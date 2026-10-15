# Phase 00 -- survey (2026-10-15)

Measured with `dcdisc info`, `dcdisc ls`, `dcdisc inspect` and `strings` on the boot file, on the
owner's Redump dump "Ecco the Dolphin - Defender of the Future (Europe) (En,Fr,De,Es)" (cue + 5 bins).

## Identity

| | |
|---|---|
| Product | MK-5103350 V1.003, released 2000-05-09, region Europe |
| Boot file | `0.BIN` (IP.BIN name; not `1ST_READ.BIN`), 2,969,164 bytes, stored plain |
| Boot SHA-1 | `e6b209962f6d3d9565a4a6f8d430f1e921d264c0` |
| Windows CE | no |
| Peripherals (IP.BIN) | VGA, Puru Puru, VMU, analog stick, analog L/R triggers |

## Tracks

| # | LBA | Sectors | Type |
|---|---|---|---|
| 1 | 0 | 526 | data |
| 2 | 676 | 5950 | audio |
| 3 | 45000 | 46615 | data (filesystem root) |
| 4 | 91765 | 3802 | audio (inside the HD area) |
| 5 | 95792 | 453358 | data (most files live here) |

## Toolchain fingerprint (strings in `0.BIN`)

| Component | Version |
|---|---|
| Compiler | CodeWarrior (banner) -- first CodeWarrior title for dreamcomp |
| Shinobi | 1.43 (Jul 23 1999) |
| Graphics | Kamui2 16.3.2.4 (Jul 23 1999); no Ninja |
| Sound | Sega AUDIO64 driver (`AUDIO64.DRV`, `ac*` API), `midida.drv` referenced |
| CRI | ADXT 5.76, ADXF 5.55, SJ 5.52, MWRNA/MWSND 1.30 |

## Files

- Only code file: `0.BIN`. No SH-4 overlays found.
- `CONFIG.TXT`: plain-text level table (start/gameover/loader scenes and 39 levels) read at boot.
- `*_DEMO.INP` (+ one `.GSG`): attract-mode input replays.
- 37 `MUSIC_*.AFS` streams = 364 MB of 886 MB; `SOUND/*.KAT` sound banks; `*.GDE` level data.

## Prior work

- Flycast `core/cheats.cpp` widescreen entry for `MK-5103350`: `0x8C275418 = 0x49D9A5DA`,
  `0x8C040E68 = 0x3F100000`, `0x8C040D1C = 0x3F100000`. The last two are literal-pool words in
  `0.BIN` that hold 0.75 (3/4) between code pointers, so a folded literal would ignore a runtime
  patch (check the generated code before relying on it).
- No decompilation known.
