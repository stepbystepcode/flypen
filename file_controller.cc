#include "file_controller.h"
#include "jwt_controller.h"
#include <json/json.h>
#include <stdio.h>

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>

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
    auto res = HttpResponse::newHttpResponse();
    if (jwtVerify(req))
    {
        char *pathvar;
        pathvar = getenv("PWD");
        std::string result = shell_commons(("cd " + std::string(pathvar) + "/.. " + "&&" + "tree -J root").c_str());
        res->addHeader("Access-Control-Allow-Origin", "*");
        res->setBody(result);
        callback(res);
    }
    else{
        res->setBody("No Authorization");
    }
}

void catFile(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto res = HttpResponse::newHttpResponse();
    if (jwtVerify(req))
    {
        char *pathvar;
        pathvar = getenv("PWD");
        std::string path = req->getParameter("path");
        std::string result = shell_commons(("cat " + std::string(pathvar) + "/../root/" + path).c_str());

        res->addHeader("Access-Control-Allow-Origin", "*");
        res->setBody(result);

        callback(res);
    }
    else
    {
        res->setBody("No Authorization");
    }
}

void saveFile(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto res = HttpResponse::newHttpResponse();
    if (jwtVerify(req))
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

        res->addHeader("Access-Control-Allow-Origin", "*");
        res->setBody("success");
        callback(res);
    }
    else
    {
        res->setBody("No Authorization");
    }
}

void imageUpload(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto resp = HttpResponse::newHttpResponse();
    if (jwtVerify(req))
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

        resp->addHeader("Access-Control-Allow-Origin", "*");
        resp->setBody(timestamp);
        file.save();
        shell_commons(("mv  ./uploads/" + file.getFileName() + " ./uploads/" + timestamp).c_str());

        LOG_INFO << "The uploaded file has been saved to the ./uploads "
                    "directory";
        callback(resp);
    }
    else
    {
        resp->setBody("No Authorization");
    }
}

void getPicture(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    std::string filename = req->getParameter("filename");
    auto resp = HttpResponse::newFileResponse("./uploads/" + filename);
    if (jwtVerify(req))
    {
        resp->addHeader("Access-Control-Allow-Origin", "*");
        callback(resp);
    }
    else
    {
        resp->setBody("No Authorization");
    }
}