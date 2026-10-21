# Hana Firmware Releases

This page tracks Hana firmware only. ZMK Studio release versions are maintained in the separate `zmk-studio` repository.

## Published releases

- [Hana ZMK Dongle v1.0](https://github.com/Sukumar002/zmk-keyboards/releases/tag/Hana_zmk_dongle_v1.0) — initial dongle-central firmware release.
- [Hana ZMK v2.0 — Stable Feature Checkpoint](https://github.com/Sukumar002/zmk-keyboards/releases/tag/Hana_zmk_dongle_v2.0) — stable source checkpoint; its notes explicitly distinguish source restoration from final firmware/hardware validation.
- [Hana ZMK Dongle v2.1](https://github.com/Sukumar002/zmk-keyboards/releases/tag/Hana_zmk_dongle_v2.1) — unified macOS/Windows layout and OLED modifier labels.

> These historical GitHub Releases currently have no attached binary assets. For a quick firmware download, use a successful Actions run artifact while it remains available, and attach the final ZIP to the next published release.

## Latest build references

- [Hana firmware build #121](https://github.com/Sukumar002/zmk-keyboards/actions/runs/39480537830) — successful build on the `feature/chocofi-hana-parity` pull-request branch, source commit `6dde3788eb730deafa234982444dc930351bb89a`. This is a candidate build, not yet a stable release.
- [Last successful Hana build on main: #115](https://github.com/Sukumar002/zmk-keyboards/actions/runs/39232377870) — source commit `aef3d79ef68355f6cc8ba766f1a53213672ad7e5`.

## Next stable release

A new Hana release should use the next version number, rather than reusing v1.0 (which already exists). Candidate notes should cover the unified BASE layout, Mac-primary layout shift, preserved combos and hold-tap behavior, NUM/NAV/SET/MOUSE layers, mouse and Bluetooth controls, OLED status and modifier labels, dongle/peripheral roles, timer/sleep settings, and battery display behavior. Only mark each behavior as included after confirming it against the exact release commit and firmware artifact.

Release checklist:
- [ ] Select a source commit on the intended release branch.
- [ ] Build all intended Hana firmware variants successfully.
- [ ] Verify the actual artifact filenames and MCU/role variants.
- [ ] Hardware-test the release candidate and preserve the previous known-good firmware.
- [ ] Attach the firmware ZIP to the GitHub Release.
- [ ] Record the source commit and build run in the release notes.
