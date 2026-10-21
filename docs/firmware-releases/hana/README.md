# Hana Firmware Releases

This page tracks Hana firmware only. ZMK Studio application releases are tracked separately.

## Published releases
- [Hana ZMK Dongle v1.0](https://github.com/Sukumar002/zmk-keyboards/releases/tag/Hana_zmk_dongle_v1.0) — initial dongle-central firmware release.
- [Hana ZMK v2.0](https://github.com/Sukumar002/zmk-keyboards/releases/tag/Hana_zmk_dongle_v2.0) — stable source checkpoint.
- [Hana ZMK Dongle v2.1](https://github.com/Sukumar002/zmk-keyboards/releases/tag/Hana_zmk_dongle_v2.1) — unified macOS/Windows layout and OLED modifier labels.

## v2.2 release candidate — selected known-good build
- **Selected source:** build #109, commit `3073b77b9eab1da27f2dcb5d6767982a31f5b4d1`
- [Build #109](https://github.com/Sukumar002/zmk-keyboards/actions/runs/39062257298)
- Artifact ID: `12111403214`; SHA-256: `01febca8d622c67dc9c75cce3360a1fbe6904979328c19155a92c81bb048a606`
- [v2.2 release notes](v2.2-draft.md)

Build #109 is the user's latest physically tested and working Hana firmware, so it is selected over #115. This release is Hana-only and does not claim universal-dongle support for sharing one dongle between Hana and Chocofi.

## Publication checklist
- [x] Successful build of the selected Hana firmware variants.
- [x] User-confirmed physical testing of build #109 on Hana.
- [x] Source commit, artifact ID, and SHA-256 recorded.
- [ ] Attach the exact build #109 ZIP to the GitHub Release as `Hana_v2.2_build109.zip`.
- [ ] Publish the GitHub release/tag `Hana_zmk_dongle_v2.2`.
