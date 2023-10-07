#include <json/json.h>
#include "mysql.h"
#include <iomanip>
#include <openssl/sha.h>
#include "jwt_controller.h"
#include "msg_controller.h"
using namespace drogon;

typedef void (*HandlerFunc)(const Json::Value &, std::string *, int *);
void userInit(std::string username)
{
    std::string sender="Flypen Team";
    std::string message="Welcome to flypen! We are glad to see you here!";
    sql_addhistory( sender, username, message, "0");
    return ;
}

void Handle(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, HandlerFunc handler)
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

    std::string msg;
    int code;

    handler(req_json, &msg, &code);

    res_json["msg"] = msg;
    res_json["code"] = code;

    if (msg.find("Success") != std::string::npos)
    {
        res_json["token"] = jwtGen(req_json);
        res_json["username"] = req_json["username"].asString();
    }

    auto output = writer.write(res_json);
    auto res = HttpResponse::newHttpResponse();

    res->addHeader("Access-Control-Allow-Origin", "*");
    res->setBody(output);

    callback(res);
}

std::string sha256(const std::string& str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];

    SHA256_CTX sha256;

    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);

    std::stringstream ss;

    for (unsigned char i : hash)
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);

    return ss.str();
}

void registerUser(const Json::Value &req_json, std::string *msg, int *code)
{
    if (sql_check(req_json["username"].asString()))
    {
        sql_add(req_json["username"].asString(), sha256(req_json["password"].asString()), req_json["avatar"].asInt());
        
        *msg = "Sign up Success";
        userInit(req_json["username"].asString());
        *code = 200;
    }
    else
    {
        *msg = "User already exist";
        *code = 409;
    }
}

void loginUser(const Json::Value &req_json, std::string *msg, int *code)
{
    if (sql_check(req_json["username"].asString(), sha256(req_json["password"].asString())))
    {
        *msg = "Login Success";
        *code = 200;
    }
    else
    {
        *msg = "Login Failed";
        *code = 401;
    }
}

void avatar(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");

    if (!jwtVerify(req))
        res->setBody("No Authorization");

    std::string receiver = jwtDecrypt(req->getHeader("Authorization").substr(7));
    set_avatar(receiver, stoi(req->getParameter("avatar")));
    res->setBody("Success");

    callback(res);
}
