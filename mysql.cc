// mysql
#include "jdbc/mysql_driver.h"
#include <jdbc/mysql_connection.h>
#include "mysql.h"
#include <jdbc/cppconn/prepared_statement.h>
void sql_add(std::string username, std::string passwd)
{
    try
    {
        sql::mysql::MySQL_Driver *driver;
        driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con;
        con = driver->connect("tcp://192.168.31.78:3306", "root", "abc.123");
        con->setSchema("flypen");
        sql::Statement *tool;
        tool = con->createStatement();
        std::string classmysql = "INSERT INTO  users(username, password,createtime) VALUES (?, ? ,0)";
        sql::PreparedStatement *ptool = con->prepareStatement(classmysql);
        ptool->setString(1, username);
        ptool->setString(2, passwd);
        ptool->executeUpdate();
        delete ptool;
        delete tool;
        delete con;
    }
    catch (sql::SQLException &e) {
    std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
}