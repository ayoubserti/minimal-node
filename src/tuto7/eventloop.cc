#include "eventloop.h"


void EventLoop::run()
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
		while (!eventQueueCheck.empty())
		{
			Handler handler = eventQueueCheck.front();
			if (handler.parent != nullptr)
			{
				//remove prepare handler associated
				auto it = std::find_if(eventQueuePrepare.begin(), eventQueuePrepare.end(), [&](const Handler& item){
					return item.id == handler.parent->id;
				});

				if (it != eventQueuePrepare.end())
				{
					it->callback.Get(isolate)->Call(context.Get(isolate)->Global(), 0, {});
					--it->ref;
					if (it->ref == 0)
					{
						eventQueuePrepare.erase(it);
					}
					
					
				}

				eventQueueCheck.pop();
			}
		}



	} while (eventQueueCheck.empty() || eventQueueTimer.empty() || eventQueuePrepare.empty());




}

int Handler::sHandlerID =0 ;