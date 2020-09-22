#pragma once
#include "AppendFile.h"
#include <memory>
#include <mutex>

//封装AppendFile，加上锁保护
class LogFile
{
public:
    LogFile(const char* fileName, uint32_t logSize=1024*1024*500);
    ~LogFile();

    void append(const char* buff, uint32_t len);
    void append(std::string buff);
    void flush(){m_apFile->flush();}
    void tryRoll();

private:
    std::unique_ptr<AppendFile>     m_apFile;
    std::mutex                      m_mtx;
    uint32_t                        m_logSize;
    std::string                     m_fileName;
};