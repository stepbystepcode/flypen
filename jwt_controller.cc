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
void jwtVerify(const std::string& token)
{
    try
    {
        auto decoded = jwt::decode(token);
        std::cout << "Header: " << decoded.get_header() << std::endl;
        std::cout << "Payload: " << decoded.get_payload() << std::endl;
        std::cout << "Signature: " << decoded.get_signature() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "Error info: " << e.what() << std::endl;
    }
}