# Universal dongle test matrix

Use this after the Chocofi hardware arrives. Record PASS/FAIL and firmware commit for each test.

| Test | Expected result | Status |
|---|---|---|
| Chocofi left keys / matrix | All physical switches map to intended logical positions | NOT TESTED |
| Chocofi right keys / matrix | All physical switches map to intended logical positions | NOT TESTED |
| Chocofi direct BLE | Both halves connect and type after reboot | NOT TESTED |
| Chocofi dedicated dongle | Both halves connect and type after reboot | NOT TESTED |
| Hana dedicated dongle baseline | Original firmware still behaves as before | NOT TESTED |
| Four link pairing | Four halves bond without unintended collisions | NOT TESTED |
| Four link reconnection | All halves reconnect after power cycle | NOT TESTED |
| Identity verification | Events can be attributed to correct board and half | NOT TESTED |
| Hana active | Only Hana ordinary keys generate HID | NOT TESTED |
| Chocofi active | Only Chocofi ordinary keys generate HID | NOT TESTED |
| Switch from inactive board | Switching command works without unintended keystrokes | NOT TESTED |
| Held modifier on switch | No stuck key/modifier after active-board change | NOT TESTED |
| Studio Hana profile | Keymap/layers/combos shown and saved correctly | NOT TESTED |
| Studio Chocofi profile | Separate keymap/layers/combos shown and saved correctly | NOT TESTED |
| Studio reboot persistence | Both board settings survive reboot | NOT TESTED |
| OLED | Active board and corresponding battery/connection data correct | NOT TESTED |
| Extended standby | No connection failures or unreasonable battery drain | NOT TESTED |
