#include "HttpReq.h"
#include <codecvt>
#include <string>
#include <locale>
#include "Util/Util.h"
#include "Logging.h"


HttpReq::HttpReq(/* args */)
    :m_reqType(ReqType::REQ_NULL)
{

}

HttpReq::~HttpReq()
{
    m_headers.clear();
}

bool HttpReq::parseReq(std::string content)
{
    std::vector<std::string> vecStr;
    splitStr(vecStr, content, "\r\n");
    if(vecStr.empty())
        return false;
    
    if(vecStr[0].find("GET") != std::string::npos)
    {
        m_reqType = ReqType::REQ_GET;
    }
    else if(vecStr[0].find("POST") != std::string::npos)
    {
        m_reqType = ReqType::REQ_POST;
    }

    std::vector<std::string> vecHead;
    splitStr(vecHead, vecStr[0], " ");
    m_loc = vecHead[1];
    
    int index = 0;
    for(int i=1; i<vecStr.size(); i++)
    {
        index = vecStr[i].find_first_of(":");
        if(index != std::string::npos)
        {
            m_headers[vecStr[i].substr(0,index)] = vecStr[i].substr(index+1, vecStr[i].length()-index-1);
        }
    }
    return true;
}

std::string HttpReq::getHeader(const std::string& header)
{
    if(m_headers.find(header) == m_headers.end())
        return "";
    
    return m_headers[header];
}

ReqType HttpReq::getReqType()const
{
    return m_reqType;
}