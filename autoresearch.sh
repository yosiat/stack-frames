#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")"
# Quick pre-check: addon loads
node -e "require('./lib/binding.js').getAt(0)" 2>/dev/null || true
# Run benchmark; parse "stackFrames#getAt x NNN ops/sec"
out=$(node benchmark.js 2>&1)
ops=$(echo "$out" | grep "stackFrames#getAt" | head -1 | sed -n 's/.*x \([0-9,]*\).*/\1/p' | tr -d ',')
if [[ -z "${ops:-}" ]]; then
  echo "METRIC ops_per_sec=0"
  exit 1
fi
echo "METRIC ops_per_sec=$ops"
