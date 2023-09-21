#include <drogon/drogon.h>
#include <json/json.h>
#include "jwt_controller.h"
#include "msg_controller.h"
#include "mysql.h"

using namespace drogon;

// send a message
void chat(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    auto body = req->getBody();
    Json::Value req_json, res_json;
    Json::Reader reader;
    std::string bodyStr(body);
    if (!reader.parse(bodyStr, req_json)) {
        callback(HttpResponse::newHttpResponse());
        return;
    }
    Json::FastWriter writer;
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");
    if (jwtVerify(req)) {
        std::string sender = jwtDecrypt(req->getHeader("Authorization").substr(7));
        std::string content = req_json["content"].asString();
        std::string receiver = req_json["receiver"].asString();

        sql_addhistory(sender, receiver, content, "0");
        
        std::string msg = req_json["content"].asString();
        auto output = writer.write(res_json);
        res->setBody(output);
    } else {
        res->setBody("No Authorization");
    }
    callback(res);
}
// get message history
void check(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    Json::Value res_json;
    Json::Reader reader;
    std::string me;
    Json::FastWriter writer;
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");
    if (jwtVerify(req)) {
        me = jwtDecrypt(req->getHeader("Authorization").substr(7));
        auto output = writer.write(sql_find_my_msg(me));
        res->setBody(output);
    } else {
        res->setBody("No Authorization");
    }
    callback(res);
}
// request new friend or cancel request
void friend_operation(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");
    if (jwtVerify(req)) {
        std::string sender = jwtDecrypt(req->getHeader("Authorization").substr(7));
        std::string receiver = req->getParameter("username");
        std::string operation = req->getParameter("operation");

        if (operation == "add")
            sql_addrequest(sender, receiver);
        else
            sql_delete_operation(sender, receiver);

        res->setBody("Success");
    } else {
        res->setBody("No Authorization");
    }
    callback(res);
}
// handle new friend request
void request_processing(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");

    if (jwtVerify(req)) {

        std::string receiver = jwtDecrypt(req->getHeader("Authorization").substr(7));
        std::string sender = req->getParameter("username");
        std::string attitude = req->getParameter("info");

        sql_process_request(sender, receiver, attitude);
        res->setBody("Success");
    } else {
        res->setBody("No Authorization");
    }

    callback(res);
}
// get chat info
void info(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    auto body = req->getBody();
    Json::Value req_json, res_json;
    Json::Reader reader;
    std::string bodyStr(body);
    if (!reader.parse(bodyStr, req_json)) {
        callback(HttpResponse::newHttpResponse());
        return;
    }
    std::string me, who_send_me;
    Json::FastWriter writer;
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");

    if (jwtVerify(req)) {

        me = jwtDecrypt(req->getHeader("Authorization").substr(7));
        if (req_json["person"].asString() == "") {
            res->setBody(writer.write(get_chat_info(me, "")));
        } else {
            who_send_me = req_json["person"].asString();
            res->setBody(writer.write(get_chat_info(me, who_send_me)));
        }
        
    } else {
        res->setBody("No Authorization");
    }

    callback(res);
}