# Multi-board firmware architecture

The maintained firmware lives in one monorepo, with each keyboard isolated under `boards/shields/<board>/`.

## Board nodes

- **Hana** — hardware-tested reference implementation.
- **Chocofi** — BLE/dongle baseline prepared; hardware validation pending.
- **Corne** — imported from `Sukumar002/zmk-builder-reference` and adapted to the shared custom stack.
- **Sofle** — imported from `Sukumar002/zmk-builder-reference` and adapted to the shared custom stack.

## Per-board build outputs

Every board workflow produces one archive containing:

### BLE deployment
- `<board>_ble_left_central`
- `<board>_right_peripheral`

### Dongle deployment
- `<board>_dongle_left_peripheral`
- `<board>_right_peripheral` (same right-peripheral firmware; compiled once)
- `<board>_dongle_no_display`
- `<board>_dongle_128x32`
- `<board>_dongle_128x64`

### Recovery
- settings-reset firmware for the MCU family used by that board.

Hana additionally emits separate XIAO and nice!nano reset images because its halves and dongle use different MCU families.

## CI isolation

Each board has its own `build-<board>.yaml` and GitHub Actions workflow. Board-local changes build only that board. Shared custom-core-facing changes (common Studio helpers, west manifest, snippets, custom behaviors, display module integration, etc.) intentionally fan out to all affected board workflows.

## Shared feature layer

Chocofi, Corne, and Sofle include `boards/shields/common/studio_features.dtsi`, which exposes the same Hana-derived runtime features without replacing board-specific keymaps:

- runtime-editable Mod-Tap and Layer-Tap,
- runtime-editable Bluetooth Profile hold-tap,
- GUI/Alt layout-shift behavior,
- OLED/base-view/timeout custom behaviors,
- four Studio runtime-addable layer slots,
- custom Studio RPC through the central target,
- dongle display integration on OLED dongle targets.

Hana remains unchanged as the tested reference implementation.

## Deferred work

The one-dongle / multiple-split switching project is intentionally parked until all four boards have independently validated baseline firmware.
