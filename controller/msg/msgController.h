#ifndef _MSG_CONTROLLER_H
#define _MSG_CONTROLLER_H
#include <drogon/WebSocketController.h>
#include "../../jwt_controller.h"
using namespace drogon;

namespace msg
{

class ChatController : public WebSocketController<ChatController>
{
public:
    void handleNewMessage(const WebSocketConnectionPtr& conn, 
                          std::string&& message,
                          const WebSocketMessageType& type) override;
    void handleNewConnection(const HttpRequestPtr &,
                             const WebSocketConnectionPtr&) override;
    void handleConnectionClosed(const WebSocketConnectionPtr&) override;

public:
    WS_PATH_LIST_BEGIN
    WS_PATH_ADD("/api/msg/chat");
    WS_PATH_LIST_END
};

class InfoController : public WebSocketController<InfoController>
{
public:
    void handleNewMessage(const WebSocketConnectionPtr& conn, 
                          std::string&& message,
                          const WebSocketMessageType& type) override;
    void handleNewConnection(const HttpRequestPtr &,
                             const WebSocketConnectionPtr&) override;
    void handleConnectionClosed(const WebSocketConnectionPtr&) override;

public:
    WS_PATH_LIST_BEGIN
    WS_PATH_ADD("/api/msg/info");
    WS_PATH_LIST_END
};

class CheckController : public WebSocketController<CheckController>
{
public:
    void handleNewMessage(const WebSocketConnectionPtr& conn, 
                          std::string&& message,
                          const WebSocketMessageType& type) override;
    void handleNewConnection(const HttpRequestPtr &,
                             const WebSocketConnectionPtr&) override;
    void handleConnectionClosed(const WebSocketConnectionPtr&) override;

public:
    WS_PATH_LIST_BEGIN
    WS_PATH_ADD("/api/msg/check");
    WS_PATH_LIST_END
};

class NfmgrController : public WebSocketController<NfmgrController>
{
public:
    void handleNewConnection(const HttpRequestPtr& req,
                             const WebSocketConnectionPtr& conn) override;
    void handleNewMessage(const WebSocketConnectionPtr& conn, 
                          std::string&& message,
                          const WebSocketMessageType& type) override;
    void handleConnectionClosed(const WebSocketConnectionPtr& conn) override;
public:
    WS_PATH_LIST_BEGIN
    WS_PATH_ADD("/api/msg/nfmgr");
    WS_PATH_LIST_END
};

class NewFriendsController : public WebSocketController<NewFriendsController>
{
public:
    void handleNewConnection(const HttpRequestPtr &,
                             const WebSocketConnectionPtr&) override;
    void handleNewMessage(const WebSocketConnectionPtr& conn, 
                          std::string&& message,
                          const WebSocketMessageType& type) override;
    void handleConnectionClosed(const WebSocketConnectionPtr&) override;
public:
    WS_PATH_LIST_BEGIN
    WS_PATH_ADD("/api/msg/newfriends");
    WS_PATH_LIST_END
};

};
#endif