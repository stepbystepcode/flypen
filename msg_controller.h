#ifndef _MSG_CONTROLLER_H_
#define _MSG_CONTROLLER_H_
#include <drogon/WebSocketController.h>
using namespace drogon;
void chat(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
void info(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
void check(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
void request_processing(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
void friend_request(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
#endif