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


void ReadFile(const v8::FunctionCallbackInfo<v8::Value>& args)
{	
	if (args.Length() > 0)
	{
		std::fstream fileScript;

		fileScript.open(*String::Utf8Value(args[0]->ToString()), std::ios_base::in);
		if (fileScript.is_open())
		{
			std::string contentScript((std::istreambuf_iterator<char>(fileScript)),
				(std::istreambuf_iterator<char>()));
			
			Local<String> content = String::NewFromUtf8(args.GetIsolate(), contentScript.c_str(), String::kNormalString);
			args.GetReturnValue().Set(content);
		}

		
	}
}