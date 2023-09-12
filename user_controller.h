#pragma once
#include <drogon/drogon.h>
using namespace drogon;
typedef std::string (*HandlerFunc)(const Json::Value&);
void Handle(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, HandlerFunc handler);
std::string registerUser(const Json::Value& rec_json);
std::string loginUser(const Json::Value& rec_json);