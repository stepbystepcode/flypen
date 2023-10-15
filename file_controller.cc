#include "file_controller.h"
#include "jwt_controller.h"
#include <json/json.h>
#include <cstdio>
#include <mysql.h>
#include <iostream>
#include <stdexcept>
#include <string>

std::string return_status(std::string result, const std::string& command,Json::Value &res_json)
{
    if (!result.empty())
    {
        result = command + " success";
        res_json["code"] = 200;
    }
    else
    {
        result = " Error in :" + command;
        res_json["code"] = 400;
    }
        
    return result;
}

void add_lock(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    std::string filename = req->getParameter("filename");
    auto res = HttpResponse::newHttpResponse();

    if (lockcheck(filename))
    {
        res->addHeader("Access-Control-Allow-Origin", "*");
        res->setBody("This file has been occupied");
    }
    else
    {
        res->addHeader("Access-Control-Allow-Origin", "*");
        res->setBody("This file is yours");
    }

    callback(res);
}

std::string shell_commands(const char *cmd)
{
    char buffer[1280];
    std::string result;
    FILE *pipe = popen(cmd, "r");

    if (!pipe)
        throw std::runtime_error("popen() failed!");
    try
    {
        while (fgets(buffer, sizeof buffer, pipe) != nullptr)
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
    auto res = HttpResponse::newHttpResponse();

    std::string result, str;
    Json::Value res_json;

    Json::FastWriter writer;
    res->addHeader("Access-Control-Allow-Origin", "*");

    if (jwtVerify(req))
    {                
        enum Command
        {
            tree,
            cp,
            mv,
            rm,
            mkdir,
            touch,
            cat
        } command;

        char *pathvar;
        pathvar = getenv("PWD");

        command = static_cast<Command>(stoi(req->getJsonObject()->get("command", "").asString()));
        std::string params1 = req->getJsonObject()->get("params", "")[0].asString();
        std::string params2 = req->getJsonObject()->get("params", "")[1].asString();

        if ((params1.find("..") != std::string::npos) || 
            (params2.find("..") != std::string::npos))
        {
            result = "Error: result in wrong directory";
            res_json["code"] = 400;
        }
        else
        {
            switch (command)
            {
            case tree:
                str = "tree";
                result = shell_commands(("cd " + std::string(pathvar) + "/.. " + "&&" + 
                                        "tree -J root").c_str());
                

                break;
            case cp:
                str = "cp";
                result = shell_commands(("cp -v " + std::string(pathvar) + "/../root/" + 
                                    params1 + " " + std::string(pathvar) + "/../root/" + 
                                    params2).c_str());

                break;
            case mv:
                str = "mv";
                result = shell_commands(("mv -v " + std::string(pathvar) + "/../root/" + 
                                        params1 + " " + std::string(pathvar) + "/../root/" + 
                                        params2).c_str());

                break;
            case rm:
                str = "rm";
                if (params1.find("..") != std::string::npos)
                {
                    result = "error:result in wrong directory";
                    res_json["code"] = 400;
                    break;
                }

                result = shell_commands(("rm -rf -v " + std::string(pathvar) + "/../root/" +
                                        params1).c_str());

                break;
            case mkdir:

                str = "mkdir";
                result = shell_commands(("mkdir -v " + std::string(pathvar) + "/../root/" + 
                                        params1).c_str());
                break;
            case touch:
                str = "touch";
                if (shell_commands(("ls  -l " + std::string(pathvar) + "/../root/" + 
                                    params1 + " grep ^- ").c_str()).empty())
                {
                    shell_commands(("touch " + std::string(pathvar) + "/../root/" + params1)
                                    .c_str());

                    result = "success";
                    res_json["code"] = 200;
                }
                else
                {
                    result = "error:file already exists";
                    res_json["code"] = 400;
                }
                
                break;
            case cat:
                str = "cat";

                result = shell_commands(("cat " + std::string(pathvar) + "/../root/" + params1)
                                        .c_str());
                res_json["code"] = 200;

                break;
            default:

                result = "Error: command not found";
                res_json["code"] = 400;

                break;
            }

            // TODO:
            result = return_status(result, str, res_json);
        }
    }
    else
    {
        result = "No Authorization";
        res_json["code"] = 401;
    }
    
    res_json["message"] = result;

    res->addHeader("Access-Control-Allow-Origin", "*");
    auto output = writer.write(res_json);
    res->setBody(output);
    callback(res);
}

void saveFile(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");

    Json::Value res_json;
    Json::FastWriter writer;

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

        res_json["code"] = 200;
        res_json["message"] = "File save Success";
    }
    else
    {
        res_json["code"]  = 401;
        res_json["message"] = "No Authorization";
    }
    
    auto output = writer.write(res_json);
    res->setBody(output);
    callback(res);
}

void imageUpload(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto resp = HttpResponse::newHttpResponse();
    resp->addHeader("Access-Control-Allow-Origin", "*");
    if (jwtVerify(req))
    {
        MultiPartParser fileUpload;
        if (fileUpload.parse(req) != 0 || fileUpload.getFiles().size() != 1)
        {
            resp->setBody("Must only be one file");
            resp->setStatusCode(k403Forbidden);
            callback(resp);
            return;
        }
        auto &file = fileUpload.getFiles()[0];
        auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        std::string timestamp = std::to_string(ms) + '.' + std::string(file.getFileExtension());

        resp->setBody(timestamp);
        file.save();
        shell_commands(("mv  ./uploads/" + file.getFileName() + " ./uploads/" + timestamp).c_str());

        LOG_INFO << "The uploaded file has been saved to the ./uploads "
                    "directory";
    }
    else
    {
        resp->setBody("No Authorization");
    }
    callback(resp);
}

void getPicture(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    std::string filename = req->getParameter("filename");
    auto resp = HttpResponse::newFileResponse("./uploads/" + filename);
    resp->addHeader("Access-Control-Allow-Origin", "*");
    callback(resp);
}
