#ifndef _FILE_CONTROLLER_H_
#define _FILE_CONTROLLER_H_
#include <string>
#include <drogon/drogon.h>
using namespace drogon;
std::string shell_commons(const char *cmd);
// void genTree(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
// void catFile(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
void saveFile(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
void imageUpload(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
void getPicture(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

void commondsCtrl(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

void add_lock(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

// std::string
#endif
