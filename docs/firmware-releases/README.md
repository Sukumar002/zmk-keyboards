# Keyboard Firmware Releases

Keyboard firmware releases are tracked separately from ZMK Studio releases. Studio releases remain in the **zmk-studio** repository; keyboard firmware releases belong here in **zmk-keyboards**.

## Boards

- [Hana firmware releases](hana/README.md)
- [Chocofi firmware releases](chocofi/README.md) — no stable v1 release yet; work remains in progress.

## Versioning convention

Use board-prefixed release names and tags so the board is unmistakable:

- Hana: `Hana_zmk_dongle_vX.Y`
- Chocofi: `Chocofi_vX.Y`

GitHub's Releases page is shared by the repository, so the board-specific pages above provide the per-keyboard index. Each published release should include the source commit, firmware variants, build run, and tested status. Attach the firmware ZIP to the release so it remains available independently of Actions artifact retention.
