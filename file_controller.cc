#include "file_controller.h"

#include <json/json.h>
#include <stdio.h>
#include <mysql.h>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>
void add_lock(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback){
    std::string filename = req->getParameter("filename");
    if(lockcheck(filename)){
        auto res = HttpResponse::newHttpResponse();
        res->addHeader("Access-Control-Allow-Origin", "*");
        res->setBody("This file has been occupied");
        callback(res);
    }
    else {
        auto res = HttpResponse::newHttpResponse();
        res->addHeader("Access-Control-Allow-Origin", "*");
        res->setBody("This file is yours");
        callback(res);
    }
}
std::string shell_commons(const char *cmd)
{
    char buffer[128];
    std::string result = "";
    FILE *pipe = popen(cmd, "r");
    if (!pipe)
        throw std::runtime_error("popen() failed!");
    try
    {
        while (fgets(buffer, sizeof buffer, pipe) != NULL)
        {
            result += buffer;
        }
    }
    catch (...)
    {
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
    std::string result = shell_commons(("cd " + std::string(pathvar) + "/.. " + "&&" + "tree -J root").c_str());
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");
    res->setBody(result);
    callback(res);
}
void catFile(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    char *pathvar;
    pathvar = getenv("PWD");
    std::string path = req->getParameter("path");
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");
    std::string result = shell_commons(("cat " + std::string(pathvar) + "/../root/" + path).c_str());
    res->setBody(result);
    callback(res);
}
void saveFile(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto body = req->getBody();
    Json::Value req_json;
    Json::Reader reader;
    std::string bodyStr(body);
    if (!reader.parse(bodyStr, req_json))
    {
        std::cout << "parse failed" << std::endl;
        callback(HttpResponse::newHttpResponse());
        return;
    }
    char *pathvar;
    pathvar = getenv("PWD");
    std::string filename = req_json["filename"].asString();
    std::string content = req_json["content"].asString();
    std::string result = shell_commons(("echo '" + content + "'>" + std::string(pathvar) + "/../root/" + filename).c_str());
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");
    res->setBody("success");
    callback(res);
}

void imageUpload(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    MultiPartParser fileUpload;
    if (fileUpload.parse(req) != 0 || fileUpload.getFiles().size() != 1)
    {
        auto resp = HttpResponse::newHttpResponse();
        resp->setBody("Must only be one file");
        resp->setStatusCode(k403Forbidden);
        callback(resp);
        return;
    }
    auto &file = fileUpload.getFiles()[0];
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    std::string timestamp = std::to_string(ms) + '.' + std::string(file.getFileExtension());
    auto resp = HttpResponse::newHttpResponse();
    resp->addHeader("Access-Control-Allow-Origin", "*");
    resp->setBody(timestamp);
    file.save();
    shell_commons(("mv  ./uploads/" + file.getFileName() + " ./uploads/" + timestamp).c_str());

    LOG_INFO << "The uploaded file has been saved to the ./uploads "
                "directory";
    callback(resp);
    // auto now = std::chrono::high_resolution_clock::now();
    // auto ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    // auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(ms.time_since_epoch());

    // std::stringstream ss;
    // ss << timestamp.count();
    // std::string fileName = ss.str();
    //   // Save image to /root folder
    //   auto imagePath = "/root/" + fileName;
    //   imageFile->second.save(imagePath);

    //   // Send back filename in response
    //   Json::Value resp;
    //   resp["filename"] = fileName;
    //   callback(HttpResponse::newHttpJsonResponse(resp));
}
void getPicture(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    std::string filename = req->getParameter("filename");
    auto resp = HttpResponse::newFileResponse("./uploads/" + filename);
    resp->addHeader("Access-Control-Allow-Origin", "*");
    callback(resp);
}