# Shared multi-shield firmware contract

This directory contains the shared ZMK behaviors and Studio helpers used by
Hana, Chocofi, Sweep, Corne, and Sofle. Keep board geometry and physical
switch positions in each board's keymap; keep common runtime behavior here.

## Shared keymap expectations

All five shields include:

- The default layers named `Base`, `Num`, `Nav`, `Set`, and `Mouse`.
- The shared Studio unlock combo on physical positions `0 + 1`, active on
  the Base layer.
- Four reserved ZMK Studio runtime layer slots.
- Shared Set-layer controls for Bluetooth profiles and clear-pair, output
  selection, layout shift, OLED toggle, base-view cycle, display timeout,
  and bootloader/reset actions.

The keymap layout and binding positions remain board-specific because the
boards have different physical geometries. Hana's additional editable
behavior metadata is intentional and does not change the shared five-layer
contract.

## Shared dongle display contract

The dongle display module is pinned by `config/west.yml` and provides both
display paths:

- **128x32:** standalone Zephyr framebuffer renderer selected by
  `CONFIG_DONGLE_DISPLAY_RAW_DASHBOARD=y`. The shared base-view behavior
  cycles between the compact dashboard and the modifier-family view.
- **128x64:** LVGL status-screen renderer. The shared base-view behavior
  cycles through Bongo Cat, bordered BASE text, and WPM; non-base layers show
  the active layer name.

Both variants use the shared `&base_view_tog_shared` behavior from the Set
layer. The behavior resolves to the renderer supplied by the selected
`dongle_display` build, and remains weakly linked for non-display targets.

The SSD1306 controller overlay stays in each board's shield directory so
panel-specific initialization properties are not forced onto other boards.

## Complete firmware matrices

Each `build-<board>.yaml` matrix includes the board's settings-reset image,
left-central BLE image, right peripheral, left peripheral, no-display dongle
central, and 128x32 and 128x64 dongle-central images. Hana additionally
builds separate settings-reset images for its XIAO halves and ProMicro-form
factor dongle.

Successful CI compilation validates the firmware matrix, not physical OLED
startup or display quality. Those still require hardware testing.
