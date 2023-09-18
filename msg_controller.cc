#include "msg_controller.h"

#include <drogon/drogon.h>
#include <json/json.h>

#include "jwt_controller.h"
#include "mysql.h"
using namespace drogon;
void chat(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto body = req->getBody();
    Json::Value req_json, res_json;
    Json::Reader reader;
    std::string bodyStr(body);
    if (!reader.parse(bodyStr, req_json))
    {
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
            std::string content = req_json["content"].asString();
            std::string receiver = req_json["receiver"].asString();
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
    std::string msg = req_json["content"].asString();
    auto output = writer.write(res_json);
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");
    res->setBody(output);
    callback(res);
}
void check(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    Json::Value res_json;
    Json::Reader reader;
    std::string me;
    Json::FastWriter writer;
    std::string authHeader = req->getHeader("Authorization");
    if (authHeader.substr(0, 7) == "Bearer ")
    {
        std::string bearerToken = authHeader.substr(7);
        // 在此处使用Bearer Token进行身份验证
        try
        {
            me = jwtDecrypt(bearerToken);
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
    res->addHeader("Access-Control-Allow-Origin", "*");
    auto output = writer.write(sql_find_my_msg(me));
    res->setBody(output);
    callback(res);
}

void friend_operation(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    std::string authHeader = req->getHeader("Authorization");
    if (authHeader.substr(0, 7) == "Bearer ")
    {
        std::string bearerToken = authHeader.substr(7);
        // 在此处使用Bearer Token进行身份验证
        try
        {
            std::string sender = jwtDecrypt(bearerToken);
            std::string receiver = req->getParameter("username");
            std::string operation = req->getParameter("operation");
            if(operation=="add")
            sql_addrequest(sender, receiver);
            else sql_delete_operation(sender,receiver);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            std::cout << "Wrong token" << std::endl;
        }
    }
    auto res = HttpResponse::newHttpResponse();
    res->setBody("Success");
    callback(res);
}
void request_processing(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    std::string authHeader = req->getHeader("Authorization");
    if (authHeader.substr(0, 7) == "Bearer ")
    {
        std::string bearerToken = authHeader.substr(7);
        // 在此处使用Bearer Token进行身份验证
        try
        {
            std::string receiver = jwtDecrypt(bearerToken);
            std::string sender = req->getParameter("username");
            std::string attitude = req->getParameter("info");
            sql_process_request(sender, receiver, attitude);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            std::cout << "Wrong token" << std::endl;
        }
    }
    auto res = HttpResponse::newHttpResponse();
    res->setBody("Success");
}
void info(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto body = req->getBody();
    Json::Value req_json, res_json;
    Json::Reader reader;
    std::string bodyStr(body);
    if (!reader.parse(bodyStr, req_json))
    {
        callback(HttpResponse::newHttpResponse());
        return;
    }
    std::string me, who_send_me;
    Json::FastWriter writer;
    std::string authHeader = req->getHeader("Authorization");
    if (authHeader.substr(0, 7) == "Bearer ")
    {
        std::string bearerToken = authHeader.substr(7);
        // 在此处使用Bearer Token进行身份验证
        try
        {
            me = jwtDecrypt(bearerToken);
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
    res->addHeader("Access-Control-Allow-Origin", "*");
    if (req_json["person"].asString() == "")
    {
        res->setBody(writer.write(get_chat_info(me, "")));
        callback(res);
    }
    else
    {
        who_send_me = req_json["person"].asString();
        res->setBody(writer.write(get_chat_info(me, who_send_me)));
        callback(res);
    }
}