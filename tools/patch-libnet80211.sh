#!/usr/bin/env bash
set -euo pipefail

if [ -z "${IDF_PATH:-}" ]; then
    echo "IDF_PATH is not set. Source ESP-IDF first:"
    echo "  . /path/to/esp-idf/export.sh"
    exit 1
fi

IDF_ROOT="$IDF_PATH"
LIB_DIR="$IDF_ROOT/components/esp_wifi/lib/esp32c6"
LIB="$LIB_DIR/libnet80211.a"
BACKUP="$LIB_DIR/libnet80211.a.orig"
PATCHED="$LIB_DIR/libnet80211.a.patched"
SYMBOL="ieee80211_raw_frame_sanity_check"

find_tool() {
    if command -v riscv32-esp-elf-objcopy >/dev/null 2>&1; then
        command -v riscv32-esp-elf-objcopy
        return
    fi
}

find_nm() {
    if command -v riscv32-esp-elf-nm >/dev/null 2>&1; then
        command -v riscv32-esp-elf-nm
        return
    fi
}

OBJCOPY="$(find_tool)"
NM="$(find_nm)"

if [ -z "$OBJCOPY" ] || [ -z "$NM" ]; then
    echo "Missing riscv32-esp-elf objcopy/nm tools. Source ESP-IDF first."
    exit 1
fi

if [ ! -f "$LIB" ]; then
    echo "Missing $LIB"
    exit 1
fi

if "$NM" "$LIB" 2>/dev/null | awk -v sym="$SYMBOL" '$3 == sym && $2 == "W" { found = 1 } END { exit found ? 0 : 1 }'; then
    echo "$LIB is already patched."
    exit 0
fi

if [ ! -f "$BACKUP" ]; then
    cp "$LIB" "$BACKUP"
    echo "Backup saved to $BACKUP"
fi

"$OBJCOPY" --weaken-symbol="$SYMBOL" "$LIB" "$PATCHED"
mv "$PATCHED" "$LIB"
touch "$LIB"

echo "Patched $LIB"
echo "Original: $(sha256sum "$BACKUP" | awk '{print $1}')"
echo "Current:  $(sha256sum "$LIB" | awk '{print $1}')"
