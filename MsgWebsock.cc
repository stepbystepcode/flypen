#include "MsgWebsock.h"
#include "jwt_controller.h"
std::set<WebSocketConnectionPtr> MsgWebsock::connections;
void MsgWebsock::handleNewMessage(const WebSocketConnectionPtr& wsConnPtr, std::string &&message, const WebSocketMessageType &type)
{
    // write your application logic here
    message = jwtDecrypt(message);   
     wsConnPtr->send("Hello, " + message + "!");
}

void MsgWebsock::handleNewConnection(const HttpRequestPtr &req, const WebSocketConnectionPtr& wsConnPtr)
{
    // write your application logic here
      const std::string &clientIP = req->peerAddr().toIp();
    LOG_INFO << "New WebSocket connection from IP: " << clientIP;

    // 添加连接到集合
    connections.insert(wsConnPtr);


    // 在此处执行任何其他初始化操作
    // 例如，向连接发送欢迎消息
    wsConnPtr->send("Welcome to the WebSocket server! The connect has been established successfully.");

}

void MsgWebsock::handleConnectionClosed(const WebSocketConnectionPtr& wsConnPtr)
{
    // write your application logic here
}
