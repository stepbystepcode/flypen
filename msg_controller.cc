#include <drogon/drogon.h>
#include <json/json.h>
#include "mysql.h"
#include "jwt_controller.h"
#include "msg_controller.h"
using namespace drogon;
void Chat(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback){
    auto body = req->getBody();
    Json::Value rec_json,res_json;
    Json::Reader reader;
    std::string bodyStr(body);
    if (!reader.parse(bodyStr, rec_json)) {
        callback(HttpResponse::newHttpResponse());
        return;
    }
    Json::FastWriter writer;
    std::string authHeader = req->getHeader("Authorization");
    if (authHeader.substr(0, 7) == "Bearer ")
    {
        std::string bearerToken = authHeader.substr(7);
        // 在此处使用Bearer Token进行身份验证
        try
        {
            std::string sender = jwtDecrypt(bearerToken);
            std::string content = rec_json["content"].asString();
            std::string receiver = rec_json["receiver"].asString();
            std::cout << "Connect success: " << sender << std::endl;
            sql_addhistory(sender, receiver, content, "0");
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            std::cout << "Wrong token" << std::endl;
        }
    }
    else
    {
        // 连接没有Authorization头部Bearer Token
        std::cout << "No Authorization" << std::endl;
    }
    std::string msg = rec_json["content"].asString();
    auto output = writer.write(res_json);
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");
    res->setBody(output);
    callback(res);
}
void Check(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback){
    auto body = req->getBody();
    Json::Value rec_json,res_json;
    Json::Reader reader;
    std::string bodyStr(body);
    std::string sender;
    Json::FastWriter writer;
    std::string authHeader = req->getHeader("Authorization");
    if (authHeader.substr(0, 7) == "Bearer ")
    {
        std::string bearerToken = authHeader.substr(7);
        // 在此处使用Bearer Token进行身份验证
        try
        {
            sender = jwtDecrypt(bearerToken);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            std::cout << "Wrong token" << std::endl;
        }
    }
    else
    {
        // 连接没有Authorization头部Bearer Token
        std::cout << "No Authorization" << std::endl;
    }

    auto res = HttpResponse::newHttpResponse();
   //  sql_find_my_msg(sender)
    res->addHeader("Access-Control-Allow-Origin", "*");
    //std::cout<<sql_find_my_msg(sender)<<std::endl;
    std::cout<<"sender"<<std::endl;

    res->setBody(sql_find_my_msg(sender));
    callback(res);

}