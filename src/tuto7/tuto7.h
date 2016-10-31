#pragma once 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <queue>

//used for this_thread::yield()
#include <thread>

#include <chrono>

#include "libplatform/libplatform.h"
#include "v8.h"

class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
 public:
  virtual void* Allocate(size_t length) {
    void* data = AllocateUninitialized(length);
    return data == NULL ? data : memset(data, 0, length);
  }
  virtual void* AllocateUninitialized(size_t length) { return malloc(length); }
  virtual void Free(void* data, size_t) { free(data); }
};

void Print(const v8::FunctionCallbackInfo<v8::Value>& args);
void NextTick(const v8::FunctionCallbackInfo<v8::Value>& args);
void SetTimeOut(const v8::FunctionCallbackInfo<v8::Value>& args);

//read file synchronously
void ReadFile(const v8::FunctionCallbackInfo<v8::Value>& args);

//read file async
void ReadFileAsync(const v8::FunctionCallbackInfo<v8::Value>& args);

enum HandlerKind{
	eTimer = 1,
	ePrepare,
	eCheck
};

struct Handler
{

	HandlerKind kind;

	std::chrono::time_point<std::chrono::system_clock> start, end;
	v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function> > callback;

	struct Less
	{
		bool operator()( const Handler& lhs, const Handler& rhs){
			if (lhs.end < rhs.end) return true;
			return false;
		}
	};

	Handler* parent;

	void* data;
	size_t len;

	Handler()
	{
		data = nullptr;
		len = 0;
	}

};


extern std::queue<Handler> eventQueue;

class EventLoop
{
private:

	v8::Persistent<v8::Context,v8::CopyablePersistentTraits<v8::Context> > context;
	v8::Isolate*  isolate;
	std::priority_queue < Handler, std::vector<Handler>, Handler::Less> eventQueueTimer;
	std::queue<Handler> eventQueuePrepare;
	std::queue<Handler> eventQueuCheck;
	
public:
	EventLoop(v8::Local<v8::Context> inContext)
	{
		context.Reset(inContext->GetIsolate(), context);
		isolate = inContext->GetIsolate();
	}
	void push(const Handler& handler)
	{
		if (handler.kind == eTimer)
		{
			eventQueueTimer.push(handler);
		}
		else if (handler.kind == ePrepare)
		{
			eventQueuePrepare.push(handler);
		}
		else if (handler.kind == eCheck)
		{
			eventQueuCheck.push(handler);
		}
	}


	void run()
	{
		//pass timers
		while (!eventQueueTimer.empty())
		{
			Handler handler = eventQueueTimer.top();
			if (handler.end < std::chrono::system_clock::now())
			{
				v8::Local<v8::Function> function = handler.callback.Get(isolate);
				function->Call(context.Get(isolate), 0, {});
				handler.callback.Empty();
				eventQueue.pop();
			}
			else
			{
				break;
			}

		}

		//pass prepare
		while (!eventQueuePrepare.empty())
		{
			Handler handler = eventQueuePrepare.front();

		}



	}

};

extern EventLoop eventLoop;