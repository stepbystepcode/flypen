#include <drogon/drogon.h>
#include <json/json.h>
#include "mysql.h"
#include "jwt_controller.h"
#include "MsgWebsock.h"
using namespace drogon;
typedef std::string (*HandlerFunc)(const Json::Value&);
void Handle(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, HandlerFunc handler){
    auto body = req->getBody();
    Json::Value rec_json,res_json;
    Json::Reader reader;
    std::string bodyStr(body);
    if (!reader.parse(bodyStr, rec_json)) {
        callback(HttpResponse::newHttpResponse());
        return;
    }
    Json::FastWriter writer;
    std::string msg = handler(rec_json);
    res_json["msg"] = msg;
    if (msg=="Login Success")
        res_json["token"] = jwtGen(rec_json);
    else
        res_json["token"] = "";
    auto output = writer.write(res_json);
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");
    res->setBody(output);
    callback(res);
}
std::string registerUser(const Json::Value& rec_json) {
    // std::cout<<"Receive Username : "<<rec_json["username"].asString()<<std::endl;
    // std::cout<<"Receive Passwd: "<<rec_json["password"].asString()<<std::endl;
    if (sql_check(rec_json["username"].asString())){
        sql_add(rec_json["username"].asString(), rec_json["password"].asString());
        return "Sign up Success";
    }else
        return "User already exist";
}
std::string loginUser(const Json::Value& rec_json) {
    if (sql_check(rec_json["username"].asString(), rec_json["password"].asString())){
        return "Login Success";
    }else
        return "Login Failed";
}