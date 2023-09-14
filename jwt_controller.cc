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
// std::string jwtDecode(const std::string& token)
// {
//     auto decoded_token = jwt::decode(token);
//     return decoded_token.get_payload_claim("name").as_string();
// }

std::string jwtDecrypt(const std::string& token)
{
    auto decrypted_token = jwt::decode(token);
    auto verifier = jwt::verify()
        .allow_algorithm(jwt::algorithm::hs256{"secret"})
        .with_issuer("auth0");
    verifier.verify(decrypted_token);
    return decrypted_token.get_payload_claim("name").as_string();
}