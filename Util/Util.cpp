#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "Util.h"
#include <thread>

const time_t BEIJING = 8*60*60;        //东八区

std::string GetFormatedTime(uint64_t microSec)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);         //这是一个c库函数，x86结构下通过与内核空间的共享内存获取时间，不会陷入内核态

    time_t seconds = static_cast<time_t>(tv.tv_sec + BEIJING);
    struct tm tm_time;
    gmtime_r(&seconds, &tm_time);

    char time[30];
    sprintf(time, "%4d-%02d-%02d %02d:%02d:%02d.%03d", tm_time.tm_year+1900,
    tm_time.tm_mon+1, tm_time.tm_mday,
    tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
    (int)tv.tv_usec/1000);
    return time;
}

std::string GetFormatedTime2()
{
    time_t rawtime;
    struct tm *info;
    char buffer[80];
    time( &rawtime );
    info = localtime( &rawtime );
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", info);
    return buffer;
}

void mSleep(const uint32_t &ms)
{
    std::this_thread::sleep_for(std::chrono::microseconds(ms));
}


bool splitStr(std::vector<std::string>& dest, const std::string& str, const std::string &seprator)
{
    int index = 0, start=0;
    std::string substr("");
    do {
        index = str.find_first_of(seprator,start);
        if (index !=std::string::npos) {
           substr= str.substr(start,index-start);
           dest.push_back(substr);

           start = index + seprator.size();
           if (start >= str.size()) {
               break;
           }
        }
    }while(index !=std::string::npos);
}

void makeDaemon()
{
    pid_t pc = fork();
    if(pc < 0)
    {
        printf("fork error while makeDaemon()\n");
        exit(1);
    }
    else if(pc > 0)
    {
        //父进程退出
        exit(0);
    }
    setsid();

    int fd = open("/dev/null", O_RDWR);
    if(fd != -1)
    {
        dup2(fd, 0);
        dup2(fd, 1);
    }
}