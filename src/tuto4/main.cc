// tuto4
// Show how to add eventloop and introduce how asynchronous function works

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <queue>

//used for thread::yield()
#include <thread>

#include "libplatform/libplatform.h"
#include "v8.h"

using namespace v8;

class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
 public:
  virtual void* Allocate(size_t length) {
    void* data = AllocateUninitialized(length);
    return data == NULL ? data : memset(data, 0, length);
  }
  virtual void* AllocateUninitialized(size_t length) { return malloc(length); }
  virtual void Free(void* data, size_t) { free(data); }
};


std::queue<Persistent<Function,v8::CopyablePersistentTraits<Function> > > eventQueue;

// The callback that is invoked by v8 whenever the JavaScript 'print'
// function is called.  Prints its arguments on stdout separated by
// spaces and ending with a newline.
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
		Persistent<Function, CopyablePersistentTraits<Function> > function;
		function.Reset(args.GetIsolate(), args[0].As<Function>());
		eventQueue.push(function);
	}
}




int main(int argc, char* argv[]) {
  // Initialize V8.
  V8::InitializeICU();
  V8::InitializeExternalStartupData(argv[0]);
  Platform* platform = platform::CreateDefaultPlatform();
  V8::InitializePlatform(platform);
  V8::Initialize();

  // Create a new Isolate and make it the current one.
  ArrayBufferAllocator allocator;
  Isolate::CreateParams create_params;
  create_params.array_buffer_allocator = &allocator;
  Isolate* isolate = Isolate::New(create_params);
  {
    Isolate::Scope isolate_scope(isolate);

    // Create a stack-allocated handle scope.
    HandleScope handle_scope(isolate);

    // Create a new context.
    Local<Context> context = Context::New(isolate);

    // Enter the context for compiling and running the hello world script.
    Context::Scope context_scope(context);

	//parse argument to find file name
	if (argc <2)
	{
		return -1;
	}
	std::fstream fileScript;
	fileScript.open(argv[1], std::ios_base::in);
	if (!fileScript.is_open()) return -2;

	std::string contentScript((std::istreambuf_iterator<char>(fileScript)),
		(std::istreambuf_iterator<char>()));

	//add nextTick to global object
	Local<Object> global = context->Global();
	global->Set(String::NewFromUtf8(isolate, "nextTick", NewStringType::kNormal).ToLocalChecked(), FunctionTemplate::New(isolate, NextTick)->GetFunction());
	
	//add console object to global object
	Local<Object> console = Object::New(isolate);
	//add console.log method
	console->Set(String::NewFromUtf8(isolate, "log", NewStringType::kNormal).ToLocalChecked(), FunctionTemplate::New(isolate, Print)->GetFunction());
	global->Set(String::NewFromUtf8(isolate, "console", NewStringType::kNormal).ToLocalChecked(), console);

    // Create a string containing the JavaScript source code.
    Local<String> source =
		String::NewFromUtf8(isolate, contentScript.c_str(),
                            NewStringType::kNormal).ToLocalChecked();

    // Compile the source code.
    Local<Script> script = Script::Compile(context, source).ToLocalChecked();

    // Run the script to get the result.
    Local<Value> result = script->Run(context).ToLocalChecked();


	//after the whole script are executed, 
	//it's now time for event loop to find delayed work
	while (!eventQueue.empty())
	{
		Local<Function> function = eventQueue.front().Get(isolate);
		function->Call(global, 0, {});
		eventQueue.front().Empty();
		eventQueue.pop();

		std::this_thread::yield(); 
	}


  }

  // Dispose the isolate and tear down V8.
  isolate->Dispose();
  V8::Dispose();
  V8::ShutdownPlatform();
  delete platform;
  return 0;
}
