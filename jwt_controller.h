#ifndef _JWT_CONTROLLER_H_
#define _JWT_CONTROLLER_H_
#include <json/json.h>
#include <cstring>
std::string jwtGen(const Json::Value& req_json);
std::string jwtDecrypt(const std::string& token);
#endif