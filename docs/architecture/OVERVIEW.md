# Shared Dongle Architecture — Initial System Contract

Status: architecture baseline, implementation not yet complete  
Scope: Chocofi-first integration, then Hana, Sweep, and Sofle  
Rule: make the complete path work first; improve individual features without changing unrelated contracts.

## 1. System boundaries

The system is composed of five independently versioned areas. Existing repositories are retained; no keyboard-specific dongle repository is introduced.

| Area | Repository | Owns | Must not own |
|---|---|---|---|
| Firmware platform | `Sukumar002/zmk` | ZMK/Zephyr integration, custom Studio RPC and firmware-side runtime features, shared firmware APIs | Board pin maps, keymaps, OLED layout |
| Dongle core | `Sukumar002/zmk-dongle-core` (new repository) | Physical peripheral identity, connection lifecycle, stable LEFT/RIGHT slots, battery state model and events | Rendering, fonts, keyboard layouts, Studio UI |
| Dongle display | `Sukumar002/zmk-dongle-display` | Display-independent status model consumption, widgets, timeout feedback, resolution-specific rendering | BLE identity discovery, connection-order assumptions, keymaps |
| Keyboard integrations | `Sukumar002/zmk-keyboards` | Shields, pin matrices, keymaps, board/controller configuration, build matrices and integration tests | Reimplementing shared core or display logic |
| Studio | `Sukumar002/zmk-studio` plus its existing client/messages repositories | UI, editing workflows, protocol/client definitions | Firmware timing authority or dongle battery/side identity |

The firmware platform is the common substrate. The dongle core and display are separate Zephyr/ZMK modules. Keyboard repositories consume pinned module revisions through their west manifest. Studio communicates only through the versioned Studio protocol.

## 2. Dependency direction

Allowed dependency direction:

```text
Keyboard integration
  ├── Firmware platform
  ├── Dongle core
  └── Dongle display ── consumes core's public status contract

Studio frontend ── Studio client/messages ── firmware Studio RPC
```

The display may depend on the core's public data contract, but the core must never depend on display code. Keyboard shields configure hardware and identify their own physical side; they do not implement shared connection/battery bookkeeping.

No module may silently follow a moving `main` branch. Every integration build pins exact revisions for the firmware platform, core, display, and Studio protocol dependencies.

## 3. Core public contract (first implementation target)

The core publishes one authoritative state per physical peripheral:

- `side`: `LEFT`, `RIGHT`, or `UNKNOWN`
- `connection`: `DISCONNECTED`, `CONNECTING`, or `CONNECTED`
- `battery`: percentage plus an explicit validity/presence flag
- `charging`: explicit state where the source can report it
- `identity_valid`: true only after the physical side has been positively established

Rules:

1. LEFT and RIGHT are physical identities, never connection-order indexes.
2. A peripheral with unknown identity must not populate either visible battery slot.
3. Duplicate claims for the same physical side are rejected or quarantined; they must not overwrite a valid slot.
4. Disconnecting a peripheral clears its connected state immediately. Its last battery value may be retained only as stale data and must be marked unavailable/stale, never presented as a live reading.
5. Battery zero is a valid reading only when the battery source is known to be present. Missing battery, unavailable reading, and 0% are distinct states.
6. The central/dongle battery is a separate optional source; it is not LEFT or RIGHT.
7. State changes are emitted as typed events or exposed through a stable read-only API. Consumers do not inspect private ZMK split internals.
8. The identity transport/mechanism must be proven against the supported split firmware before the API is treated as production-ready. Never substitute connection order as a fallback.

Initial API shape (names illustrative until implementation is checked against the pinned ZMK APIs):

```c
enum zmk_dongle_side {
    ZMK_DONGLE_SIDE_UNKNOWN = 0,
    ZMK_DONGLE_SIDE_LEFT,
    ZMK_DONGLE_SIDE_RIGHT,
};

struct zmk_dongle_peripheral_state {
    enum zmk_dongle_side side;
    bool identity_valid;
    bool connected;
    bool battery_present;
    bool battery_valid;
    uint8_t battery_percent;
    bool charging;
};
```

## 4. Display contract

The display consumes the core's public state and renders it. It does not infer LEFT/RIGHT from array positions or BLE connection order.

- One shared logical status model feeds independent 128x64 and 128x32 renderers.
- The renderers are designed for their actual dimensions; 128x32 is not a vertically squeezed 128x64 layout.
- Missing/disconnected peripherals have an explicit visual state. Stale battery values are not presented as current.
- Timeout changes are transient, full-screen feedback, centered horizontally and vertically, and restore the normal screen after the configured duration.
- The normal screen and timeout overlay have separate layout responsibilities.
- The first 128x32 screen is deliberately minimal: USB/BT connection indicators on the left and LEFT/RIGHT battery percentages stacked vertically on the right. No layer, WPM, modifier, or decorative content is required for the first 32px milestone.
- The 128x64 renderer can retain its richer widgets, but it must use the same core status semantics.

