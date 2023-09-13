#include "jwt_controller.h"
#include <jwt-cpp/jwt.h>
using namespace jwt;
void jwtGen (const Json::Value& rec_json)
{
    auto token = jwt::create()
    .set_type("JWS")
    .set_payload_claim("sample", jwt::claim(std::string("test")))
    .sign(jwt::algorithm::hs256{"secret"});
}