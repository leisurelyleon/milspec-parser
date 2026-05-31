#!/usr/bin/env bash
# Build, then validate the valid sample (passes) and the invalid sample
# (reports every violation with precise positions).
set -euo pipefail

if [ ! -x build/bin/milspec-cli ]; then
    cmake -S . -B build
    cmake --build build
fi

echo "== Describe schema =="
./build/bin/milspec-cli describe schemas/dla-asset.schema

echo
echo "== Validate valid.dat (expect: all valid) =="
./build/bin/milspec-cli validate schemas/dla-asset.schema data/samples/valid.dat

echo
echo "== Validate invalid.dat (expect: violations reported) =="
./build/bin/milspec-cli validate schemas/dla-asset.schema data/samples/invalid.dat || true
