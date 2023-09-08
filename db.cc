#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>

using namespace drogon;
using namespace drogon::orm;

int main()
{
    // Create an instance of the Drogon application
    app().addListener("0.0.0.0", 8080);

    // Configure the MySQL connection
    auto clientPtr = DbClient::newMysqlClient("host=192.168.31.78 port=3306 user=root password=abc.123 dbname=flypen");

    // Register a route handler
    app().registerHandler("/query", [clientPtr](const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
        // Execute a MySQL query
        clientPtr->execSqlAsync([callback](const Result& result) {
            // Process the query result
            auto response = HttpResponse::newHttpResponse();
            response->setBody(result.toJsonString());
            callback(response);
        }, "SELECT * FROM table");

    }, {Get});

    // Run the application
    app().run();

    return 0;
}

