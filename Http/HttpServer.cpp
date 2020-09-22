#include "HttpServer.h"
#include "HttpReq.h"
#include "HttpResp.h"
#include "Logging.h"
#include "Util.h"
#include <stdio.h>

HttpServer::HttpServer(EventLoop* pLoop)
    :m_pLoop(pLoop)
{
    if(!m_pLoop)
        m_pLoop = new EventLoop();
    m_pTcpServer = std::make_unique<TcpServer>("127.0.0.1", 8080, m_pLoop);
    m_pTcpServer->SetRecvCb(std::bind(&HttpServer::OnMessage, this, std::placeholders::_1, std::placeholders::_2));
    m_pTcpServer->start();
}

HttpServer::~HttpServer()
{

}

void HttpServer::OnMessage(std::shared_ptr<Socket> sock, const char* buff)
{
    HttpReq req;
    if(!req.parseReq(buff))
    {
        sock->send("HTTP/1.1 400 Bad Request\r\n\r\n");
        sock->close(); 
        //TODO: 在TcpServer中释放这个连接
        return;
    }

    HttpResp resp;

    if(req.getHeader("Connection") == "close")
        resp.setCloseConnection(true);
    else 
        resp.setCloseConnection(false);
    
    if(req.getReqType() == ReqType::REQ_GET)
    {
        if (req.getPath() == "/")
        {
            resp.setStatusCode(HttpResp::k200Ok);
            resp.setStatusMessage("OK");
            resp.setContentType("text/html");
            resp.addHeader("Server", "CC");
            std::string now = GetFormatedTime();
            resp.setBody("<html><head><title>This is title</title></head>"
                "<body><h1>Hello</h1>Now is " + now +
                "</body></html>");
        }
        else if (req.getPath() == "/hello")
        {
            resp.setStatusCode(HttpResp::k200Ok);
            resp.setStatusMessage("OK"); 
            resp.setContentType("text/plain");
            resp.addHeader("Server", "CC");
            resp.setBody("hello, world!\n");
        }
        else
        {
            if(req.getPath().back() == '/')
            {
                req.pathAppend("index.html");
            }

            resp.setStatusCode(HttpResp::k200Ok);
            resp.setStatusMessage("OK");
            resp.setContentType(resp.parseContentType(req.getPath()));
            resp.addHeader("index", "CC");
        
            std::string path = req.getPath();
            path = "./bin"+path;

            FILE* pFile = fopen(path.c_str(),"r");
            if(pFile == NULL)
            {
                LOG_INFO << "Open " << path.c_str() <<" failed! " <<strerror(errno) <<ENDL;
                
                resp.setStatusCode(HttpResp::k404NotFound);
                resp.setStatusMessage("Not Found");
                resp.setCloseConnection(true);

                std::string sendBuffer;
                resp.appendToBuffer(sendBuffer);
                sock->send(sendBuffer.c_str());
                return;
            }
            char buffer[102400];
            fread(buffer, 1, 102400, pFile);
            fclose(pFile);
            LOG_INFO << "Read content "<<buffer <<ENDL;

            resp.setBody(buffer);
        }
    }

    std::string sendBuffer;
    resp.appendToBuffer(sendBuffer);
    sock->send(sendBuffer.c_str());
}