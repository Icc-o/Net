#include "AppendFile.h"

AppendFile::AppendFile(std::string fileName)
    :m_pFile(fopen(fileName.c_str(), "a"))
    ,m_fileSize(0)
{

}

AppendFile::~AppendFile()
{
    flush();
    fclose(m_pFile);
}

bool AppendFile::append(const char* buff, uint32_t len)
{
    uint32_t n = fwrite_unlocked(buff, 1, len, m_pFile);
    uint32_t rest = len-n;
    while(rest>0)
    {
        uint32_t nbyte = fwrite_unlocked(buff+n, 1, rest, m_pFile);
        n+=nbyte;
        rest-=nbyte;
    }
    m_fileSize+=len;
}

bool AppendFile::flush()
{
    fflush(m_pFile);
}