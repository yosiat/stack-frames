# Autoresearch: Optimize stackFrames.getAt throughput

## Objective

Maximize **ops/sec** of `stackFrames.getAt(index)` in the Benchmark.js suite. The workload is repeated calls to the native addon that returns stack frame info (file_name, line_number) for a given stack index. This is used in high-throughput logging; every nanosecond counts.

## Metrics

- **Primary**: ops_per_sec (ops/sec, higher is better) — from the "stackFrames#getAt" benchmark.
- **Secondary**: tests pass, lint/format unchanged.

## How to Run

`./autoresearch.sh` — runs `node benchmark.js`, parses the "stackFrames#getAt" line, outputs `METRIC ops_per_sec=<number>`.

## Files in Scope

| File | Purpose |
|------|--------|
| `src/stack_frames.cc` | Native N-API + V8 stack trace; all hot-path logic. |
| `binding.gyp` | Build config (sources, defines, includes). |
| `lib/binding.js` | JS entry; keep thin, no new API. |
| `benchmark.js` | Benchmark suite; can tune runs if needed. |

## Off Limits

- Changing the **semantic contract**: we must still accept an **index** and return **file name** and **line number** (or null if the frame doesn't exist). API shape (property names, object structure, etc.) may change.
- Adding new dependencies for the benchmark or the addon itself.
- Changing test.js assertions or test behavior (only fixes if a change breaks them).

## Constraints

- `npm test` must pass (autoresearch.checks.sh).
- No new production dependencies.
- Keep clang-format / Prettier style; run `npm run format` if touching formatted files.

## What's Been Tried

- (Baseline established at session start. Update this section as experiments run.)
