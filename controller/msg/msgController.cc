#include "msgController.h"
#include "../../pgsql.h"
using namespace msg;

// chat
void ChatController::handleNewConnection(const HttpRequestPtr& req,
                             const WebSocketConnectionPtr& conn)
{
    if(jwtVerify(req)) 
    {
        std::string sender = jwtDecrypt(req->getHeader("Authorization").substr(7));
        conn->setContext(std::make_shared<std::string>(sender));
    }
    else 
    {
        Json::Value res_json;
        res_json["message"] = "jwtVerify in chat(): No Authorization";
        res_json["code"] = 401;

        conn->send(Json::FastWriter().write(res_json));
        conn->forceClose();
    }
}

void ChatController::handleNewMessage(const WebSocketConnectionPtr& conn, 
                          std::string&& message,
                          const WebSocketMessageType& type)
{
    Json::Value req_json, res_json;
    Json::Reader reader;
    Json::FastWriter writer;

    if(!reader.parse(message, req_json)) 
    {
        res_json["message"] = "Invalid Json";
        res_json["code"] = 400;
    }
    else
    {
        std::string sender = *conn->getContext<std::string>();
        std::string content = req_json["content"].asString();
        std::string receiver = req_json["recevier"].asString();

        sql_addhistory(sender, receiver, content, "0");

        res_json["message"] = "Message Send Success!";
        res_json["code"] = 200;
        std::cout << "INFO: " << sender << " sends " << content << " to " << receiver << std::endl;
    }

    conn->send(writer.write(res_json));
}

void ChatController::handleConnectionClosed(const WebSocketConnectionPtr& conn)
{
    std::cout << "WebSocket connection closed\n";
}

// check
void CheckController::handleNewConnection(const HttpRequestPtr& req,
                         const WebSocketConnectionPtr& conn)
{
    if(jwtVerify(req)) 
    {
        auto check_type = req->getParameter("type");
        std::string me = jwtDecrypt(req->getHeader("Authorization").substr(7));
        std::vector<std::string> v{me, check_type};
        conn->setContext(std::make_shared<std::vector<std::string>>(v));
    }
    else
    {
        Json::Value res_json;
        res_json["message"] = "jwtVerify in check(): No Authorization";
        res_json["code"] = 400;

        conn->send(Json::FastWriter().write(res_json));
        conn->forceClose();
    }
}

void CheckController::handleNewMessage(const WebSocketConnectionPtr& conn, 
                          std::string&& message,
                          const WebSocketMessageType& type)
{
    Json::Value res_json, req_json;
    Json::Reader reader;
    Json::FastWriter writer;
    
    if(!reader.parse(message, req_json)) 
    {
        res_json["message"] = "Invalid Json";
        res_json["code"] = 400;
    }
    else
    {
        auto v = *conn->getContext<std::vector<std::string>>();
        std::string me = v[0], check_type = v[1];
        res_json["message"] = sql_find_my_msg(me, check_type);
        res_json["code"] = 200;
    }
    conn->send(writer.write(res_json));
}

void CheckController::handleConnectionClosed(const WebSocketConnectionPtr& conn)
{
    std::cout << "WebSocket connection closed\n";
}

void InfoController::handleNewConnection(const HttpRequestPtr& req,
                             const WebSocketConnectionPtr& conn)
{
    if(jwtVerify(req))
    {
        std::string me = jwtDecrypt(req->getHeader("Authorization").substr(7));
        conn->setContext(std::make_shared<std::string>(me));
    }
    else
    {
        Json::Value res_json;
        res_json["message"] = "jwtVerify in info(): No Authorization";
        res_json["code"] = 401;
        conn->send(Json::FastWriter().write(res_json));
    }
}

void InfoController::handleNewMessage(const WebSocketConnectionPtr& conn, 
                          std::string&& message,
                          const WebSocketMessageType& type)
{
    Json::Value res_json;
    
    auto me = *conn->getContext<std::string>();
    res_json["code"] = 200;
    res_json["message"] = get_my_info(me);

    conn->send(Json::FastWriter().write(res_json));
}

void InfoController::handleConnectionClosed(const WebSocketConnectionPtr& conn)
{
    std::cout << "WebSocket connection closed\n";
}


void NfmgrController::handleNewConnection(const HttpRequestPtr& req,
                             const WebSocketConnectionPtr& conn)
{
    if(jwtVerify(req))
    {
        std::string receiver = jwtDecrypt(req->getHeader("Authorization").substr(7));
        conn->setContext(std::make_shared<std::string>(receiver)); 
    }
    else 
    {
        Json::Value res_json;
        res_json["code"] = 401;
        res_json["message"] = "jwtVerify in nfgmr() No Authorization";
        conn->send(Json::FastWriter().write(res_json));
        conn->forceClose();
    }
}

void NfmgrController::handleNewMessage(const WebSocketConnectionPtr& conn, 
                          std::string&& message,
                          const WebSocketMessageType& type)
{
    Json::Value res_json, req_json;
    Json::Reader reader;

    if(!reader.parse(message, req_json))
    {
        res_json["code"] = 400;
        res_json["message"] = "Invalid JSON";
    }
    else
    {
        std::string sender = req_json["username"].asString();
        std::string attitude = req_json["info"].asString();
        std::string receiver = *conn->getContext<std::string>();

        sql_process_request(sender, receiver, attitude);

        res_json["message"] = "Friends" + attitude + "Success";
        res_json["code"] = 200;
    }

    conn->send(Json::FastWriter().write(res_json));
}

void NfmgrController::handleConnectionClosed(const WebSocketConnectionPtr& conn)
{
    std::cout << "WebSocket connection closed\n";
}

void NewFriendsController::handleNewConnection(const HttpRequestPtr& req,
                             const WebSocketConnectionPtr& conn)
{
    if(jwtVerify(req))
    {
        std::string sender = jwtDecrypt(req->getHeader("Authorization").substr(7));
        conn->setContext(std::make_shared<std::string>(sender)); 
    }
    else
    {
        Json::Value res_json;
        res_json["code"] = 401;
        res_json["message"] = "jwtVerify in NewFriends() No Authorization";
        conn->send(Json::FastWriter().write(res_json));
        conn->forceClose();
    }
}

void NewFriendsController::handleNewMessage(const WebSocketConnectionPtr& conn, 
                          std::string&& message,
                          const WebSocketMessageType& type)
{
    Json::Value res_json, req_json;
    Json::Reader reader;

    if(!reader.parse(message, res_json))
    {
        res_json["code"] = 400;
        res_json["message"] = "Invalid JSON";
    }
    else
    {
        std::string sender = *conn->getContext<std::string>();
        std::string receiver = req_json["username"].asString();
        std::string op = req_json["operation"].asString();

        if(op == "add")
        {
            if(sql_findexist(receiver))
            {
                sql_addrequest(sender, receiver);
                res_json["code"] = 200;
                res_json["message"] = "Operation Success";
            }
            else
            {
                res_json["code"] = 404;
                res_json["message"] = "No this user";
            }
        }
        else
        {
            sql_delete_operation(sender, receiver);
            res_json["code"] = 200;
            res_json["message"] = "Delete Operation Success";
        }
    }
    conn->send(Json::FastWriter().write(res_json));
}

void NewFriendsController::handleConnectionClosed(const WebSocketConnectionPtr& conn)
{
    std::cout << "WebSocket connection closed\n";
}