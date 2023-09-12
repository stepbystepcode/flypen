#include <drogon/drogon.h>
#include <json/json.h>

#include "mysql.h"
using namespace drogon;

void registerUser(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    auto body = req->getBody();
    Json::Value root;
    Json::Reader reader;
    std::string bodyStr(body);
    if (!reader.parse(bodyStr, root)) {
        callback(HttpResponse::newHttpResponse());
        return;
    }
    Json::FastWriter writer;
    auto output = writer.write(root);
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");
    std::cout << "username :" << root["username"].asString() << std::endl;
    std::cout << "passwd :" << root["password"].asString() << std::endl;
    res->setBody(output);
    if (sql_check(root["username"].asString()))
        sql_add(root["username"].asString(), root["password"].asString());
    else
        std::cout << "User already exist" << std::endl;
    callback(res);
}
void loginUser(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    auto body = req->getBody();
    Json::Value rec_json,res_json;
    Json::Reader reader;
    std::string bodyStr(body);
    if (!reader.parse(bodyStr, rec_json)) {
        callback(HttpResponse::newHttpResponse());
        return;
    }
    Json::FastWriter writer;
    std::cout << "The Collected username: " << rec_json["username"].asString() << std::endl;
    std::cout << "The Collected password: " << rec_json["password"].asString() << std::endl;
    if (sql_check(rec_json["username"].asString(), rec_json["password"].asString())){
        std::cout << "Success" << std::endl;
        //3.4,3.12
        res_json["msg"]="Success";
    }else{
        std::cout << "Failed" << std::endl;
        res_json["msg"]="Failed";
    }
    auto output = writer.write(res_json);
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");
    res->setBody(output);
    callback(res);
}