# Universal dongle proof of concept — pre-hardware plan

Status: Architecture planning only. No universal firmware is implemented or flash-ready.

## Baseline
- Branch starts from `chocofi-setup` at `9af9934`; Hana `main` is untouched.
- Hana halves: `xiao_ble//zmk`; Chocofi halves: `nice_nano//zmk`.
- Both dedicated dongles: `nice_nano//zmk` with optional SSD1306 OLED.
- The current ZMK and display modules are pinned in `config/west.yml`. Keep the versions fixed during the first experiment.
- Chocofi build #3 passed compilation, **not** hardware or Studio acceptance testing.

## Non-negotiable prerequisites
1. Verify Chocofi PCB matrix, pin map, actual key count and thumb positions against the Corne-derived placeholder shield.
2. Fix the Chocofi keymap before flash testing: it currently assumes a 6-column Corne physical layout with duplicated edge positions, and references layers 3/4 without defining those layers.
3. Bring Chocofi to independent BLE/dongle operation and confirm correct bond persistence with its *own* central.
4. Validate the current Hana dongle firmware and display in actual hardware first; preserve a known-good backup and recovery/settings-reset image.
5. Confirm all four peripheral identities can be distinguished reliably. Do not rely only on BLE device names.
6. Document keyboard-specific transforms and sizes; Hana and Chocofi must not overlap in a combined global key-position namespace.
7. Audit RAM/flash and connection stability for four simultaneous BLE peripherals on the nice!nano-compatible clone.
8. Define safe behavior for idle, disconnect, reconnect, modifier release and switching initiated from the inactive keyboard.
9. Plan Studio isolation: which layout is reported, what layers/combos change at runtime, and what must persist separately. A physical layout switch by itself is **not** proof of two independent Studio profiles.

## Hypothesis to test
A persistent central may connect four split peripherals (two for each board), map their key positions into disjoint logical position ranges, and gate ordinary HID events based on an active keyboard ID. Explicit switching events must bypass the gate from either board.

This is a custom ZMK extension: increasing `CONFIG_ZMK_SPLIT_BLE_CENTRAL_PERIPHERALS` does not automatically supply identity mapping, event filtering, keyboard-profile switching, or separate Studio storage.

## Prototype gates
### P0 — static analysis
- Locate the ZMK central's peripheral discovery, identity, connection limit, event dispatch and keymap-position processing paths in our pinned fork.
- Document any assumptions about central/peripheral counts and how per-peripheral position offsets are applied.
- Choose whether to use all four simultaneous links (preferred first experiment) or dynamic reconnection.

### P1 — build only
- Add a `universal_dongle` shield and `build-universal-dongle.yaml` **only once** a coherent four-peripheral logical layout exists.
- Add a dedicated path-filtered workflow, not entries in Hana/Chocofi matrices.
- Record RAM/flash usage; maintain a separate settings-reset/recovery build.

### P2 — physical connections
- Pair Hana L/R and Chocofi L/R using a separate experimental dongle.
- Verify input identity and orderly reconnection after power cycling.
- No HID routing until identity mapping is proven.

### P3 — runtime keyboard selector
- Add `&keyboard_select 0`, `&keyboard_select 1`, optionally `&keyboard_next`.
- Permit selector commands on both boards even when inactive.
- Release pressed HID keys/modifiers before switching; suppress ordinary events from inactive board.
- Prefer disconnecting or suspending inactive peripherals later *only if* four-link performance proves unacceptable.

### P4 — Studio and OLED
- Prove layouts, combos, runtime layers, persisted settings and Studio edit operations are isolated.
- Add current keyboard identity and associated L/R battery/connection information on OLED.
- Regression-test Hana-only and Chocofi-only build workflows.

## Recovery and safety
- Never overwrite dedicated dongle firmware on this experimental branch.
- Do not merge into `main` until all physical gates pass.
- BLE bonds/settings may require reset when changing firmware personality; keep a recovery plan.
- Existing dedicated dongles remain the fallback during the experiment.
