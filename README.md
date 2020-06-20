# stack-frames

Allows getting stack-frames at specific position with low overhead.

### api

#### stackFrames.getAt(number)

given a position returns stack-frames object (which contains file_name and line_nubmer) at the requested position


### benchmarks

```
stackFrames#getAt x 373,198 ops/sec ±0.69% (63 runs sampled)
callsites()[] x 130,833 ops/sec ±0.57% (86 runs sampled)
```

_see benchmark.js_

