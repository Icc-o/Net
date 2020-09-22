#include "ThreadPool/ThreadPool.h"
#include <iostream>
#include "Util/Util.h"
#include "Log/AsynLog.h"
#include "Log/Logging.h"

using namespace std;

void easyFunc()
{
    for(int i=0; i<10000; i++)
    {
        cout<<__FUNCTION__<<":"<<__LINE__<<this_thread::get_id()<<endl;
        LOG_INFO <<"AAAAAAAAAAAAAAAAAAA"<<ENDL;
    }
}

class Function
{
public:
    void func(){
        for(int i=0; i<10000; i++)
            cout<<"Function::func"<<":"<<__LINE__<<this_thread::get_id()<<endl;
    }
};


int main()
{
    ThreadPool pool(10);
    Function f;
    for(int i=0; i<20; i++)
    {
        pool.add_work(easyFunc);
        pool.add_work(std::bind(&Function::func, &f));
    }

    return 0;
}
