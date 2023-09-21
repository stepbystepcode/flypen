#include "file_controller.h"

#include <json/json.h>
#include <stdio.h>

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>

std::string shell_commons(const char *cmd)
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

void commondsCtrl(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    std::cout << "commondsCtrl" << std::endl;
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
        result = shell_commons(("cd " + std::string(pathvar) + "/.. " + "&&" + "tree -J root").c_str());
        break;
    case cp:
        result = shell_commons(("cp -v " + std::string(pathvar) + "/../root/" + params1 + " " + std::string(pathvar) + "/../root/" + params2).c_str());
        if(result!="")
            result = "success";
        else
            result = "error: in cp" ;
        break;
    case mv:
        result = shell_commons(("mv -v " + std::string(pathvar) + "/../root/" + params1 + " " + std::string(pathvar) + "/../root/" + params2).c_str());
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
        result = shell_commons(("rm -rf -v " + std::string(pathvar) + "/../root/" + params1).c_str());
        if(result!="")
            result = "success";
        else
            result = "error: in rm" ;
        break;
    case mkdir:
        result = shell_commons(("mkdir " + std::string(pathvar) + "/../root/" + params1).c_str());
        if(result!="")
            result = "success";
        else
            result = "error: in mkdir" ;
        break;
    case touch:
        if("" == shell_commons(("ls  -l " + std::string(pathvar) + "/../root/" + params1  + " grep ^- ").c_str()))
        {
            result = shell_commons(("touch " + std::string(pathvar) + "/../root/" + params1).c_str());
                 result = "success";
        }
       
        else
        {
            result = "error:file already exists";
        }
        break;
    case cat:
        result = shell_commons(("cat " + std::string(pathvar) + "/../root/" + params1).c_str());
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
//     std::string result = shell_commons(("cd " + std::string(pathvar) + "/.. " + "&&" + "tree -J root").c_str());
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
//     std::string result = shell_commons(("cat " + std::string(pathvar) + "/../root/" + path).c_str());
//     auto res = HttpResponse::newHttpResponse();
//     res->addHeader("Access-Control-Allow-Origin", "*");
//     res->setBody(result);
//     callback(res);
// }
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
