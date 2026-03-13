const stackFrames = require("./lib/binding");
const test = require("node:test");
const assert = require("node:assert");

test("returns file name", () => {
  const [file_name] = stackFrames.getAt(0);
  assert.equal(file_name, __filename);
});

test("nested", () => {
  const g = () => stackFrames.getAt(0);
  const f = () => g();

  const [file_name, line_number] = f();
  assert.equal(file_name, __filename);
  assert.equal(line_number, 11);
});

test("missing stack frame index parameter", () => {
  assert.throws(() => stackFrames.getAt(), {
    message: "Missing stack frame index parameter",
  });
});

test("stack frame index parameter is not number", () => {
  assert.throws(() => stackFrames.getAt("hello"), {
    message: "Stack frame index parameter is not a number",
  });
});

test("not found stack frame index returns null", () => {
  assert.equal(stackFrames.getAt(34578), null);
});
