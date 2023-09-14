#ifndef _MYSQL_H_ 
#define _MYSQL_H_
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
//#include "mysql.cc"
void sql_add(std::string  , std::string );
bool sql_check(std::string  , std::string passwd="@DEFAULT@");
void sql_addhistory(std::string,std::string,std::string,bool);
void sql_addconnect(std::string connectptr);
//bool sql_check(std::string );
#endif