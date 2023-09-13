#include "MsgWebsock.h"

void MsgWebsock::handleNewMessage(const WebSocketConnectionPtr& wsConnPtr, std::string &&message, const WebSocketMessageType &type)
{
    // write your application logic here
     wsConnPtr->send(message);
}

void MsgWebsock::handleNewConnection(const HttpRequestPtr &req, const WebSocketConnectionPtr& wsConnPtr)
{
    // write your application logic here
}

void MsgWebsock::handleConnectionClosed(const WebSocketConnectionPtr& wsConnPtr)
{
    // write your application logic here
}