The existing display module currently contains battery-source bookkeeping alongside LVGL rendering. During extraction, move source identity, validity, and connection lifecycle into core; keep drawing and layout in display. Do not rewrite stable 128x64 visuals merely to share code with the 32px renderer.

## 5. Studio and timing boundary

Studio remains a separate application and protocol consumer. Runtime remapping features may be exposed where supported by the firmware, but firmware owns behavior timing.

For the first Chocofi integration:

- Mod-Tap and Layer-Tap timing values are firmware-defined.
- Studio runtime timing overrides are disabled and are not transmitted to or applied by the dongle.
- The validated Chocofi timing profile is preserved; the first integration must not trade typing behavior for display/battery features.
- Studio's existing combo/custom-behavior workflows remain independent of dongle core/display state.
- Board-specific behavior metadata and protected controls remain keyboard integration concerns.

## 6. Integration and release strategy

1. Freeze the public core state contract and event semantics.
2. Extract/implement core identity and battery reporting against the pinned firmware platform.
3. Adapt the display to consume core state; retain separate 128x64 and 128x32 renderers.
4. Integrate Chocofi first, using the known-good 128x64 path as the first hardware reference and the 128x32 renderer as a separately gated target.
5. Run the complete firmware build matrix before producing artifacts.
6. Port the same contracts to Hana, then Sweep and Sofle, without copying core/display implementation into their shields.
7. Release each module independently and update keyboard integrations only by explicit pinned revision changes.

Use one consolidated firmware build workflow with independent board/build matrices and clearly isolated artifacts. Each release records: module version, source commit, firmware platform commit, Zephyr/ZMK manifest revisions, build matrix, and artifact names. A keyboard release is reproducible from its manifest alone.

## 7. First-build quality gates

A successful compile is necessary but not sufficient. The first candidate must pass:

- Clean build for all Chocofi central/peripheral targets and reset target.
- Correct physical LEFT/RIGHT identification regardless of connection order and reconnect order.
- Correct battery-to-side mapping after cold boot, disconnect, reconnect, and one-side-at-a-time power cycling.
- No phantom dongle battery when powered only over USB without a battery installed.
- No false 0% display for an absent/unavailable battery.
- 128x64 layout remains readable and correctly positioned.
- 128x32 layout remains legible, with no compressed/centered strip or overlap.
- Timeout feedback is centered, full-screen, transient, and restores the normal screen.
- Mod-Tap/Layer-Tap behavior matches the firmware timing profile; Studio cannot alter it.
- Studio connection, remapping, combo editing, and protected controls remain functional where enabled.
- No module consumes an unpinned dependency revision.

Hardware-only assertions are explicitly marked as pending until tested. CI must not label hardware behavior as validated merely because the firmware compiled.

## 8. Immediate work queue

- [ ] Create the dedicated `zmk-dongle-core` repository and establish its minimal module skeleton.
- [ ] Write and test the side-identity/battery contract before adding UI dependencies.
- [ ] Refactor `zmk-dongle-display` to consume the core contract.
- [ ] Remove moving upstream dependencies from the display module's own west manifest; the consuming keyboard manifest owns the firmware revision.
- [ ] Add a dedicated Chocofi architecture/integration branch and pin all module revisions.
- [ ] Establish CI compile gates and publish the first test artifacts without changing protected default branches.

## 9. Personal hold-tap defaults

All ZMK keyboard builds intended for this user use one firmware-owned typing profile:

- `tapping-term-ms = <200>`
- `quick-tap-ms = <175>`
- `require-prior-idle-ms = <-1>`
- Mod-Tap flavor: `balanced`

These are global personal defaults, not per-board tuning suggestions. The shared source is `boards/shields/common/personal_hold_tap.dtsi`; every supported keyboard keymap must include it directly or through the shared feature include. Board-local definitions must not silently override these values.

Studio runtime editing of Mod-Tap/Layer-Tap timing is disabled so these known-good firmware values remain authoritative. Custom non-typing hold-tap behaviors (for example, a deliberately long Bluetooth profile selector) retain their explicitly designed timing and are not rewritten as typing behaviors.

For QMK-based builds, preserve the equivalent personal tapping/quick-tap behavior using the corresponding QMK configuration; do not mechanically translate ZMK's `require-prior-idle-ms` property into an unrelated QMK option.

## 10. Physical side identity transport

The firmware platform now exposes a dedicated encrypted split-service GATT read characteristic for physical side identity. Its one-byte values are 0=unknown, 1=LEFT, and 2=RIGHT. The peripheral build must set `CONFIG_ZMK_SPLIT_BLE_PERIPHERAL_SIDE`; the central reports identity through a typed event and read-only API. BLE source indexes remain transport-local and are never used as side identity.

Chocofi, Hana, and Sofle peripheral build configurations explicitly set LEFT/RIGHT values. A peripheral with missing or invalid identity is excluded from side-specific battery presentation; no connection-order fallback is permitted. The consuming dongle core must reject duplicate simultaneous side claims and handle disconnect/reconnect state transitions.
