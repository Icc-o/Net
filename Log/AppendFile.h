#pragma once
#include <stdio.h>
#include <stdint.h>
#include <string>
class AppendFile
{
public:
    AppendFile(std::string fileName);
    ~AppendFile();

    bool append(const char* buff, uint32_t len);
    bool flush();
    bool close();
    uint32_t currentSize(){return m_fileSize;}

    AppendFile(const AppendFile&) = delete;
    const AppendFile& operator = (const AppendFile&) = delete;

private:
    FILE*       m_pFile;
    uint32_t    m_fileSize;
};