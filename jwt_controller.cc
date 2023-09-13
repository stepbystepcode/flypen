#include "jwt_controller.h"
#include <iostream>
#include <jwt-cpp/jwt.h>
#include <cstring>
using namespace jwt;
std::string jwtGen(const Json::Value& rec_json)
{
    auto token = jwt::create()
    .set_type("JWS")
    .set_payload_claim("name", jwt::claim(rec_json["username"].asString()))
    .sign(jwt::algorithm::hs256{"secret"});
    return std::string(token);
}