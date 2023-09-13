#include "jwt_controller.h"
#include <jwt-cpp/jwt.h>
#include <iostream>
//using namespace jwt;
std::string  jwtGen (const Json::Value& rec_json)
{
    auto token = jwt::create()
    .set_type("JWS")
    .set_payload_claim("username", jwt::claim(rec_json["username"].asString()))
    .sign(jwt::algorithm::hs256{"ljlgyzzc"});
    std::cout<<token<<std::endl;
    return token;
}