#include <json/json.h>
#include "jwt_controller.h"
#include "msg_controller.h"
#include "mysql.h"

using namespace drogon;

// send a message
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
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");

    if (jwtVerify(req))
    {
        std::string sender = jwtDecrypt(req->getHeader("Authorization").substr(7));
        std::string content = req_json["content"].asString();
        std::string receiver = req_json["receiver"].asString();

        sql_addhistory(sender, receiver, content, "0");
        std::string msg = req_json["content"].asString();
        res_json["message"] = "message Send Success";
        res_json["code"] = 200;
        std::cout<<"INFO: "<<sender<<" send "<<msg<<" to "<<receiver<<std::endl;
    }
    else
    {
        res_json["message"] = "No Authorization";
        res_json["code"] = 401;
    }

    auto output = writer.write(res_json);
    res->setBody(output);
    callback(res);
}

// get message history or new message
void check(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto check_type = req->getParameter("type");
    Json::Value res_json;
    Json::Reader reader;
    std::string me;
    Json::FastWriter writer;
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");

    if (jwtVerify(req))
    {
        me = jwtDecrypt(req->getHeader("Authorization").substr(7));
        res_json["message"] = sql_find_my_msg(me, check_type);
        res_json["code"] = 200;
    }
    else
    {
        res_json["message"] = "No Authorization";
        res_json["code"] = 401;
    }

    auto output = writer.write(res_json);
    res->setBody(output);

    callback(res);
}

// request new friend or cancel request
void friend_operation(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto res = HttpResponse::newHttpResponse();
    Json::Value res_json;
    Json::FastWriter writer;
    res->addHeader("Access-Control-Allow-Origin", "*");

    if (jwtVerify(req))
    {
        std::string sender = jwtDecrypt(req->getHeader("Authorization").substr(7));
        std::string receiver = req->getParameter("username");
        std::string operation = req->getParameter("operation");

        if (operation == "add")
        {
            if (sql_findexist(receiver))
            {
                sql_addrequest(sender, receiver);
                res_json["code"] = 200;
                res_json["message"] = "Operation Success";
                // res->setBody("Success");
            }
            else
            {
                res_json["code"] = 404;
                res_json["message"] = "No this user";
                // res->setBody("No this body");
            }
            // res->setBody("No this body");
        }
        else
        {
            sql_delete_operation(sender, receiver);
            res_json["code"] = 200;
            res_json["message"] = " Delete Operation Success";
            // res->setBody("Success");
        }
    }
    else
    {
        res_json["code"] = 401;
        res_json["message"] = "No Authorization ";
        // res->setBody("No Authorization");
    }

    auto output = writer.write(res_json);
    res->setBody(output);
    callback(res);
}

// handle new friend request
void request_processing(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    Json::Value res_json;
    Json::FastWriter writer;
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");

    if (jwtVerify(req))
    {
        std::string receiver = jwtDecrypt(req->getHeader("Authorization").substr(7));
        std::string sender = req->getParameter("username");
        std::string attitude = req->getParameter("info");
        sql_process_request(sender, receiver, attitude);
        // res->setBody("Success");
        res_json["code"] = 200;
        res_json["message"] = "Friends " + attitude + " Success";
    }
    else
    {
        // res->setBody("No Authorization");
        res_json["code"] = 401;
        res_json["message"] = "No Authorization";
    }

    auto output = writer.write(res_json);
    res->setBody(output);

    callback(res);
}

// get chat info
void info(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    Json::Value res_json;
    Json::Reader reader;
    std::string me, who_send_me;
    Json::FastWriter writer;

    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");

    if (jwtVerify(req))
    {
        res_json["code"] = 200;
        me = jwtDecrypt(req->getHeader("Authorization").substr(7));
        res_json["message"] = get_my_info(me);
    }
    else
    {
        // res->setBody("No Authorization");
        res_json["code"] = 401;
        res_json["message"] = "No Authorization";
    }

    auto output = writer.write(res_json);
    res->setBody(output);
    callback(res);
}