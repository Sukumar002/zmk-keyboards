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
