#include <json/json.h>
#include "msg_websocket.h"
#include "jwt_controller.h"
#include "mysql.h"

// WebSocketConnectionPtr findTargetConnection(const std::string& targetClientId) {

// }
std::set<WebSocketConnectionPtr> MsgWebsock::connections;
void MsgWebsock::handleNewMessage(const WebSocketConnectionPtr &wsConnPtr, std::string &&message, const WebSocketMessageType &type)
{
    auto json = (message);
    std ::cout << "recved json: " << json << std::endl;
    Json::Value jsonValue;
    Json::Reader jsonReader;
    if (jsonReader.parse(json, jsonValue))
    {
        // 输出JSON对象的属性值
        std::string content = jsonValue["content"].asString();
        std::string time = jsonValue["time"].asString();
        std::string isread = jsonValue["isread"].asString();
        std::string receiver = jsonValue["receiver"].asString();
        //change next line, dangerous code
        std::string sender = jsonValue["sender"].asString();
        // std::string authHeader = req->getHeader("Authorization"); //error: ‘req’ was not declared in this scope
        // std::string bearerToken = authHeader.substr(7);
        // std::string sender = jwtDecrypt(bearerToken);
        // std::cout << "Name: " << name << std::endl;
        //I need get sender from token, instead of get from json
        wsConnPtr->send("RECIVE: " + message);
        sql_addhistory(sender, receiver, content, isread);
        std::cout << "Success parse JSON" << std::endl;
    }
    else
    {
        std::cout << "Failed to parse JSON" << std::endl;
    }
    // 解信息获得username和message
    // 判断登录状态
    //  WebSocketConnectionPtr targetConnection = findTargetConnection(username);
    //  targetConnection->send(message);
    // 提取属性值
    // 发送属性值
    // wsConnPtr->send(password);
}
void MsgWebsock::handleNewConnection(const HttpRequestPtr &req, const WebSocketConnectionPtr &wsConnPtr)
{
    // sql_addconnect();
    const std::string &clientIP = req->peerAddr().toIp();
    LOG_INFO << "New WebSocket connection from IP: " << clientIP;

    // 在此处执行任何其他初始化操作
    // 例如，向连接发送欢迎消
    std::string user;
    std::string authHeader = req->getHeader("Authorization");
    if (authHeader.substr(0, 7) == "Bearer ")
    {
        std::string bearerToken = authHeader.substr(7);
        // 在此处使用Bearer Token进行身份验证
        try
        {
            user = jwtDecrypt(bearerToken);
            std::cout << "Connect success: " << user << std::endl;
            // 添加连接到集合
            connections.insert(wsConnPtr);
            wsConnPtr->send(sql_find_my_msg(user));
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            std::cout << "Wrong token" << std::endl;
            wsConnPtr->shutdown();
        }
    }
    else
    {
        // 连接没有Authorization头部Bearer Token
        std::cout << "No Authorization" << std::endl;
        wsConnPtr->shutdown();
    }
}

void MsgWebsock::handleConnectionClosed(const WebSocketConnectionPtr &wsConnPtr)
{

    // write your application logic here
}
