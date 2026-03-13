#include <node.h>
#include <v8.h>

namespace stack_frames {

static const v8::StackTrace::StackTraceOptions kOptions =
    static_cast<v8::StackTrace::StackTraceOptions>(v8::StackTrace::kLineNumber |
                                                   v8::StackTrace::kScriptName);

// Returns [file_name, line_number] as a packed array, or null if frame
// doesn't exist. Using Array::New(isolate, elements, 2) creates a
// PACKED_ELEMENTS array in a single allocation — faster than Object::New +
// named property Set calls which involve string key resolution and hidden
// class transitions.
void GetAt(const v8::FunctionCallbackInfo<v8::Value> &args) {
  v8::Isolate *isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);

  if (args.Length() < 1) {
    isolate->ThrowException(
        v8::Exception::TypeError(v8::String::NewFromUtf8Literal(
            isolate, "Missing stack frame index parameter")));
    return;
  }

  int32_t frameIndex;
  if (args[0]->IsInt32()) {
    frameIndex = args[0].As<v8::Int32>()->Value() + 1;
  } else if (args[0]->IsNumber()) {
    frameIndex =
        args[0]->Int32Value(isolate->GetCurrentContext()).FromJust() + 1;
  } else {
    isolate->ThrowException(
        v8::Exception::TypeError(v8::String::NewFromUtf8Literal(
            isolate, "Stack frame index parameter is not a number")));
    return;
  }

  if (frameIndex < 1) {
    args.GetReturnValue().SetNull();
    return;
  }

  v8::Local<v8::StackTrace> trace =
      v8::StackTrace::CurrentStackTrace(isolate, frameIndex, kOptions);

  if (frameIndex > trace->GetFrameCount()) {
    args.GetReturnValue().SetNull();
    return;
  }

  v8::Local<v8::StackFrame> frame = trace->GetFrame(isolate, frameIndex - 1);

  v8::Local<v8::String> scriptName = frame->GetScriptName();
  v8::Local<v8::Value> elements[2];
  if (!scriptName.IsEmpty() && scriptName->Length() > 0) {
    elements[0] = scriptName;
  } else {
    elements[0] = v8::Null(isolate);
  }
  elements[1] = v8::Integer::New(isolate, frame->GetLineNumber());

  args.GetReturnValue().Set(v8::Array::New(isolate, elements, 2));
}

void Initialize(v8::Local<v8::Object> exports, v8::Local<v8::Value> module,
                v8::Local<v8::Context> context, void *priv) {
  NODE_SET_METHOD(exports, "getAt", GetAt);
}

} // namespace stack_frames

NODE_MODULE_CONTEXT_AWARE(NODE_GYP_MODULE_NAME, stack_frames::Initialize)
