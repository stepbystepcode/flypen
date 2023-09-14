#include "jwt_controller.h"
#include <iostream>
#include <jwt-cpp/jwt.h>
#include <cstring>
#include <chrono>
using namespace jwt;
std::string jwtGen(const Json::Value& rec_json)
{
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    std::string timestamp = std::to_string(ms);
    auto token = jwt::create()
    .set_type("JWS")
    .set_payload_claim("name", jwt::claim(rec_json["username"].asString()))
    .set_payload_claim("time", jwt::claim(timestamp))
    .sign(jwt::algorithm::hs256{"secret"});
    return std::string(token);
}
// std::string jwtDecode(const std::string& token)
// {
//     auto decoded_token = jwt::decode(token);
//     return decoded_token.get_payload_claim("name").as_string();
// }

std::string jwtDecrypt(const std::string& token)
{
    try {
        auto decoded_token = jwt::decode(token);
        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{"secret"})
            .with_type("JWS");
        verifier.verify(decoded_token);
        return decoded_token.get_payload_claim("name").as_string();
    } catch (const std::exception& e) {
        std::cout<<"Failed to decrypt JWT: " + std::string(e.what());
        return "wrong" ;
    }
}