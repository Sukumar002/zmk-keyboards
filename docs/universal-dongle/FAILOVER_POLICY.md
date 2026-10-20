# Universal dongle: unattended failover and recovery

Status: **testable router policy only**. Not connected to the ZMK BLE central, HID release machinery, or keymap switching. Do not flash as universal firmware.

## Scenarios
1. Chocofi selected, Hana present: remain on Chocofi; do not steal focus.
2. Chocofi disconnects completely, Hana has a verified peripheral: schedule transition to Hana. The adapter MUST release held HID keys and drain combos/hold-taps before accepting the replacement keyboard.
3. One Chocofi half disconnects, other half remains: stay on Chocofi; report partial keyboard status. Do not switch back and forth due to a single missing half.
4. Both keyboards disconnected: idle **input** mode, no key events accepted, keep BLE scanning/advertisement discovery or a scheduled low-duty scan active. Not a full MCU/radio sleep that cannot hear advertisements.
5. Either keyboard advertises and reconnects while idle: select the available keyboard. If the dongle's current active board reconnects, just resume it; if the other appears first, use the safe transition sequence.
6. Chocofi returns while Hana is working: stay on Hana until manual switch or Hana disappears. Avoid surprising keyboard switches.
7. Repeated BLE drops/brownouts: require debounce/grace intervals in the BLE adapter, avoid immediate oscillation; document actual measured disconnect/reconnect latency.

## Engineering requirements before hardware
- Maintain stable four-way board/side identity from secure bonding metadata. The policy test uses numeric slots as placeholders only.
- Maintain a low-duty discovery window for both keyboard pairs while idle, including after USB host suspend as power policy permits; BLE advertisements do **not** themselves wake a dongle whose radio/scanner is shut off.
- Distinguish deliberate power-off from temporary radio loss only where possible; otherwise delay failover by an adjustable grace period.
- For a split keyboard, one live half can enable partial usage; do not claim full keyboard readiness until both halves are attached.
- Set an explicit selection policy to handle both keyboards reconnecting at once (e.g. last active/preferred), without changing focus once active.
- Make keyboard selection independent of a combo on an unavailable keyboard. Fallback can be automatic from connection events.
- Expose status on OLED: CHOCOFI / HANA / WAITING, left/right availability, failure reason, whether fallback was automatic.
- Never erase current pairing/settings just to satisfy an automated test.

Policy code: `Sukumar002/zmk/app/src/universal_dongle/router.c`.
Host test: `Sukumar002/zmk/tests/universal_dongle/router_test.c`.
