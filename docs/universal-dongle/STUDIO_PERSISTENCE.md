# Universal dongle — Studio and nonvolatile storage isolation

Status: design contract / experiment, NOT yet active in firmware or the Studio UI.

## Mandatory guarantees
- Never write Hana settings when Chocofi is selected, or vice versa.
- A keyboard selector is **not** a Bluetooth host profile selector.
- USB HID stays a single keyboard to the host; Studio sees one dongle with two configuration namespaces.
- All edits, unsaved changes, saves, discards, restores, resets, combo edits, hold-tap parameter edits, layout shifts, and layer reorders must carry an explicit target board identity.
- The hardware-side active keyboard may automatically change due to failover; a Studio edit session must not silently follow that change. Pin editing to a board ID at session start and require explicit confirmation to switch the editor target.
- A switch during a save must serialize / finish the transaction for the original board (or return BUSY). Never write partially into a newly selected board.
- Use separate generation counters and CRC/checkpoint metadata for each board; recover the last valid record independently after unexpected power loss.
- Preserve existing `keymap/*`, `combos/*` and runtime behavior settings from the currently flashed single-board dongle. Never reuse or erase legacy settings automatically.
- Existing `settings_reset` and factory reset operations are **unsafe** for a multi-board dongle unless reset is scoped. Require confirmation for "both keyboards".

## Existing code
- ZMK `app/src/keymap.c` currently persists `keymap/l/%d/%d`, `keymap/l_n/%d` and `keymap/layer_order`.
- ZMK `app/src/combo.c` persists `combos/c/%d`.
- Runtime-editable behavior settings and layout shift persistence also need an audit and namespacing; keymap and combo isolation by itself is insufficient.
- Custom Studio repo: `Sukumar002/zmk-studio`. Existing backup importer/exporter: `src/backup/`; live device RPC: `src/rpc/`.

## Proposed on-flash settings keys (design, not implemented)
- `universal/v1/hana/keymap/...`
- `universal/v1/hana/combos/...`
- `universal/v1/hana/behaviors/...`
- `universal/v1/hana/layout_shift/...`
- `universal/v1/chocofi/keymap/...`
- `universal/v1/chocofi/combos/...`
- `universal/v1/chocofi/behaviors/...`
- `universal/v1/chocofi/layout_shift/...`
- `universal/v1/meta/selected_board` (separate from editor target).
- Explicit versioned migrations; don't silently import Hana into Chocofi or vice versa.

## Protocol proposal
- `get_capabilities` returns `universal_dongle: true`, protocol version, board list and position/layer capacities.
- `get_board_status` reports currently active board, peripheral readiness and failover reason.
- `select_editor_board(board_id)` pins subsequent read/write commands to a named board; does not change the typing board.
- `read_config(board_id)`, `set_binding(board_id,...)`, `set_combo(board_id,...)`, `set_behavior(board_id,...)`, `save(board_id,expected_revision)`, `discard(board_id)`, `reset_board(board_id)`. All writes require board ID AND revision.
- `select_typing_board(board_id)` is a distinct, guarded operation, executed with the HID-release switch protocol.
- Backup export adds board ID and schema version. Import refuses mismatched targets unless the user explicitly performs a mapped conversion.

## UI requirements
- Two clearly labelled tabs: "Hana — edit" and "Chocofi — edit", not tied to active typing board.
- Status indicator: "Typing: Hana/Chocofi/Waiting" and reason if fallback occurred.
- Unsaved-change indicator is per board. Switching tabs must never implicitly save or discard another board's data.
- Per-board positions, physical layout, layers, combos, behaviors, OS modifier display settings and separate backups.
- On a legacy/single-keyboard dongle, retain existing UI unchanged.

## Acceptance tests
1. Edit Hana layer, combo, modtap; save; switch typing board to Chocofi; confirm unchanged Chocofi after reconnect/reboot.
2. Edit Chocofi, cause failover mid-edit to Hana; save continues Chocofi or clearly returns BUSY, NEVER writes to Hana.
3. Kill power during each board's save, reboot and verify independent valid config.
4. Reset Chocofi alone; verify Hana intact.
5. Import Chocofi backup while editing Hana -> explicit mismatch confirmation; no silent import.
6. Multiple simultaneous Studio clients: reject stale revision / serialize writer.
7. No automatic settings reset, bond deletion or factory migration on older firmware.

## Dependency
The catalogue proof of concept stores both keyboard snapshots as *read-only program data*. It does not yet implement any runtime keymap selection or isolated persistence. The runtime-config API must be wired into the ZMK fork before the Studio UI can safely use it.
