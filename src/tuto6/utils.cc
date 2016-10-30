#include "tuto6.h"
using namespace v8;
void Print(const v8::FunctionCallbackInfo<v8::Value>& args) {
	bool first = true;
	for (int i = 0; i < args.Length(); i++) {
		v8::HandleScope handle_scope(args.GetIsolate());
		if (first) {
			first = false;
		}
		else {
			std::cout << " ";
		}
		v8::String::Utf8Value str(args[i]);
		std::cout << *str;
		
		
	}
	std::cout << std::endl;
	fflush(stdout);
}


//nextTick function
void NextTick(const v8::FunctionCallbackInfo<v8::Value>& args) {
	
	if (args.Length() > 0)
	{
		Handler handler;
		handler.end =handler.start = std::chrono::system_clock::now();
		handler.callback.Reset(args.GetIsolate(), args[0].As<Function>());

		eventQueue.push(handler);
	}
}

//setTimeOut
void SetTimeOut(const v8::FunctionCallbackInfo<v8::Value>& args) {

	if (args.Length() > 1)
	{
		Handler handler;
		handler.end = handler.start = std::chrono::system_clock::now();
		handler.callback.Reset(args.GetIsolate(), args[0].As<Function>());
		
		handler.end += std::chrono::milliseconds(args[1]->ToInt32()->Value());
		eventQueue.push(handler);
	}
}


