#ifndef _JWT_CONTROLLER_H_
#define _JWT_CONTROLLER_H_

#include <drogon/drogon.h>
#include <json/json.h>
#include <cstring>

std::string jwtGen(const Json::Value& req_json);
std::string jwtDecrypt(const std::string& token);
bool jwtVerify(const std::__1::shared_ptr<drogon::HttpRequest> &req);

#endif