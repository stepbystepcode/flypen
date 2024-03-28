  #include <iostream>
#include <pqxx/pqxx>

int main() {
    try {
        // 连接到数据库
  pqxx::connection C("dbname=flypen user=postgres password=xxxxxx hostaddr=127.0.0.1 port=5432");
        if (C.is_open()) {
            std::cout << "Opened database successfully: " << C.dbname() << std::endl;
        } else {
            std::cout << "Can't open database" << std::endl;
            return 1;
        }

        // 创建一个事务对象
        pqxx::work W(C);

        // 删除greetings表（如果存在），然后创建
        W.exec("DROP TABLE IF EXISTS greetings");
        W.exec("CREATE TABLE greetings (id SERIAL PRIMARY KEY, greeting TEXT NOT NULL)");

        // 插入一些问候语
        W.exec("INSERT INTO greetings (greeting) VALUES ('Hello, world!')");
        W.exec("INSERT INTO greetings (greeting) VALUES ('Hi there')");
        W.exec("INSERT INTO greetings (greeting) VALUES ('Welcome to flypen')");

        // 提交事务
        W.commit();

        // 创建一个非事务性的执行对象
        pqxx::nontransaction N(C);

        // 执行SQL查询
        pqxx::result R(N.exec("SELECT greeting FROM greetings"));

        // 打印查询结果
        std::cout << "Greetings in database:" << std::endl;
        for (auto c : R) {
            std::cout << c[0].as<std::string>() << std::endl;
        }

        // 关闭数据库连接
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
