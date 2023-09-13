#include "jwt_controller.h"
#include <iostream>
#include <jwt-cpp/jwt.h>
#include <cstring>
using namespace jwt;
std::string jwtGen(const Json::Value& rec_json)
{
    auto token = jwt::create()
    .set_type("JWS")
    .set_payload_claim("sample", jwt::claim(std::string("test")))
    .sign(jwt::algorithm::hs256{"secret"});
    return std::string(token);
}