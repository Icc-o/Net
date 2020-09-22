#include "Util.h"
#include "Logging.h"
#include "HttpServer.h"

using namespace std;

int main()
{
    Logger::setLogName("Http");
    
    /*std::string testStr = "AAAAA\r\nBBBBB\r\nCCCCC\r\n";
    vector<string> vecStr;
    splitStr(vecStr, testStr,"\r\n");
    for(auto &s : vecStr)
        LOG_INFO << s <<ENDL;
    */
   HttpServer server;
   
   while(1){mSleep(1000);}

    return 0;
}