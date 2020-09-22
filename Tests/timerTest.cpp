#include "Logging.h"
#include "Util.h"
#include "EventLoop.h"
#include <sys/time.h>

void runEveryFunc()
{
    LOG_INFO << "runEveryFunc Timer triggered!"<<ENDL;
}

void runAtFunc()
{
    LOG_INFO << "runAtFunc Timer triggered!"<<ENDL;
}

void runAfterFunc()
{
    LOG_INFO << "runAfterFunc Timer triggered!"<<ENDL;
}


int main()
{
    Logger::setLogName("Timer");
    EventLoop loop;

    struct timeval now;
    gettimeofday(&now, NULL);
    loop.runAt(runAtFunc, now.tv_sec*1000 + now.tv_usec/1000 +3000);
    loop.runAfter(runAfterFunc, 2000);
    loop.runEvery(runEveryFunc, 2000);

    while(1)
    {
        mSleep(10000);
    }
    return 0;
}