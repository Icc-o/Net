#pragma once
#include "Log/Logging.h"
//一些默认函数的定义

inline void connected_func()
{
    LOG_INFO <<"Connected!"<<ENDL;
}

inline void disconnected_func()
{
    LOG_INFO << "Disconnected!"<<ENDL;
}

inline void recv_func()
{
    LOG_INFO << "Recv "<<ENDL;
}

inline void timeout_func()
{
    LOG_INFO << "Timeout event!"<<ENDL;
}
