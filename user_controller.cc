#include <drogon/drogon.h>
#include <json/json.h>
#include "mysql.h"
#include <openssl/sha.h>
#include "jwt_controller.h"
#include "msg_controller.h"
using namespace drogon;
typedef std::string (*HandlerFunc)(const Json::Value&);
void Handle(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, HandlerFunc handler){
    auto body = req->getBody();
    Json::Value req_json,res_json;
    Json::Reader reader;
    std::string bodyStr(body);
    if (!reader.parse(bodyStr, req_json)) {
        callback(HttpResponse::newHttpResponse());
        return;
    }
    Json::FastWriter writer;
    std::string msg = handler(req_json);
    res_json["msg"] = msg;
    if (msg.find("Success")!= std::string::npos){
        res_json["token"] = jwtGen(req_json);
        res_json["username"] = req_json["username"].asString();
    }
    auto output = writer.write(res_json);
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");
    res->setBody(output);
    callback(res);
}
std::string sha256(const std::string str){
  unsigned char hash[SHA256_DIGEST_LENGTH];

  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, str.c_str(), str.size());
  SHA256_Final(hash, &sha256);

  std::stringstream ss;

  for(int i = 0; i < SHA256_DIGEST_LENGTH; i++){
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>( hash[i] );
  }
  return ss.str();
}
std::string registerUser(const Json::Value& req_json) {
    // std::cout<<"Receive Username : "<<req_json["username"].asString()<<std::endl;
    // std::cout<<"Receive Passwd: "<<req_json["password"].asString()<<std::endl;
    if (sql_check(req_json["username"].asString())){
        sql_add(req_json["username"].asString(), sha256(req_json["password"].asString()), req_json["avatar"].asInt());
        return "Sign up Success";
    }else
        return "User already exist";
}
std::string loginUser(const Json::Value& req_json) {
    if (sql_check(req_json["username"].asString(), sha256(req_json["password"].asString()))){
        return "Login Success";
    }else
        return "Login Failed";
}
void avatar(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");
    if (jwtVerify(req)) {
        std::string receiver = jwtDecrypt(req->getHeader("Authorization").substr(7));
        set_avatar(receiver, stoi(req->getParameter("avatar")));
        res->setBody("Success");
    } else {
        res->setBody("No Authorization");
    }
    callback(res);
}