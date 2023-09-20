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

std::string genTree(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    char *pathvar;
    pathvar = getenv("PWD");
    std::string result = shell_commons(("cd "+std::string(pathvar)+"/.. " + "&&"+"tree -J root" ).c_str()) ;
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");
    res ->setBody(result);  
    callback(res);
    return result;
}
std::string catFile(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    char *pathvar;
    pathvar = getenv("PWD");
    std::string path = req->getParameter("path");
    std::string result = shell_commons(("cat "+std::string(pathvar)+"/../root/"+path).c_str()) ;
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");
    res ->setBody(result);  
    callback(res);
    return result;
}
