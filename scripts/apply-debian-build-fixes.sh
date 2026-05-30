#!/bin/bash
# Apply Debian/old-lib compatibility fixes when git pull is not available.
# Run from the tr4mpass repo root:  bash scripts/apply-debian-build-fixes.sh

set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
PATCH="$ROOT/debian-build-fix.patch"

if [ ! -f "$PATCH" ]; then
    echo "Missing $PATCH — copy the full repo from the build machine." >&2
    exit 1
fi

cd "$ROOT"
if git apply --check "$PATCH" 2>/dev/null; then
    git apply "$PATCH"
    echo "[+] Patch applied."
elif patch -p1 --dry-run --force < "$PATCH" >/dev/null 2>&1; then
    patch -p1 --force < "$PATCH"
    echo "[+] Patch applied (patch(1))."
else
    echo "[-] Patch does not apply cleanly. Sync the repo instead:" >&2
    echo "    rsync -av --exclude .git USER@HOST:~/iphone/tr4mpass/ ~/tr4mpass/" >&2
    exit 1
fi

echo "[*] Required packages (Debian/Ubuntu):"
echo "    sudo apt install libcurl4-openssl-dev libimobiledevice-dev \\"
echo "         libirecovery-1.0-dev libusb-1.0-0-dev libplist-dev \\"
echo "         libssl-dev libssh2-1-dev pkg-config build-essential"
echo "[*] Then: make clean && make"
