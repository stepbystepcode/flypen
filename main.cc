#include <drogon/drogon.h>

#include "user_controller.h"
using namespace drogon;

int main() {
    drogon::app().addListener("0.0.0.0", 8080);
    std::cout << "ready" << std::endl;
    drogon::app().registerHandler("/api/signup", [](const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
        registerUser(req, std::move(callback));
    });
    drogon::app().registerHandler("/api/login", [](const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
        loginUser(req, std::move(callback));
    });
    drogon::app().run();
    return 0;
}
