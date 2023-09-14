#include "MsgWebsock.h"
#include "mysql.h"
//创建一个socket容器
WebSocketConnectionPtr findTargetConnection(const std::string& targetClientId) {

}
void MsgWebsock::handleNewMessage(const WebSocketConnectionPtr& wsConnPtr, std::string &&message, const WebSocketMessageType &type)
{   std::string sender =" ";
    std::string receiver =" ";
    bool isread =1;
    // write your application logic here
    //解信息获得username和message
    //判断登录状态
    //  WebSocketConnectionPtr targetConnection = findTargetConnection(username);
    //  targetConnection->send(message);
    wsConnPtr->send("Hello, " + message + "!");
    sql_addhistory(sender,receiver,message,isread);
}

void MsgWebsock::handleNewConnection(const HttpRequestPtr &req, const WebSocketConnectionPtr& wsConnPtr)
{
    // write your application logic here

    //sql_addconnect();
}

void MsgWebsock::handleConnectionClosed(const WebSocketConnectionPtr& wsConnPtr)
{   

    // write your application logic here
}
