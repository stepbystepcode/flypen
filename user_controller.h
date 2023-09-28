#ifndef _USER_CONTROLLER_H_
#define _USER_CONTROLLER_H_

#include <drogon/drogon.h>
using namespace drogon;

typedef void (*HandlerFunc)(const Json::Value&, std::string* str, int*);

void Handle(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, HandlerFunc handler);
void registerUser(const Json::Value& req_json, std::string* msg, int* code);
void loginUser(const Json::Value& req_json, std::string* msg, int* code);
void avatar(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
#endif