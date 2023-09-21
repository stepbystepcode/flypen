#ifndef _MYSQL_H_ 
#define _MYSQL_H_
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <json/json.h>

void sql_unlocked(std::string DeleteName);
void sql_add(std::string username, std::string passwd, int avatar);
bool sql_check(std::string  , std::string passwd="@DEFAULT@"); 
void sql_addhistory(std::string,std::string,std::string,std::string);
void sql_addconnect(std::string connectptr);

void sql_addrequest(std::string send,std::string receiver);
void sql_process_request(std::string ,std::string,std::string);
Json::Value sql_find_my_msg(std::string);

Json::Value get_chat_info(std::string,std::string="");

void sql_delete_operation(std::string,std::string);
void set_avatar(std::string person, int avatar);

#endif