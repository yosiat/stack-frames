const stackFrames = require("./lib/binding");
const test = require("ava");

console.log(__filename);

test("returns file name", (t) => {
  t.is(stackFrames.getAt(0).file_name, __filename);
});

test("nested", (t) => {
  const g = () => stackFrames.getAt(0);
  const f = () => g();

  t.deepEqual(f(), {
    line_number: 11, // where `g` is declared
    file_name: __filename,
  });
});

test("missing stack frame index parameter", (t) => {
  const error = t.throws(() => stackFrames.getAt());
  t.is(error.message, "Missing stack frame index parameter");
});

test("stack frame index parameter is not number", (t) => {
  const error = t.throws(() => stackFrames.getAt("hello"));
  t.is(error.message, "Stack frame index parameter is not a number");
});

test("not found stack frame index returns null", (t) => {
  t.is(stackFrames.getAt(34578), null);
});
