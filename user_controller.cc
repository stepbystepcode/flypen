#include <json/json.h>
#include <drogon/drogon.h>
#include "mysql.h"
using namespace drogon;

void registerUser(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto body = req->getBody();
    Json::Value root;
    Json::Reader reader;
    std::string bodyStr(body);
    if (!reader.parse(bodyStr, root))
    {
        callback(HttpResponse::newHttpResponse());
        return;
    }
    Json::FastWriter writer;
    auto output = writer.write(root);

    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");
    std::cout <<"username :"<< root["username"].asString() << std::endl;
    std::cout <<"passwd :"<< root["password"].asString() << std::endl;
    res->setBody(output);
    if(sql_check(root["username"].asString())) 
        sql_add(root["username"].asString(), root["password"].asString());
    else std::cout<<"sblgy"<<std::endl; 

    callback(res);
}