# Multi-board firmware architecture

The maintained keyboard firmware is organized as a multi-board workspace. Each keyboard shield owns its physical hardware description and keymap; common behaviors and dongle display functionality are consumed as shared Zephyr/ZMK modules.

## Supported boards

- **Hana** — Seeed XIAO nRF52840 halves and ProMicro/nice!nano-form-factor nRF52840 dongle.
- **Chocofi** — ProMicro/nice!nano v2 nRF52840 halves and dongle.
- **Sweep** — ProMicro/nice!nano v2 nRF52840 halves and dongle; its direct-scan pin order and original keymap are retained.
- **Corne** — ProMicro/nice!nano v2 nRF52840 halves and dongle.
- **Sofle** — ProMicro/nice!nano v2 nRF52840 halves and dongle.

The MCU mapping is a board profile, not a user-selectable build guess. Hana is the exception: its halves use `xiao_ble//zmk`, while its dongle uses `nice_nano//zmk`. Other supported boards currently use `nice_nano//zmk` for both halves and dongle.

## Standard firmware package

The manual **Build Complete Firmware Pack** workflow accepts one input: the board name. It builds the complete matrix in `build-<board>.yaml` and uploads one ZIP artifact containing:

- `<board> left central`
- `<board> left peripheral`
- `<board> right peripheral`
- `<board> dongle central 128x64`
- `<board> dongle central 128x32`
- `<board> dongle no display`
- `<board> settings reset`

Hana has two reset images instead of one:
- `hana halves settings reset` — XIAO nRF52840.
- `hana dongle settings reset` — ProMicro/nice!nano nRF52840.

The right-peripheral image is built once and is shared by BLE-central and dongle-central deployment. The package includes a build manifest with the source commit, MCU mapping, and actual firmware filenames.

Per-board build workflows remain available for pull-request and push validation. The complete-pack workflow is the single manual entry point for obtaining all deployment variants.

## Shared feature layer

The goal is for shield folders to contain hardware configuration, matrix/physical-layout metadata, and board-specific keymaps—not copies of feature implementations.

Already shared in the current workspace:
- GUI/Alt layout-shift behavior and its custom behavior implementation.
- Bluetooth profile hold behavior for profile selection and long-hold clear/pair operations.
- OLED on/off, BASE-view cycle, and timeout-cycle behaviors.
- Runtime-editable Mod-Tap and Layer-Tap defaults.
- Four ZMK Studio runtime-addable layer slots.
- Dongle display widgets and status sources, including battery, output, layer, modifier, WPM, Bongo Cat, timeout and host lock indicators.

Still requiring a deliberate extraction/compatibility pass:
- Common mouse-layer behavior bindings and per-keymap placement.
- Output-toggle binding and other shared utility bindings.
- A consistent feature configuration interface for all shield profiles.
- Display renderer parity between the 128x32 and 128x64 paths, including Hana's validated raw 128x32 path and the compact LVGL path used by other current targets.

## Unified default keymap

All supported shields use the same five default layers in the same order and with the same display labels:

| Index | Layer | Shared purpose |
|---:|---|---|
| 0 | Base | Board's normal typing layout and home-row/board-specific typing behaviors |
| 1 | Num | Number row, function keys and symbols |
| 2 | Nav | Cursor movement, navigation, paging and host/media controls |
| 3 | Set | Bluetooth profile selection/hold-to-clear, Bluetooth clear-all, output toggle, layout shift, OLED toggle, BASE-view cycle, timeout controls, bootloader and reset |
| 4 | Mouse | Pointer movement, scrolling and mouse buttons |

The physical binding positions are adapted to each shield's matrix and thumb cluster, but the layer indices and feature meanings are kept consistent. The original typing layout and board-specific hardware configuration are retained.

All five keymaps include `studio_features.dtsi`, `studio_runtime_layers.dtsi` and the single shared `studio_unlock_combo.dtsi`. Studio unlock is the Base-layer chord at physical key positions `0 1` on every shield. Do not add per-shield copies of this combo.

## Feature parity audit

The shared behavior definitions are now included by all five keymaps. Hana's board-local copies of the layout-shift map, Bluetooth profile hold-tap, OLED toggle, BASE-view toggle and timeout-cycle behavior have been replaced with the common definitions. Hana retains only its hardware/layout-specific keymap and hold-trigger-position overrides. Sweep also uses the shared BASE-view behavior instead of a duplicate local node.

The keymap bindings are not yet at full parity:
- **Hana and Sweep** expose the complete feature-control set in their keymaps, including output toggle and mouse controls.
- **Chocofi** includes the common feature definitions and Studio runtime layers, but its current keymap still uses direct Bluetooth selection and does not yet expose the full OLED/timeout/output/mouse control set.
- **Corne and Sofle** include the shared definitions and Studio runtime layers, but their imported keymaps still need feature-control bindings.

The next keymap pass should add a consistent feature/adjust layer and optional mouse layer without replacing each board's existing base typing layout. Key positions remain board-specific.

## Display integration status

All five complete build matrices include both 128×32 and 128×64 dongle targets. The current shared display dependency is pinned to the Sweep-tested module line. Hana's separately validated raw-framebuffer 128×32 candidate is on the dedicated `feature/hana-oled-refinement` branch and has not yet been reconciled with that shared display-module line. The complete-pack workflow must not be treated as release-ready for Hana's 128×32 target until that renderer integration is completed and built.

## Hardware-specific responsibilities

These remain in the individual board profile:
- MCU family and board target.
- Matrix wiring, transforms, GPIO assignments and I2C aliases.
- OLED controller, resolution, rotation/offset and display enablement.
- Battery/charge sensing capabilities.
- Keymap positions and which optional feature bindings are exposed.

A ProMicro footprint does not guarantee identical I2C, ADC, charge-detection or regulator wiring. Profiles must declare actual supported hardware rather than assume all clones are electrically identical.

## Project boundary

The experimental two-keyboard **Unified Dongle** router remains isolated in `Sukumar002/zmk-unified-dongle`. It is not part of the standard per-keyboard firmware package or the ordinary central-dongle targets.

## Validation gates

1. Every complete board matrix compiles and produces the expected UF2 count.
2. Studio-enabled central targets expose the intended RPC transport and runtime-editable behaviors.
3. Both OLED sizes render correctly on the intended dongle hardware.
4. Split BLE pairing, reconnection, battery telemetry, timeout behavior, and mouse behavior are hardware-tested.
5. Settings-reset images are tested against the matching MCU family.
6. A green build is compile evidence only; it is not a substitute for physical validation.
