# Universal dongle protocol review (design gate)

Status: design review; no live BLE/HID dispatch changes made.

## Existing protocol facts verified from the experimental ZMK source

- Split peripherals advertise the existing ZMK split service UUID. The central scans for that service, reserves a peripheral slot, stops scanning, and initiates a connection.
- The central maps a connection to a slot using the current connection/slot table. The slot is a transport routing index, not by itself a durable Hana/Chocofi identity.
- The central's queued peripheral events include a numeric source slot. The work callback forwards the event to ZMK's split transport handler.
- On disconnect, the central releases any positions tracked in that slot by enqueueing key-up events, clears slot position state, then makes the slot available.
- Existing split BLE pairing/address validation and slot allocation must remain authoritative until a separate identity mapping is proven.
- The experimental universal router is currently only a policy model. Its assignments are in RAM and are not connected to the BLE central, keymap runtime, HID state, or a durable identity registry.

## Critical protocol distinction

A BLE connection is not proof of which keyboard a peripheral belongs to. A slot index is not a keyboard identity, and connection order must never decide Hana versus Chocofi. Before routing any key event, the dongle needs a stable, collision-checked mapping from a bonded peripheral identity to a keyboard and side. Unknown, conflicting, or duplicate identities must be quarantined rather than routed.

## Required connection state machine

1. Advertisement discovered: inspect identity using the existing ZMK bond/allow-list mechanisms; do not infer keyboard from advertisement order.
2. Unknown or ambiguous identity: do not deliver key-position, sensor, behavior, battery, or layout events to the active keyboard; log diagnostic and keep discovery available.
3. Recognized identity: reserve a slot only if the identity-to-side mapping is unique and the existing ZMK peripheral admission checks pass.
4. Connection ready: mark the slot connected only after the existing split GATT service/characteristic setup has completed.
5. Input event: validate the connection is still bound to the same identity and that the identity belongs to the active keyboard. Ignore all stale, unassigned, or inactive-board events.
6. Disconnect/failover: stop accepting that source first; release every held position and any board-scoped behavior/HID state; drain or invalidate queued events from that source; then reconcile availability and switch only to a fully identified connected board.
7. No eligible board connected: enter input-idle state but continue scanning/reconnection attempts.

## Switching safety invariants

- Never switch based solely on slot number or connection order.
- Never forward inactive-board key presses.
- No stuck keys: old board's pressed positions must be released before the new board can emit accepted input.
- Queue entries must be checked against current slot generation/connection identity to prevent stale events after slot reuse.
- A keyboard switch must not delete, reset, or migrate BLE bonds.
- Runtime keymap and Studio configuration switching must be atomic with routing selection, or input must remain gated during transition.
- The dongle must not accept a Studio write unless the request includes a recognized board identity and its revision matches the current stored revision.
- Missing/corrupt board-specific settings must fall back to a safe default for that board, never to the other board's settings.

## Storage prototype limitations

The current storage prototype stores only four record types with a 128-byte maximum per record. It cannot hold a complete ZMK Studio configuration. Its Zephyr settings adapter has not yet been validated with on-device reboot tests or the actual Studio RPC write path. It must not be treated as production persistence.

## Next implementation sequence

1. Add pure host tests for identity assignment, unknown/duplicate identities, slot reuse/generation, stale queued events, failover, and release-before-accept.
2. Verify the existing ZMK BLE bond/slot APIs and central callback ordering at the exact source revision.
3. Implement an experimental identity registry that uses an explicit, collision-checked board+side identity provisioned per peripheral. Do not use address order or name alone.
4. Add event-source validation at the central queue boundary, with a slot generation/identity check; keep default builds unchanged behind an explicit Kconfig option.
5. Only then connect safe switch transitions to HID release and active-board runtime selection.
6. Replace the tiny prototype storage format with bounded, versioned, checksummed, atomic records sized for actual Studio configurations, and test reboot/power-loss behavior before enabling writes.
7. Extend custom Studio RPC/UI to explicitly target a board identity and maintain independent revisions.

## Release gate

No firmware should be labelled ready for everyday use until both Hana and Chocofi are individually bonded and tested; all four halves have unambiguous identities; active/inactive input routing, disconnect during held key, failover, both-off idle/reconnect, Studio writes to each board, reboot persistence, and no-bond-reset upgrade have been tested on hardware.
