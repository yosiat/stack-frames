# stack-frames

Allows getting stack-frames at specific position with low overhead.

### API

#### stackFrames.getAt(number)

given a position returns stack-frames object (which contains file_name and line_number) at the requested position


### Benchmarks

`MacBook Pro, Apple M4 Max` with `Node v22.14.0`

Run using `node benchmark.js`

```
stackFrames#getAt x 723,520 ops/sec ±4.24% (52 runs sampled)
callsites()[] x 324,443 ops/sec ±0.44% (92 runs sampled)
```
