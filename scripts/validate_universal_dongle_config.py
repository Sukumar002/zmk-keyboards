#!/usr/bin/env python3
"""Static release-gate checks for the Universal Dongle five-image experiment."""
from pathlib import Path
import re
import sys

root = Path(".")
zmk = Path("zmk-source")
checks = []

def check(name, condition, detail=""):
    checks.append((name, bool(condition), detail))

def read(path):
    p = Path(path)
    if not p.exists():
        raise SystemExit(f"Required file missing: {p}")
    return p.read_text(encoding="utf-8")

matrix = read(root / "build-universal-dongle-routing-smoke.yaml")
keymap = read(root / "boards/shields/universal_dongle/universal_dongle.keymap")
west = read(root / "config/west.yml")
central = read(zmk / "app/src/split/bluetooth/central.c")
service = read(zmk / "app/src/split/bluetooth/service.c")
router = read(zmk / "app/src/universal_dongle/router.c")
router_header = read(zmk / "app/include/zmk/universal_router.h")
router_tests = read(zmk / "tests/universal_dongle/router_test.c")

artifacts = re.findall(r"^\s*artifact-name:\s*(\S+)\s*$", matrix, re.M)
check("exactly five firmware images", len(artifacts) == 5, f"found {len(artifacts)}")
check("artifact labels remain non-flashable", len(artifacts) == 5 and all("NOT_FOR_FLASHING" in a for a in artifacts), ", ".join(artifacts))
check("artifact names are unique", len(set(artifacts)) == len(artifacts), ", ".join(artifacts))

for token, label in [
    ("CONFIG_ZMK_UNIVERSAL_DONGLE_CENTRAL_IDENTITY=y", "central identity protocol"),
    ("CONFIG_ZMK_BEHAVIOR_UNIVERSAL_DONGLE_SWITCH=y", "profile-switch behavior"),
    ("CONFIG_ZMK_SPLIT_BLE_CENTRAL_PERIPHERALS=4", "four BLE peripherals"),
    ("CONFIG_BT_MAX_CONN=7", "BLE connection capacity"),
    ("CONFIG_BT_MAX_PAIRED=7", "bond capacity"),
]:
    check(f"dongle enables {label}", token in matrix, token)

for token, label in [
    ("CONFIG_ZMK_UNIVERSAL_DONGLE_BOARD_ID=1 -DCONFIG_ZMK_UNIVERSAL_DONGLE_SIDE_ID=1", "Hana left identity"),
    ("CONFIG_ZMK_UNIVERSAL_DONGLE_BOARD_ID=1 -DCONFIG_ZMK_UNIVERSAL_DONGLE_SIDE_ID=2", "Hana right identity"),
    ("CONFIG_ZMK_UNIVERSAL_DONGLE_BOARD_ID=2 -DCONFIG_ZMK_UNIVERSAL_DONGLE_SIDE_ID=1", "Chocofi left identity"),
    ("CONFIG_ZMK_UNIVERSAL_DONGLE_BOARD_ID=2 -DCONFIG_ZMK_UNIVERSAL_DONGLE_SIDE_ID=2", "Chocofi right identity"),
]:
    check(f"correct {label}", token in matrix, token)

check("keymap has Hana profile layer", 'display-name = "HANA_BASE"' in keymap)
check("keymap has Chocofi profile layer", 'display-name = "CHOCOFI_BASE"' in keymap)
check("Hana-to-Chocofi switch combo exists", "key-positions = <0 11>;" in keymap and "bindings = <&universal_switch 2>;" in keymap)
check("Chocofi-to-Hana switch combo exists", "key-positions = <40 44>;" in keymap and "bindings = <&universal_switch 1>;" in keymap)
check("west manifest tracks the audited integration branch", "revision: project/universal-dongle-routing" in west)

match = re.search(r"static const uint8_t chocofi_to_universal\[UD_CHOCOFI_POSITION_COUNT\]\s*=\s*\{(.*?)\};", router, re.S)
values = [int(v) for v in re.findall(r"\d+", match.group(1))] if match else []
check("Chocofi map has 36 positions", len(values) == 36, f"found {len(values)}")
check("Chocofi map is unique and within 46-position keymap", len(values) == 36 and len(set(values)) == 36 and all(0 <= v < 46 for v in values))
check("router supports four stable identities", "UD_SLOTS 4" in router_header and "ud_identity_to_slot" in router_header)
check("new identities can register during a switch without opening input", "Registering a stable identity is safe during a switch" in router and "assert(ud_accept_position(&reconnect_router, 1) == UD_IGNORED)" in router_tests)

check("peripheral identity characteristic is encrypted-read", "BT_GATT_CHRC_READ, BT_GATT_PERM_READ_ENCRYPT" in service and "split_svc_universal_identity" in service)
check("central requests BLE encryption", "bt_conn_set_security(conn, BT_SECURITY_L2)" in central)
check("identity reads wait for encrypted link", "bt_conn_get_security(conn) < BT_SECURITY_L2" in central and "split_central_start_identity_read" in central)
check("identity read is resumed on security change", "Failed to start universal peripheral identity read after encryption" in central)
check("unvalidated peripherals cannot send input", "Ignoring split input from peripheral without a validated universal identity" in central)
check("duplicate logical identities are rejected", "Rejecting duplicate universal identity" in central)
check("disconnect synthesizes releases and drains them through the worker", "slot->position_state[i] & BIT(j)" in central and "universal_complete_switch_if_drained" in central)
check("switch waits for complete target pair", "ud_connected_count(&universal_router, target) != 2" in central and "ud_finish_switch(&universal_router)" in central)

print("Universal Dongle configuration/source audit")
print(f"Integration source branch: {west.split('revision: project/universal-dongle-routing', 1)[0].count('projects:') and 'project/universal-dongle-routing'}")
print(f"Chocofi position map: {len(values)} entries, {len(set(values))} unique")
failed = 0
for name, ok, detail in checks:
    print(f"[{'PASS' if ok else 'FAIL'}] {name}" + (f" — {detail}" if detail else ""))
    failed += not ok
print(f"\nResult: {len(checks) - failed}/{len(checks)} checks passed")
if failed:
    sys.exit(1)
