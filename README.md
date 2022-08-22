# stack-frames

Allows getting stack-frames at specific position with low overhead.

### API

#### stackFrames.getAt(number)

given a position returns stack-frames object (which contains file_name and line_nubmer) at the requested position


### Benchmarks

`MacBook Pro (16-inch, 2021), Apple M1 Max` with `Node v16.16.0`

Run using `node benchmark.js`

```
stackFrames#getAt x 734,641 ops/sec ±0.30% (74 runs sampled)
callsites()[] x 164,158 ops/sec ±0.24% (92 runs sampled)
```
