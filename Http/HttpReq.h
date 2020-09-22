#pragma once
#include <map>
#include <string>

enum class ReqType{
    REQ_NULL= 0,
    REQ_GET = 1,
    REQ_POST= 2,
    REQ_PUT = 3,
    REQ_DEL = 4,
};

class HttpReq
{
private:
    /* data */
    std::map<std::string, std::string>  m_headers;
    ReqType                             m_reqType;
    std::string                         m_loc;

public:
    HttpReq(/* args */);
    ~HttpReq();

    bool parseReq(std::string content);

    std::string getHeader(const std::string& header);
    ReqType getReqType()const;
    std::string getPath()const {return m_loc;}
    void pathAppend(const std::string& str){m_loc.append(str);}
};