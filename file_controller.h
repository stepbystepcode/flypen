#ifndef _FILE_CONTROLLER_H_
#define _FILE_CONTROLLER_H_
#include <string>
#include <drogon/drogon.h>
using namespace drogon;
std::string shell_commons(const char* cmd);
std::string genTree(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
std::string catFile(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
//std::string 
#endif
