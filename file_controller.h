#ifndef _FILE_CONTROLLER_H_
#define _FILE_CONTROLLER_H_
#include <string>
#include <drogon/drogon.h>
using namespace drogon;

void saveFile(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
void imageUpload(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
void getPicture(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
void commandsCtrl(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
void add_lock(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
std::string return_status(std::string result, const std::string& command,Json::Value &res_json);

#endif
