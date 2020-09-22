#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <thread>

using namespace std;

int main()
{
    thread t([](){cout<<syscall(224)<<endl;});
    t.join();
    pid_t a = syscall(224);
    cout<<a<<endl;
    return 0;
}