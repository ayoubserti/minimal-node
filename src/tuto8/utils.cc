#include "tuto8.h"
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
		handler.kind = eTimer;
		eventLoop->push(handler);
	}
}

//setTimeOut
void SetTimeOut(const v8::FunctionCallbackInfo<v8::Value>& args) {

	if (args.Length() > 1)
	{
		Handler handler;
		handler.kind = eTimer;
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

static void streamReader(std::fstream* inFS,Handler* handler)
{
	static int chunck_size = 512; //512 byte lenght
	char buf[512];
	while (!inFS->eof())
	{
		std::streamsize len = inFS->read(buf, 512).readsome(buf, 512);
		
		if (len > 0)
		{
			//install check handler
			Handler check;
			check.kind = eCheck;
			check.parent = handler;
			check.data = ::malloc(len);
			memcpy(check.data, buf, len);
			check.len = len;
			eventLoop->push(check);
		}
		
	}
}


void ReadFileAsync(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	std::fstream* fs = new std::fstream();
	fs->open(*String::Utf8Value(args[0]->ToString()), std::ios_base::binary | std::ios_base::in);
	if (fs->is_open())
	{
		//install prepare handler into eventloop
		Handler* prepare = new Handler;
		prepare->kind = ePrepare;
		prepare->callback.Reset(args.GetIsolate(), args[1].As<Function>());

		std::thread* th = new std::thread(&streamReader, fs,prepare);
		if (th != nullptr)
		{
			eventLoop->push(*prepare);
		}
	}
}

//int Handler::sHandlerID = 0;