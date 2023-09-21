#include "file_controller.h"
#include "jwt_controller.h"
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
std::string shell_commands(const char *cmd)
{
    char buffer[1280];
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

void commandsCtrl(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{

    std::cout << "commandsCtrl" << std::endl;
    enum Command {
        tree,
        cp,
        mv,
        rm,
        mkdir,
        touch,
        cat
}command;
    char *pathvar;
    pathvar = getenv("PWD");
    
    command = static_cast<Command>(stoi(req->getJsonObject()->get("command","").asString()));
    std::string params1 = req->getJsonObject()->get("params","")[0].asString();
    std::string params2 = req->getJsonObject()->get("params","")[1].asString();
    std::string result;
    switch (command)
    {
    case tree:
        result = shell_commands(("cd " + std::string(pathvar) + "/.. " + "&&" + "tree -J root").c_str());
        break;
    case cp:
        result = shell_commands(("cp -v " + std::string(pathvar) + "/../root/" + params1 + " " + std::string(pathvar) + "/../root/" + params2).c_str());
        if(result!="")
            result = "success";
        else
            result = "error: in cp" ;
        break;
    case mv:
        result = shell_commands(("mv -v " + std::string(pathvar) + "/../root/" + params1 + " " + std::string(pathvar) + "/../root/" + params2).c_str());
        if(result!="")
            result = "success";
        else
            result = "error: in mv" ;
        break;
    case rm:
        if(params1.find("..") != std::string::npos){
            result = "error:result in wrong directory";
            break;
        }
        result = shell_commands(("rm -rf -v " + std::string(pathvar) + "/../root/" + params1).c_str());
        if(result!="")
            result = "success";
        else
            result = "error: in rm" ;
        break;
    case mkdir:
        result = shell_commands(("mkdir -v " + std::string(pathvar) + "/../root/" + params1).c_str());
        if(result!="")
            result = "success";
        else
            result = "error: in mkdir" ;
        break;
    case touch:
        if("" == shell_commands(("ls  -l " + std::string(pathvar) + "/../root/" + params1  + " grep ^- ").c_str()))
        {
            result = shell_commands(("touch " + std::string(pathvar) + "/../root/" + params1).c_str());
                 result = "success";
        }
       
        else
        {
            result = "error:file already exists";
        }
        break;
    case cat:
        result = shell_commands(("cat " + std::string(pathvar) + "/../root/" + params1).c_str());
        break;  
    default:
        result = "error:command not found";
        break;
    }
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");
    res->setBody(result);
    callback(res);

}
// void genTree(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
// {
//     char *pathvar;
//     pathvar = getenv("PWD");
//     std::string result = shell_commands(("cd " + std::string(pathvar) + "/.. " + "&&" + "tree -J root").c_str());
//     auto res = HttpResponse::newHttpResponse();
//     res->addHeader("Access-Control-Allow-Origin", "*");
//     res->setBody(result);
//     callback(res);
// }
// void catFile(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
// {
//     char *pathvar;
//     pathvar = getenv("PWD");
//     std::string path = req->getParameter("path");
//     std::string result = shell_commands(("cat " + std::string(pathvar) + "/../root/" + path).c_str());
//     auto res = HttpResponse::newHttpResponse();
//     res->addHeader("Access-Control-Allow-Origin", "*");
//     res->setBody(result);
//     callback(res);
// }


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
        std::string result = shell_commands(("echo '" + content + "'>" + std::string(pathvar) + "/../root/" + filename).c_str());
        
        sql_unlocked(filename);
        
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
        shell_commands(("mv  ./uploads/" + file.getFileName() + " ./uploads/" + timestamp).c_str());

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

