#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <vector>
#include <chrono>


#include <thread>
#include "v8.h"



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


class EventLoop
{
private:

	v8::Persistent<v8::Context,v8::CopyablePersistentTraits<v8::Context> > context;
	v8::Isolate*  isolate;
	std::priority_queue < Handler, std::vector<Handler>, Handler::Less> eventQueueTimer;
	std::queue<Handler> eventQueuePrepare;
	std::queue<Handler> eventQueueCheck;
	
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
			eventQueueCheck.push(handler);
		}
	}

	void run();


	

};
