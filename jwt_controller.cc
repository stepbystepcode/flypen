#include <iostream>
#include <drogon/drogon.h>
#include <jwt-cpp/jwt.h>
#include <cstring>
#include <chrono>
#include "jwt_controller.h"

using namespace jwt;

std::string jwtGen(const Json::Value& req_json)
{
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    std::string timestamp = std::to_string(ms);
    auto token = jwt::create()
    .set_type("JWS")
    .set_payload_claim("name", jwt::claim(req_json["username"].asString()))
    .set_payload_claim("time", jwt::claim(timestamp))
    .sign(jwt::algorithm::hs256{"secret"});
    return std::string(token);
}

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
        std::cout<<"Failed to decrypt JWT: " + std::string(e.what())<<std::endl;
        throw std::runtime_error("Failed to decrypt JWT");
    }
}

bool jwtVerify(const drogon::HttpRequestPtr &req){
    std::string authHeader = req->getHeader("Authorization");
    if (authHeader.substr(0, 7) == "Bearer ") {
        std::string bearerToken = authHeader.substr(7);
        try {
            std::string sender = jwtDecrypt(bearerToken);
            return true;
        } catch (const std::exception &e) {
            std::cout << "Wrong token" << std::endl;
            return false;
        }
    } else {
        std::cout << "No Authorization" << std::endl;
        return false;
    }
}