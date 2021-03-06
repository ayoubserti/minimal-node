//tuto6 
// implement require function
#include "tuto6.h"

using namespace v8;

std::queue<Handler> eventQueue;

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

	global->Set(String::NewFromUtf8(isolate, "setTimeout", NewStringType::kNormal).ToLocalChecked(), FunctionTemplate::New(isolate, SetTimeOut)->GetFunction());
	global->Set(String::NewFromUtf8(isolate, "readFile", NewStringType::kNormal).ToLocalChecked(), FunctionTemplate::New(isolate, ReadFile)->GetFunction());
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
		Handler handler = eventQueue.front();
		if (handler.end < std::chrono::system_clock::now())
		{
			Local<Function> function = handler.callback.Get(isolate);
			function->Call(global, 0, {});
			handler.callback.Empty();
			eventQueue.pop();
		}
		
		std::this_thread::yield(); 
		v8::platform::PumpMessageLoop(platform,isolate);
	}


  }

  // Dispose the isolate and tear down V8.
  isolate->Dispose();
  V8::Dispose();
  V8::ShutdownPlatform();
  delete platform;
  return 0;
}
