#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <vector>
#include <chrono>


#include <thread>
#include <mutex>
#include "v8.h"



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
		bool operator()(const Handler& lhs, const Handler& rhs){
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
		ref = 1;
	}
	~Handler()
	{
		callback.Empty();
		/*if (data != nullptr)
			::free(data);*/
	}
	int id;

	int ref;
};



class EventLoop
{
private:

	v8::Persistent<v8::Context,v8::CopyablePersistentTraits<v8::Context> > context;
	v8::Isolate*  isolate;
	std::priority_queue < Handler, std::vector<Handler>, Handler::Less> eventQueueTimer;
	std::vector<Handler> eventQueuePrepare;
	std::queue<Handler> eventQueueCheck;

	
	
public:
	EventLoop(v8::Local<v8::Context> inContext)
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
			eventQueueCheck.push(handler);
		}
	}

	void run();


	

};
