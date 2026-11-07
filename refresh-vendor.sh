#!/bin/sh
# refresh-vendor.sh — sync vendored copy of the contracts header into this
# repo. Repo-isolated ghost-run (nool) builds must be self-contained.
set -e
ROOT="${1:-..}"
rm -rf vendor
mkdir -p vendor/contracts/include/logi
cp "$ROOT/contracts/include/logi/proto.h" vendor/contracts/include/logi/
echo "vendor/ refreshed from $ROOT"
