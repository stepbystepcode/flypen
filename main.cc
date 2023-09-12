#include <drogon/drogon.h>

#include "user_controller.h"
using namespace drogon;

int main() {
    drogon::app().addListener("0.0.0.0", 8081);
    std::cout << "ready" << std::endl;
    drogon::app().registerHandler("/api/signup", [](const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
        Handle(req, std::move(callback),registerUser);
    });
    drogon::app().registerHandler("/api/login", [](const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
        Handle(req, std::move(callback),loginUser);
    });
    drogon::app().run();
    return 0;
}
