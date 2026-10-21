# Hana Firmware Releases

This page tracks Hana firmware only. ZMK Studio versions are tracked in the separate `zmk-studio` repository.

## Published releases
- [Hana ZMK Dongle v1.0](https://github.com/Sukumar002/zmk-keyboards/releases/tag/Hana_zmk_dongle_v1.0) — initial dongle-central firmware release.
- [Hana ZMK v2.0](https://github.com/Sukumar002/zmk-keyboards/releases/tag/Hana_zmk_dongle_v2.0) — stable source checkpoint.
- [Hana ZMK Dongle v2.1](https://github.com/Sukumar002/zmk-keyboards/releases/tag/Hana_zmk_dongle_v2.1) — unified macOS/Windows layout and OLED modifier labels.

## Current candidate — v2.2
- [Build #115 on main](https://github.com/Sukumar002/zmk-keyboards/actions/runs/39232377870)
- Source commit: `aef3d79ef68355f6cc8ba766f1a53213672ad7e5`
- Candidate artifact: `hana-firmware`, SHA-256 `d137bf8c7340b88652df777562a1e3c9ac2e2c721c7a092eae40879941254d8b`
- [v2.2 release-candidate notes](v2.2-draft.md)

Build #115 passed the listed Hana build jobs. Physical testing is still pending, so v2.2 is not yet marked stable or published. Build #121 on `feature/chocofi-hana-parity` is Chocofi parity work and must not be used as the Hana v2.2 source.

## v2.2 release checklist
- [x] Successful build of the intended Hana variants.
- [x] Record source commit, build run, artifact ID, and artifact checksum.
- [ ] Test the new 128×32 dongle with both Hana halves.
- [ ] Complete functional/OLED/Studio checks listed in the candidate notes.
- [ ] Attach the tested ZIP to the GitHub Release.
- [ ] Publish the board-specific tag `Hana_zmk_dongle_v2.2` only after validation.
