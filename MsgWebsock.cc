#include "MsgWebsock.h"
#include "jwt_controller.h"
#include <json/json.h>
std::set<WebSocketConnectionPtr> MsgWebsock::connections;
void MsgWebsock::handleNewMessage(const WebSocketConnectionPtr& wsConnPtr, std::string &&message, const WebSocketMessageType &type)
{
    auto json =(message);
    Json::Value jsonValue;
    Json::Reader jsonReader;
    if (jsonReader.parse(json, jsonValue))
    {
        // 输出JSON对象的属性值
        std::string content = jsonValue["content"].asString();
        std::string time = jsonValue["time"].asString();
        std::string isread = jsonValue["isread"].asString();
        std::string receiver = jsonValue["receiver"].asString();
        std::string sender = jsonValue["sender"].asString();
       // std::cout << "Name: " << name << std::endl;
        wsConnPtr->send("name");


    }
    else
    {
        std::cout << "Failed to parse JSON" << std::endl;
    }


    // 提取属性值


    // 发送属性值
    
   // wsConnPtr->send(password);
}

void MsgWebsock::handleNewConnection(const HttpRequestPtr &req, const WebSocketConnectionPtr& wsConnPtr)
{
    // write your application logic here
    const std::string &clientIP = req->peerAddr().toIp();
    LOG_INFO << "New WebSocket connection from IP: " << clientIP;
    // 添加连接到集合
    connections.insert(wsConnPtr);
    // 在此处执行任何其他初始化操作
    // 例如，向连接发送欢迎消
    std::string authHeader = req->getHeader("Authorization");
    if (authHeader.substr(0, 7) == "Bearer ")
    {
        std::string bearerToken = authHeader.substr(7);
        // 在此处使用Bearer Token进行身份验证
        std::cout<<jwtDecrypt(bearerToken)<<std::endl;
    }
    else
    {
        // Authorization头部不是Bearer Token
        std::cout<<"sblgy"<<std::endl;
    }
    wsConnPtr->send("Welcome to the WebSocket server! The connect has been established successfully.");
}

void MsgWebsock::handleConnectionClosed(const WebSocketConnectionPtr& wsConnPtr)
{
    // write your application logic here
}
