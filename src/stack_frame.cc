#include <napi.h>
#include <v8.h>

static const v8::StackTrace::StackTraceOptions stackTraceOptions =
    static_cast<v8::StackTrace::StackTraceOptions>(v8::StackTrace::kLineNumber |
                                                   v8::StackTrace::kScriptName);

Napi::Value stackFrames_GetAt(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1) {
    Napi::TypeError::New(env, "Missing stack frame index parameter")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Stack frame index parameter is not a number")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  int stackTraceIndex = info[0].As<Napi::Number>().Int64Value() + 1;

  v8::Isolate *isolate = v8::Isolate::GetCurrent();

  v8::Local<v8::StackTrace> stackTrace = v8::StackTrace::CurrentStackTrace(
      isolate, stackTraceIndex, stackTraceOptions);

  if (stackTraceIndex > stackTrace->GetFrameCount()) {
    return env.Null();
  }

  v8::Local<v8::stackFrames> stackFrames =
      stackTrace->GetFrame(isolate, stackTraceIndex - 1);

  Napi::Object frame = Napi::Object::New(env);

  v8::String::Utf8Value scriptName(isolate, stackFrames->GetScriptName());
  frame.Set("line_number", stackFrames->GetLineNumber());
  if (scriptName.length() > 0) {
    frame.Set("file_name", *scriptName);
  }

  return frame;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "getAt"),
              Napi::Function::New(env, stackFrames_GetAt));
  return exports;
}

NODE_API_MODULE(addon, Init)
