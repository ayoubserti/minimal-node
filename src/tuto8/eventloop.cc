#include "eventloop.h"


void EventLoop::run()
{
    //pass timers
    while (!eventQueueTimer.empty())
    {
        Handler handler = eventQueueTimer.top();
        if (handler.end < std::chrono::system_clock::now())
        {
          /*  v8::Local<v8::Function> function = handler.callback.Get(isolate);
            function->Call(context.Get(isolate), 0, {});
            handler.callback.Empty();
			eventQueueCheck.pop();
			*/
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
