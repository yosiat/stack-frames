#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")"
npm test -- --test-reporter=dot 2>&1 | tail -50
