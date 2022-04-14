#include <node.h>
#include <v8.h>
#include "os.h"

namespace demo
{

	using v8::ArrayBuffer;
	using v8::Context;
	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Local;
	using v8::Number;
	using v8::Object;
	using v8::String;
	using v8::Uint8ClampedArray;
	using v8::Value;

	// 传入系统类型参数
	void Method(const FunctionCallbackInfo<Value> &args)
	{
		Isolate *isolate = args.GetIsolate();

		double p = args[0].As<Number>()->Value();

		Local<Context> context = isolate->GetCurrentContext();
		Local<Object> obj = Object::New(isolate);

		Local<Object> screen = Object::New(isolate);
		// Local<ArrayBuffer> img_b = ArrayBuffer::New(0, 0);
		// Local<Uint8ClampedArray> img = Uint8ClampedArray::New(img_b, 0, 0);
		Local<Number> img = Number::New(isolate, 1);
		Local<Number> w = Number::New(isolate, 1);
		Local<Number> h = Number::New(isolate, 1);
		screen->Set(context, String::NewFromUtf8(isolate, "img").ToLocalChecked(), img).FromJust();
		screen->Set(context, String::NewFromUtf8(isolate, "width").ToLocalChecked(), w).FromJust();
		screen->Set(context, String::NewFromUtf8(isolate, "height").ToLocalChecked(), h).FromJust();

		obj->Set(context, 0, screen).FromJust();

#if defined IS_WINDOWS
#elif defined IS_MACOS
#elif defined IS_LINUX
#endif

		args.GetReturnValue().Set(obj);
	}

	void Initialize(Local<Object> exports)
	{
		NODE_SET_METHOD(exports, "capture", Method);
	}

	NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)

} // namespace demo
