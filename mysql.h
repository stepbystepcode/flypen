#ifndef _MYSQL_H_
#define _MYSQL_H_
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <json/json.h>

void sql_unlocked(const std::string& DeleteName);
void sql_add(const std::string& username, const std::string& passwd, int avatar);
bool sql_check(const std::string&, const std::string& passwd = "@DEFAULT@");
void sql_addhistory(const std::string&, const std::string&, const std::string&, const std::string&);
void sql_addconnect(std::string connectptr);

void sql_addrequest(const std::string& send, const std::string& receiver);
void sql_process_request(const std::string&, const std::string&, const std::string&);
Json::Value sql_find_my_msg(const std::string&, const std::string&);
int lockcheck(const std::string& filename);
Json::Value get_my_info(const std::string&);
void sql_delete_operation(const std::string&, const std::string&);
void set_avatar(const std::string& person, int avatar);
int sql_findexist(const std::string& receiver);
#endif