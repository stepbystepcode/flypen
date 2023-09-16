#ifndef _USER_CONTROLLER_H_
#define _USER_CONTROLLER_H_
#include <drogon/drogon.h>
using namespace drogon;
typedef std::string (*HandlerFunc)(const Json::Value&);
void Handle(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, HandlerFunc handler);
std::string registerUser(const Json::Value& req_json);
std::string loginUser(const Json::Value& req_json);
#endif