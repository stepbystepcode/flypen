#pragma once

#include <drogon/WebSocketController.h>

using namespace drogon;

class MsgWebsock : public drogon::WebSocketController<MsgWebsock>
{
  public:
     void handleNewMessage(const WebSocketConnectionPtr&,
                                  std::string &&,
                                  const WebSocketMessageType &) override;
    void handleNewConnection(const HttpRequestPtr &,
                                     const WebSocketConnectionPtr&) override;
    void handleConnectionClosed(const WebSocketConnectionPtr&) override;
    WS_PATH_LIST_BEGIN
    // list path definitions here;
    WS_PATH_ADD("/api/chat");
    WS_PATH_LIST_END
};
