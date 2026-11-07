#!/bin/sh
# refresh-vendor.sh — sync vendored copies of the shared platform libraries
# into this firmware repo. Ghost-run validation is repo-isolated by design,
# so the build must be self-contained: `make vendor` before committing.
set -e
ROOT="${1:-../..}"
rm -rf vendor
mkdir -p vendor/contracts/include/logi vendor/mcu-protocol/src vendor/mcu-protocol/include/logi vendor/hal-core/src vendor/hal-core/include/logi
cp "$ROOT/platform/contracts/include/logi/proto.h"          vendor/contracts/include/logi/
cp "$ROOT/platform/mcu-protocol/include/logi/packet.h"      vendor/mcu-protocol/include/logi/
cp "$ROOT/platform/mcu-protocol/src/packet.c"               vendor/mcu-protocol/src/
for f in gpio_mock led button sensor battery_mock radio_mock; do
  cp "$ROOT/platform/hal-core/src/$f.c"                     vendor/hal-core/src/
done
cp "$ROOT/platform/hal-core/include/logi/hal.h"             vendor/hal-core/include/logi/
echo "vendor/ refreshed from $ROOT"
