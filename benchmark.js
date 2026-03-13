const Benchmark = require("benchmark");
const callsites = require("callsites").default;
const stackFrames = require("./lib/binding.js");

function getStackTraceInfo(stackIndex) {
  const callSite = callsites()[stackIndex + 1];

  return {
    file_name: callSite.getFileName(),
    line_number: callSite.getLineNumber(),
  };
}

function main() {
  var suite = new Benchmark.Suite();

  suite
    .add("stackFrames#getAt", () => stackFrames.getAt(1))
    .add("callsites()[]", () => callsites()[1])
    .on("cycle", function (event) {
      console.log(String(event.target));
    })
    .on("complete", function () {
      process.exit();
    })
    .run({ async: true });
}

main();
