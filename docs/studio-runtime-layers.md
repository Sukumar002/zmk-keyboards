# ZMK Studio runtime layers

This fork reserves runtime-addable layer capacity explicitly in each keyboard keymap.

## Shared helper

Use:

```dts
#include "../common/studio_runtime_layers.dtsi"
```

from a shield keymap under `boards/shields/<keyboard>/`, then add this inside the
`keymap { ... };` node after the normal layers:

```dts
ZMK_STUDIO_RUNTIME_LAYER_SLOTS_4
```

This creates four compile-time reserved layer slots. When Studio RPC is enabled,
ZMK includes those reserved children in `ZMK_KEYMAP_LAYERS_LEN` but leaves them
out of the initial layer order. The keymap RPC reports them as
`available_layers`, allowing ZMK Studio to add/remove layers dynamically.

Hana already uses this helper.

When the Chocofi shield/keymap is added to this repository, include the same
helper and macro in its keymap so it exposes the same four runtime slots.

## Required firmware capability

The Studio-enabled central build must use:

```yaml
snippet: studio-rpc-usb-uart
cmake-args: -DCONFIG_ZMK_STUDIO=y
```

Studio RPC selects `CONFIG_ZMK_KEYMAP_LAYER_REORDERING` and
`CONFIG_ZMK_KEYMAP_SETTINGS_STORAGE`, which provide add/remove/move/restore
and persistence support.
