#include <iostream>
#include <string>
#include <pqxx/pqxx>
#include <json/json.h>

void sql_unlocked(const std::string &DeleteName) ;
int sql_findexist(const std::string &receiver) ;
int lockcheck(const std::string &filename) ;
void process(pqxx::work &txn, std::vector<std::string> s) ;
void sql_delete_operation(const std::string &sender, const std::string &receiver) ;
void sql_process_request(const std::string &sender, const std::string &receiver, const std::string &attitude) ;
void sql_addrequest(const std::string &sender, const std::string &receiver) ;
void sql_addhistory(const std::string &sender, const std::string &receiver, const std::string &message, const std::string& nonce, const std::string &isread) ;
void sql_add(const std::string &username, const std::string &passwd, int avatar, const std::string &public_key) ;
Json::Value get_my_info(const std::string &me) ;
bool sql_check(const std::string &usesr, const std::string &passwd = "@DEFAULT@") ;
Json::Value sql_find_my_msg(const std::string &me, const std::string &connect_type) ;
void set_avatar(const std::string &person, int avatar) ;
std::string sql_query_public_key(const std::string& userName);