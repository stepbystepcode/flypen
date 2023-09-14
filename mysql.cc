// mysql
#include "mysql.h"
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/mysql_connection.h>
#include "jdbc/mysql_driver.h"
void sql_addconnect(std::string connectptr){
    try{
        sql::mysql::MySQL_Driver *driver;
        driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con;
        con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");
        sql::Statement *tool;
        tool = con->createStatement();
        std::string classmysql = "INSERT INTO users(username,password,time,connection) VALUES (?,?,NOW(),?)";
        sql::PreparedStatement *insertconnect = con->prepareStatement(classmysql);
        insertconnect->setString(4,connectptr);
        delete insertconnect;
        delete tool;
        delete con;
    }
    catch (sql::SQLException &e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
}
void sql_addhistory(std::string sender,std::string receiver,std::string message,std::string isread){
    try{
        sql::mysql::MySQL_Driver *driver;
        driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con;
        con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");
        sql::Statement *tool;
        tool = con->createStatement();
        std::string classmysql = "INSERT INTO chat(content,isread, receiver,sender,time) VALUES (?,?,?,?,NOW())";
        sql::PreparedStatement *insertdata = con->prepareStatement(classmysql);
        insertdata->setString(1, message);
        insertdata->setString(2,isread);
        insertdata->setString(3,sender);
        insertdata->setString(4,receiver);
        insertdata->executeUpdate();
        delete insertdata;
        delete tool;
        delete con;
    }
    catch (sql::SQLException &e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
}
void sql_add(std::string username, std::string passwd) {
    try {
        sql::mysql::MySQL_Driver *driver;
        driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con;
        //con = driver->connect("tcp://localhost:3306", "root", "abc.123");
        //con = driver->connect("tcp://192.168.31.78:3306", "root", "abc.123");
        con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");
        sql::Statement *tool;
        tool = con->createStatement();
       std::string classmysql = "INSERT INTO users(username, password, createtime) VALUES (?, ?, NOW())";

        sql::PreparedStatement *ptool = con->prepareStatement(classmysql);
        ptool->setString(1, username);
        ptool->setString(2, passwd);
        ptool->executeUpdate();
        delete ptool;
        delete tool;
        delete con;
    } catch (sql::SQLException &e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
    
}
// bool sql_check(std::string user) {
//     bool result = false;
//     try {
//         sql::mysql::MySQL_Driver *driver;
//         driver = sql::mysql::get_mysql_driver_instance();
//         sql::Connection *con;
//         con = driver->connect("tcp://localhost:3306", "root", "abc.123");
//         con->setSchema("flypen");
//         std::string sql = "SELECT * FROM users WHERE username = ? LIMIT 1";
//         sql::PreparedStatement *prepStmt = con->prepareStatement(sql);
//         prepStmt->setString(1, user);
//         sql::ResultSet *res = prepStmt->executeQuery();

//         // 获取查询结果
//         if (!(res->next())) {
//             result = true;
//             // 提取所有列的值
//             std::string username = res->getString("username");
//             std::string password = res->getString("password");
//             int createtime = res->getInt("createtime");

//             // 在这里输出或使用提取的值
//             std::cout << "Username: " << username << std::endl;
//             std::cout << "Password: " << password << std::endl;
//             std::cout << "CreateTime: " << createtime << std::endl;
//         }

//         delete res;
//         delete prepStmt;
//         delete con;
//     } catch (sql::SQLException &e) {
//         //  std::cerr << "SQL Exception: " << e.what() << std::endl;
//     }

//     return result;
// }
bool sql_check(std::string user, std::string passwd ) {
    bool result = false;
    try {
        sql::mysql::MySQL_Driver *driver;
        driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con;
        //con = driver->connect("tcp://127.0.0.1:3306", "root", "abc.123");
        //con = driver->connect("tcp://192.168.31.78:3306", "root", "abc.123");
        con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");
        std::string sql = "SELECT * FROM users WHERE username = ? LIMIT 1";
        sql::PreparedStatement *prepStmt = con->prepareStatement(sql);
        prepStmt->setString(1, user);
        sql::ResultSet *res = prepStmt->executeQuery();
      //    std::cout << passwd << std::endl;

        // 获取查询结果
        // (¬A ∧B) ∨ (A ∧¬B)
        if (((passwd != "@DEFAULT@")&&(res->next()))||((passwd == "@DEFAULT@")&&(!(res->next())))) {
            result = true;
            //std::cout << "in" << std::endl;
             //提取所有列的值
            std::string username = res->getString("username");
            std::string password = res->getString("password");
            int createtime = res->getInt("createtime");
            if ( (passwd !=password)&&(passwd != "@DEFAULT@") )
                result = false;
            // 在这里输出或使用提取的值
            // std::cout << "SQL: Username: " << username << std::endl;
            // std::cout << "SQL: Password: " << password << std::endl;
            // std::cout << "SQL: CreateTime: " << createtime << std::endl;
        }

        delete res;
        delete prepStmt;
        delete con;
    } catch (sql::SQLException &e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
    std::cout << result<< std::endl;

    return result;
}
