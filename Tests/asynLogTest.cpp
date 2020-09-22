#include "Log/AsynLog.h"
#include <iostream>
#include <sys/time.h>
#include "Log/Logging.h"
#include "Util/Util.h"

//测试多线程写日志的效率

using namespace std;

void WriteLog(const char* msg)
{
    AsynLog::getInstance()->push(msg);
}

void WriteThreadFunc()
{
    for(int i=0; i<2000000; i++)
    {
        LOG_INFO << "CC Info for asynlog test !!!"<<i<<ENDL;
    }
}

int main()
{
    cout<<"start: "<<GetFormatedTime()<<endl;
    struct timeval now;
    gettimeofday(&now, NULL);
    uint32_t msStart = now.tv_sec*1000 + now.tv_usec/1000;

    //AsynLog::getInstance()->setProgramName("CCLog");
    Logger::setLogName("ASYN_LOG_TEST");
    Logger::setWriteFunc(WriteLog);
    Logger::setLogLevel(Logger::LOGL_INFO);

    vector<thread> threads;
    for(int i=0; i<thread::hardware_concurrency(); i++)
    {
        threads.emplace_back(WriteThreadFunc);
    }

    for(auto &t : threads)
        if(t.joinable())
            t.join();

    cout<<"finish:  "<<GetFormatedTime()<<endl;
    gettimeofday(&now, NULL);
    uint32_t msEnd = now.tv_sec*1000 + now.tv_usec/1000;
    uint32_t logPms = (thread::hardware_concurrency()*2000000) / (msEnd-msStart);
    cout<<"["<<logPms <<"] logs per ms!"<<endl;
    return 0;
}
