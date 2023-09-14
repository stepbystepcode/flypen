#ifndef _JWT_CONTROLLER_H_
#define _JWT_CONTROLLER_H_
#include <json/json.h>
#include <cstring>
std::string jwtGen(const Json::Value& rec_json);
void jwtVerify(const std::string& token);
#endif