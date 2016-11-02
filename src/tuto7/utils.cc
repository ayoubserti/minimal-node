#include "tuto7.h"
#include <iostream>
#include <fstream>
#include "stream.h"

#include "eventloop.h"


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

		eventLoop->push(handler);
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
		eventLoop->push(handler);
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

static void streamReader(Stream* inFS)
{
	
	static int chunck_size = 512; //512 byte lenght
	while (!inFS->isEOF())
	{
		char* buf = (char*)::malloc(chunck_size);
		unsigned int len = inFS->Get(buf, chunck_size);
		
	}	
}


void ReadFileAsync(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	Handler* prepare = new Handler;
	prepare->kind = ePrepare;
	prepare->callback.Reset(args.GetIsolate(), args[1].As<Function>());
	Stream* stream = IFileStream::CreateIFileStream(eventLoop, prepare, *String::Utf8Value(args[0]->ToString()));
	eventLoop->push(*prepare);
	std::thread* th = new std::thread(&streamReader,stream);
}