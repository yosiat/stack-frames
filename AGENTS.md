# Agent instructions for stack-frames

`stack-frames` is a project that exposes stack frame info, it's being used in high throughput logging framework.

## Project summary

- **What it is:** A Node.js **native addon** (C++) that returns stack frame info (file name, line number) at a given stack position with low overhead.
- **Package:** `@yosiat/stack-frames`
- **Main API:** `stackFrames.getAt(index)` → `{ file_name, line_number }` or `null` if the frame doesn’t exist.

## Tech stack

- **Runtime:** Node.js (CI tests on 20.x, 22.x, 24.x).
- **Native layer:** C++ with **node-gyp**, **N-API** (node-addon-api), and **V8** for stack traces.
- **JS layer:** Small wrapper in `lib/binding.js` (re-exports the addon via `bindings`).
- **Formatting:** `clang-format` for `src/**`, Prettier for `**/*.js` and root `*.js`.

## Key files and layout

| Path | Purpose |
|------|--------|
| `src/stack_frames.cc` | Native implementation (N-API + V8 stack trace API). |
| `binding.gyp` | node-gyp build config (sources, node-addon-api). |
| `lib/binding.js` | Loads the native addon and exports it. |
| `test.js` | Node.js `node:test` tests for the public API. |
| `benchmark.js` | Benchmark.js suite (e.g. vs `callsites`). |
| `README.md` | API and benchmark summary. |

## Commands

- **Install and build:** `npm install` (builds the addon).
- **Test:** `npm test` (runs `node test`).
- **Lint:** `npm run lint` (clang-format check on `src/**` + Prettier check on JS).
- **Format:** `npm run format` (clang-format --fix + Prettier --write).
- **Benchmarks:** `node benchmark.js` (no npm script).

## Conventions and rules

1. **Native code:** Keep C++ in `src/`. Use N-API for the JS boundary and V8 only where needed (e.g. stack trace). Follow existing style; format with `npm run format`.
2. **JavaScript:** Use the existing style; format with Prettier. Entry point for the addon is `lib/binding.js`; don’t add API surface there without updating the native side and tests.
3. **Testing:** Any change to behavior or API should be covered in `test.js`. Run `npm test` before committing.
4. **Performance:** This project is performance-sensitive. Avoid unnecessary allocations or API changes that would regress the benchmarks; when changing the native API, run `node benchmark.js` and consider updating README/OPTIMIZATION_SUMMARY if relevant.
5. **API stability:** The public API is `getAt(index)`. Breaking changes (e.g. return shape) should be discussed and documented (e.g. in OPTIMIZATION_SUMMARY or a CHANGELOG).

## Notes for agents

- **Build:** After editing `src/*.cc` or `binding.gyp`, run `npm install` or `npx node-gyp rebuild` so tests and benchmarks use the new binary.
- **CI:** GitHub Actions runs on Node 20, 22, 24 with `npm install`, `npm run lint`, and `npm test`. No benchmark step in CI.
