#ifndef _MSG_CONTROLLER_H_
#define _MSG_CONTROLLER_H_
#include <drogon/WebSocketController.h>
using namespace drogon;
void Chat(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
#endif