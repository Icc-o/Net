#include "LogFile.h"
#include "Util/Util.h"

LogFile::LogFile(const char* fileName,uint32_t logSize)
    :m_logSize(logSize)
    ,m_fileName(fileName)
{
    std::string name = std::string(fileName);
    name.append(".");
    name.append(GetFormatedTime());
    name.append(".log");

    m_apFile.reset(new AppendFile(name.c_str()));
}

LogFile::~LogFile()
{
    m_apFile->flush();
}

void LogFile::append(const char* buff, uint32_t len)
{
    std::lock_guard<std::mutex> lk(m_mtx);
    m_apFile->append(buff, len);
    tryRoll();
}

void LogFile::append(std::string buff)
{
    std::lock_guard<std::mutex> lk(m_mtx);
    m_apFile->append(buff.c_str(), buff.size());
    tryRoll();
}

void LogFile::tryRoll()
{
    if(m_apFile->currentSize() >= m_logSize)
    {
        std::string name = std::string(m_fileName);
        name.append(".");
        name.append(GetFormatedTime());
        name.append(".log");
        m_apFile.reset(new AppendFile(name.c_str()));
    }
}

