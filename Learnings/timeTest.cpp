#include <sys/time.h>
#include <iostream>
#include <string.h>
#include "../Util/Util.h"

using namespace std;

int main(int argc, char** argv)
{
    cout<<"start time: "<<GetFormatedTime()<<endl;
    if(strcmp(argv[1], "gettimeofday")==0)
    {
        struct timeval tv;
        for(int i=0; i<1000000; i++)
        {
            gettimeofday(&tv, nullptr);
        }
    }
    else if(strcmp(argv[1], "localtime_r")==0)
    {
        time_t curr_time;
        struct tm tmm;
        for(int i=0; i<1000000; i++)
        {
            curr_time = time(NULL);
            localtime_r(&curr_time, &tmm);
        }
    }
    cout<<"end time: "<<GetFormatedTime()<<endl;
    return 0;
}