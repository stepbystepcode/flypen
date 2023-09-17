<<<<<<< HEAD
#ifndef _MYSQL_H_ 
#define _MYSQL_H_
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <json/json.h>
//#include "mysql.cc"
void sql_add(std::string  , std::string );
bool sql_check(std::string  , std::string passwd="@DEFAULT@");
void sql_addhistory(std::string,std::string,std::string,std::string);
void sql_addconnect(std::string connectptr);
Json::Value sql_find_my_msg(std::string,std::string);
Json::Value get_chat_info(std::string,std::string);
//bool sql_check(std::string );
=======

#ifndef _MYSQL_H_ 
#define _MYSQL_H_
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <json/json.h>
//#include "mysql.cc"
void sql_add(std::string  , std::string , int );
bool sql_check(std::string , std::string passwd="@DEFAULT@");
void sql_addhistory(std::string,std::string,std::string,std::string);
void sql_addconnect(std::string connectptr);
Json::Value sql_find_my_msg(std::string,std::string);
//bool sql_check(std::string );
>>>>>>> f57de4b287d009d8cea1c1611e6714868588b3b5
#endif