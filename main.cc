#include <node.h>
#include <nan.h>
#include <v8.h>

namespace demo
{

	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Local;
	using v8::Number;
	using v8::Object;
	using v8::String;
	using v8::Value;

	// 传入系统类型参数
	void Method(const FunctionCallbackInfo<Value> &args)
	{
		Isolate *isolate = args.GetIsolate();

		double p = args[0].As<Number>()->Value();

		if (p == 0) //"win"
		{
		}
		else if (p == 1) //"x11"
		{
		}
		else if (p == 2) //"wayland"
		{
		}
		else if (p == 3) //"mac"
		{
		}

		args.GetReturnValue().Set(args[0]);
	}

	void Initialize(Local<Object> exports)
	{
		NODE_SET_METHOD(exports, "capture", Method);
	}

	NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)

} // namespace demo
