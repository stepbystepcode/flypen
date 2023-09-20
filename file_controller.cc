#include "file_controller.h"
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <json/json.h>


std::string shell_commons(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

void genTree(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    char *pathvar;
    pathvar = getenv("PWD");
    std::string result = shell_commons(("cd "+std::string(pathvar)+"/.. " + "&&"+"tree -J root" ).c_str()) ;
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");
    res ->setBody(result);  
    callback(res);
}
void catFile(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    char *pathvar;
    pathvar = getenv("PWD");
    std::string path = req->getParameter("path");
    std::string result = shell_commons(("cat "+std::string(pathvar)+"/../root/"+path).c_str()) ;
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");
    res ->setBody(result);  
    callback(res);
}
void saveFile(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto body = req->getBody();
    Json::Value req_json;
    Json::Reader reader;
    std::string bodyStr(body);
    if (!reader.parse(bodyStr, req_json)) {
        std::cout<<"parse failed"<<std::endl;
        callback(HttpResponse::newHttpResponse());
        return;
    }
    char *pathvar;
    pathvar = getenv("PWD");
    std::string filename = req_json["filename"].asString();
    std::string content = req_json["content"].asString();
    std::string result = shell_commons(("echo '"+content+"'>"+std::string(pathvar)+"/../root/"+filename).c_str()) ;
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");
    res ->setBody("success");  
    callback(res);
}
