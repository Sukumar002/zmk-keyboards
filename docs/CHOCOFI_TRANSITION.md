# Chocofi transition and shared-dongle evaluation

Status: Chocofi Actions run #3 (39068322313) passed all eight targets on chocofi-setup. This is a compilation milestone, **not** verified keyboard operation.

## Hardware targets
| Component | Hana | Chocofi |
|---|---|---|
| Left/right halves | xiao_ble//zmk | nice_nano//zmk |
| Dongle | nice_nano//zmk | nice_nano//zmk |
| Display | Dongle OLED module | Same module, keyboard-specific firmware |

## Feature parity audit (code review, not hardware-tested)
- [x] Shared custom ZMK fork and dongle-display module via config/west.yml
- [x] BLE-central and dongle-central build variants compile
- [x] Studio RPC enabled for Chocofi centrals in build matrix
- [ ] Verify actual 5-column Chocofi PCB and matrix wiring; current baseline is Corne 6-column shape with repeated outer key bindings
- [ ] Adapt keymap to Chocofi's verified 5-column/three-thumb layout before flashing
- [ ] Port Hana's custom Studio runtime-addable layer slots, unlock binding and meaningful initial layers
- [ ] Port OS GUI/Alt layout shift, dynamic layer controls, mod-tap timing and macros
- [ ] Port pointing/mouse key bindings (CONFIG_ZMK_POINTING enabled, but Chocofi keymap lacks mouse layer)
- [ ] Port BT profile hold/tap behavior, OLED toggle, timeout cycling and display view controls
- [ ] Audit combos, tap/hold features and media keys after key position mapping is established
- [ ] Verify Studio menus (layer add/remove, combos, mouse settings) in actual device
- [ ] Pair and test BLE halves; separately pair two dongle peripherals
- [ ] Test sleep, battery status, USB modes and settings reset recovery
- [ ] Merge only after physical acceptance tests

## Dongle options
1. Separate Hana/Chocofi dongles — supported architecture, least risk, different layout firmware.
2. One *physical* dongle, manually reflashed to the selected keyboard personality — feasible in principle but stored split BLE bonds and host state require careful migration/reset; inconvenient, not seamless.
3. One multi-keyboard dongle with runtime keyboard selection — experimental custom central firmware. Requires segregated BLE peripheral bonds for four halves, keyboard-specific transforms/keymaps, switch logic, Studio active-layout metadata and display state. Changing host Bluetooth profiles alone does not implement it.

Recommended sequence: finish dedicated Chocofi mode first, build and pair separate dongle, then prototype a multi-keyboard dongle on a spare controller without changing the working Hana or Chocofi images.

Important: ZMK's `CONFIG_ZMK_SPLIT_BLE_CENTRAL_PERIPHERALS` specifies the expected simultaneous peripherals, not a supported runtime picker for two distinct keyboards. Do not simply set it to 4 as an assumed solution.

Sources: https://zmk.dev/docs/features/split-keyboards and https://zmk.dev/docs/hardware-integration/dongle
