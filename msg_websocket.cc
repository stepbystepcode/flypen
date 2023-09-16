#include <json/json.h>
#include "msg_websocket.h"
#include "jwt_controller.h"
#include "mysql.h"
extern std::unordered_map<std::string, WebSocketConnectionPtr> clientTable;
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
        std::string sender = jsonValue["sender"].asString();
        wsConnPtr->send("RECIVE: " + message);
        //找连接
        std::unordered_map<std::string, WebSocketConnectionPtr>::iterator it = clientTable.find(receiver);
        if (it != clientTable.end())
        {
            std::string message = "{'" + sender + ':' + content + "'} ";
            WebSocketConnectionPtr targetptr =it->second;
            targetptr->send(message);
            sql_addhistory(sender, receiver, content, "1");
        }
        else sql_addhistory(sender, receiver, content, "0");
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
    std::string bearerToken = req->getHeader("Sec-WebSocket-Protocol");
    if (bearerToken!="")
    {
        // 在此处使用Bearer Token进行身份验证
        try
        {
            user = jwtDecrypt(bearerToken);
            std::cout << "Connect success: " << user << std::endl;
            // 添加到表
            clientTable[user] = wsConnPtr;
            std::shared_ptr<std::string> context = std::make_shared<std::string>(user);
            wsConnPtr->setContext(context);
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
    std::shared_ptr<std::string> strptr = wsConnPtr->getContext<std::string>();
    std::string username = *strptr;
    clientTable[username] = nullptr;
    // write your application logic here
}

