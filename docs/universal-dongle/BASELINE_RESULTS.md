# Baseline results — nRF52840 nice_nano//zmk

GitHub Actions successful run: https://github.com/Sukumar002/zmk-keyboards/actions/runs/39482702615

These are **linker-reported application image region usage**, not UF2 package sizes or total silicon capacity.

| Build | FLASH used | FLASH region | FLASH used % | RAM used | RAM region | RAM used % |
|---|---:|---:|---:|---:|---:|---:|
| Hana central + 128x64 OLED + Studio | 428,764 B | 792 KiB | 52.87% | 121,212 B | 256 KiB | 46.24% |
| Chocofi central + 128x64 OLED + Studio | 416,324 B | 792 KiB | 51.33% | 119,684 B | 256 KiB | 45.66% |

Source logs: Hana build job 118835143606; Chocofi build job 118835143851.

Flash headroom: Hana 382,244 B; Chocofi 394,684 B (792*1024 region).
RAM headroom: Hana 140,932 B; Chocofi 142,460 B.

**Do not add baseline sizes to estimate combined firmware.** Shared display, USB, Studio, BLE and ZMK code will be deduplicated but the new peripheral identity, configuration storage and runtime switching infrastructure adds data, code and stack requirements.

**Next gate:** compile a true combined-image proof of concept, then capture new linker size. BLE pairing is postponed until dual runtime keymap isolation and the memory results are acceptable.

## 2026-10-21 experimental integration

Run: https://github.com/Sukumar002/zmk-keyboards/actions/runs/39492082820

| Target | Flash (792 KiB region) | RAM (256 KiB region) |
|---|---:|---:|
| Chocofi + read-only Hana/Chocofi catalogue | 422,448 B (52.09%) | 119,684 B (45.66%) |
| Same + experimental board-isolated settings loader and router probe | 423,024 B (52.16%) | 120,796 B (46.08%) |

The two experimental options cost 576 B additional flash and 1,112 B RAM beyond the catalogue-only build. This is **not** a dual-keyboard runtime or full configuration storage implementation. Unit tests and CI compilation do not prove flash persistence, BLE pairing preservation, HID-release correctness or physical keyboard switching.

The complete build matrix succeeded, including the integration compilation target. The artifact merge may complete later.
