#pragma once 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>

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
	static int sHandlerID;

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
		id = ++sHandlerID;
	}
	~Handler()
	{
		callback.Empty();
		::free(data);
	}
	int id;
};




class EventLoop
{
private:

	v8::Persistent<v8::Context,v8::CopyablePersistentTraits<v8::Context> > context;
	v8::Isolate*  isolate;
	std::priority_queue < Handler, std::vector<Handler>, Handler::Less> eventQueueTimer;
	std::vector<Handler> eventQueuePrepare;
	std::queue<Handler> eventQueuCheck;
	
public:
	EventLoop(){}
	~EventLoop(){
		context.Empty();
	}
	EventLoop(v8::Local<v8::Context> inContext)
	{
		context.Reset(inContext->GetIsolate(), inContext);
		isolate = inContext->GetIsolate();
	}
	void SetContext(v8::Local<v8::Context> inContext)
	{
		context.Reset(inContext->GetIsolate(), inContext);
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
			eventQueuePrepare.push_back(handler);
		}
		else if (handler.kind == eCheck)
		{
			eventQueuCheck.push(handler);
		}
	}


	void run()
	{
		do{
			//pass timers
			while (!eventQueueTimer.empty())
			{
				Handler handler = eventQueueTimer.top();
				if (handler.end < std::chrono::system_clock::now())
				{
					v8::Local<v8::Function> function = handler.callback.Get(isolate);
					function->Call(context.Get(isolate)->Global(), 0, {});
					handler.callback.Empty();
					eventQueueTimer.pop();
				}
				else
				{
					break;
				}

			}

			//pass directly to check
			while (!eventQueuCheck.empty())
			{
				Handler handler = eventQueuCheck.front();
				if (handler.parent != nullptr)
				{
					//remove prepare handler associated
					auto it = std::find_if(eventQueuePrepare.begin(), eventQueuePrepare.end(), [&](const Handler& item){
						return item.id == handler.parent->id;
					});
					
					if (it != eventQueuePrepare.end())
					{
						it->callback.Get(isolate)->Call(context.Get(isolate)->Global(), 0, {});
						eventQueuePrepare.erase(it);
					}

					eventQueuCheck.pop();
				}
			}



		} while (eventQueuCheck.empty() || eventQueueTimer.empty() || eventQueuePrepare.empty());
		
	}

};

extern EventLoop eventLoop;