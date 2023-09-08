#pragma once
#include <drogon/drogon.h>
using namespace drogon;

void registerUser(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);