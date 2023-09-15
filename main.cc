#include <drogon/drogon.h>
#include "msg_websocket.h"
#include "user_controller.h"
#include <drogon/WebSocketController.h>
using namespace drogon;
std::unordered_map<std::string, WebSocketConnectionPtr> clientTable;
int main() {
    drogon::app().addListener("0.0.0.0", 8081);
    std::cout << "ready" << std::endl;
    drogon::app().registerHandler("/api/signup", [](const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
        Handle(req, std::move(callback),registerUser);
    });
    drogon::app().registerHandler("/api/login", [](const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
        Handle(req, std::move(callback),loginUser);
    });

    //drogon::app().registerWebSocketController("/api/chat");

    drogon::app().registerWebSocketController("/api/chat", "MsgWebsock");
    drogon::app().run();
    // std::unordered_map<std::string, WebSocketConnectionPtr> clientTable;
    return 0;
}
