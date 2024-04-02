#include <pqxx/pqxx>
#include <sstream>
#include <iostream>
#include "json/json.h"
#include <sodium.h>

void sql_add_keypair(std::string username, unsigned char* pk, unsigned char* sk)
{
    try {
        pqxx::connection conn("host=127.0.0.1 port=5432 dbname=flypen user=postgres password=abc.123");
        pqxx::work txn(conn);

        auto encode_bin_to_hex = [](unsigned char* k, std::size_t len) -> std::string {
            std::size_t b64_len = sodium_base64_encoded_len(len, sodium_base64_VARIANT_ORIGINAL);
            char* b64_str = new char[b64_len];

            sodium_bin2base64(b64_str, b64_len, k, len, sodium_base64_VARIANT_ORIGINAL);
            return std::string(b64_str);
        };

        std::string PK = encode_bin_to_hex(pk, crypto_box_PUBLICKEYBYTES);
        std::string SK = encode_bin_to_hex(sk, crypto_box_SECRETKEYBYTES);
        
        txn.exec_params("UPDATE users SET public_key = $1, private_key = $2 WHERE username = $3", 
                                                        PK, SK, username);
        txn.commit();
    } 
    catch (const std::exception &e) {
        std::cerr << "SQL Exception in sql_add_keypair() " << e.what() << std::endl;
    }
}

void sql_unlocked(const std::string &DeleteName) {
    try {
        pqxx::connection conn("host=127.0.0.1 port=5432 dbname=flypen user=postgres password=abc.123");
        pqxx::work txn(conn);
        std::string deleteQuery = "DELETE FROM file WHERE filename = '" + DeleteName + "'";
        txn.exec(deleteQuery);
        txn.commit();
    } catch (const std::exception &e) {
        std::cerr << "SQL Exception: in sql_unlocked() function " << e.what() << std::endl;
    }
}

int sql_findexist(const std::string &receiver) {
    try {
        pqxx::connection conn("host=127.0.0.1 port=5432 dbname=flypen user=postgres password=abc.123");
        pqxx::nontransaction txn(conn);
        std::string readData = "SELECT username FROM users";
        pqxx::result res = txn.exec(readData);
        for (const auto &row : res) {
            if (receiver == row["username"].as<std::string>())
                return 1;
        }
    } catch (const std::exception &e) {
        std::cerr << "SQL Exception in sql_findexist: " << e.what() << std::endl;
    }
    return 0;
}

int lockcheck(const std::string &filename) {
    try {
        pqxx::connection conn("host=127.0.0.1 port=5432 dbname=flypen user=postgres password=abc.123");
        pqxx::work txn(conn);
        std::string readData = "SELECT filename FROM file";
        pqxx::result res = txn.exec(readData);
        for (const auto &row : res) {
            if (row["filename"].as<std::string>() == filename)
                return 1;
        }
        txn.exec_params("INSERT INTO file(filename) VALUES ($1)", filename);
        txn.commit();
    } catch (const std::exception &e) {
        std::cerr << "SQL Exception in lock_check: " << e.what() << std::endl;
    }
    return 0;
}

void process(pqxx::work &txn, std::vector<std::string> s) {
    for (int i = 0; i < 2; i++) {
        std::string readData = "SELECT friends FROM users WHERE username = $1";
        pqxx::result res = txn.exec_params(readData, s[i]);
        std::string friendlist = res[0]["friends"].as<std::string>();
        size_t pos = friendlist.find(s[i + 1]);
        if (pos != std::string::npos && pos != 0)
            friendlist.erase(pos - 1, s[i + 1].length() + 1);
        else if (pos == 0)
            friendlist.erase(pos, s[i + 1].length() + 1);
        std::string changeData = "UPDATE users SET friends = $1 WHERE username = $2";
        txn.exec_params(changeData, friendlist, s[i]);
    }
}

void sql_delete_operation(const std::string &sender, const std::string &receiver) {
    try {
        pqxx::connection conn("host=127.0.0.1 port=5432 dbname=flypen user=postgres password=abc.123");
        pqxx::work txn(conn);
        std::vector<std::string> s{sender, receiver};
        process(txn, s);
        txn.commit();
    } catch (const std::exception &e) {
        std::cerr << "SQL Exception in sql_delete_operation: " << e.what() << std::endl;
    }
}

void sql_process_request(const std::string &sender, const std::string &receiver, const std::string &attitude) {
    try {
        pqxx::connection conn("host=127.0.0.1 port=5432 dbname=flypen user=postgres password=abc.123");
        pqxx::work txn(conn);
        std::string readData = "SELECT req FROM users WHERE username = $1";
        pqxx::result res = txn.exec_params(readData, receiver);
        std::string req = res[0]["req"].as<std::string>();
        size_t pos = req.find(sender);
        if (pos != std::string::npos && pos != 0)
            req.erase(pos - 1, sender.length() + 1);
        else if (pos == 0)
            req.erase(pos, sender.length() + 1);
        std::string updateQuery = "UPDATE users SET req = $1 WHERE username = $2";
        txn.exec_params(updateQuery, req, receiver);

        if (attitude == "allow") {
            std::vector<std::string> users{receiver, sender};
            for (const auto &user : users) {
                std::string readData = "SELECT friends FROM users WHERE username = $1";
                pqxx::result res = txn.exec_params(readData, user);
                std::string friendsList = res[0]["friends"].as<std::string>();
                if (user == receiver) {
                    size_t pos = friendsList.find(sender);
                    if (pos != std::string::npos)
                        continue;
                }
                std::string updateQuery = "UPDATE users SET friends = $1 WHERE username = $2";
                if (!friendsList.empty())
                    txn.exec_params(updateQuery, friendsList + "," + (user == receiver ? sender : receiver), user);
                else
                    txn.exec_params(updateQuery, (user == receiver ? sender : receiver), user);
            }
        }
        txn.commit();
    } catch (const std::exception &e) {
        std::cerr << "SQL Exception in sql_process_request: " << e.what() << std::endl;
    }
}

void sql_addrequest(const std::string &sender, const std::string &receiver) {
    
    if(sender == receiver)
    {
        std::cout << "ERROR: SAME NAME\n";
        return;
    }

    try {
        pqxx::connection conn("host=127.0.0.1 port=5432 dbname=flypen user=postgres password=abc.123");
        pqxx::work txn(conn);

        std::string readData = "SELECT req FROM users WHERE username = $1";
        pqxx::result res = txn.exec_params(readData, receiver);
        
        // 数据库中的字段默认有一个元素为空
        if(res[0][0].is_null()) res.clear();
        
        std::string updateQuery = "UPDATE users SET req = $1 WHERE username = $2";
        if(res.empty()) 
            txn.exec_params(updateQuery, sender, receiver);
        else 
        {
            std::string req = res[0]["req"].as<std::string>();
            size_t pos = req.find(sender);
            if (pos != std::string::npos) return;
            if (!req.empty())
                txn.exec_params(updateQuery, req + "," + sender, receiver);
        }
        txn.commit();
    } 
    catch (const std::exception &e) {
        std::cerr << "SQL Exception in sql_addrequest: " << e.what() << std::endl;
    }
}

void sql_addhistory(const std::string &sender, const std::string &receiver, const std::string &message, const std::string &isread) {
    try {
        pqxx::connection conn("host=127.0.0.1 port=5432 dbname=flypen user=postgres password=abc.123");
        pqxx::work txn(conn);
        std::string insertData = "INSERT INTO chat(content, sender_isread, receiver_isread, sender, receiver, time) VALUES ($1, $2, $3, $4, $5, CURRENT_TIMESTAMP)";
        txn.exec_params(insertData, message, isread, isread, sender, receiver);
        txn.commit();
    } catch (const std::exception &e) {
        std::cerr << "SQL Exception in sql_addhistory: " << e.what() << std::endl;
    }
}

void sql_add(const std::string &username, const std::string &passwd, int avatar) {
    try {
        pqxx::connection conn("host=127.0.0.1 port=5432 dbname=flypen user=postgres password=abc.123");
        pqxx::work txn(conn);
        std::string insertData = "INSERT INTO users(username, password, avatar, friends, createtime) VALUES ($1, $2, $3, 'FlypenTeam', CURRENT_TIMESTAMP)";
        txn.exec_params(insertData, username, passwd, avatar);
        txn.commit();
    } catch (const std::exception &e) {
        std::cerr << "SQL Exception in sql_add: " << e.what() << std::endl;
    }
}

Json::Value get_my_info(const std::string &me) {
    Json::Value json;
    try {
        pqxx::connection conn("host=127.0.0.1 port=5432 dbname=flypen user=postgres password=abc.123");
        pqxx::nontransaction txn(conn);
        if (!me.empty()) {
            std::string sql = "SELECT * FROM users WHERE username = $1 LIMIT 1";
            pqxx::result res = txn.exec_params(sql, me);
            if (!res.empty()) {
                Json::Value user;
                int avatar = res[0]["avatar"].as<int>();
                std::string friends = res[0]["friends"].as<std::string>();
                std::string req = res[0]["req"].as<std::string>();
                std::string registerTime = res[0]["createtime"].as<std::string>();

                auto fetchUserInfo = [&](const std::string &token) -> Json::Value {
                    Json::Value info;
                    info["username"] = token;
                    std::string sql = "SELECT * FROM users WHERE username = $1 LIMIT 1";
                    pqxx::result res = txn.exec_params(sql, token);
                    if (!res.empty()) {
                        info["avatar"] = res[0]["avatar"].as<int>();
                        info["registerTime"] = res[0]["createtime"].as<std::string>();
                    }
                    return info;
                };

                Json::Value friends_array(Json::arrayValue);
                Json::Value req_array(Json::arrayValue);
                std::stringstream sf(friends);
                std::stringstream sr(req);
                std::string token;

                while (std::getline(sf, token, ','))
                    friends_array.append(fetchUserInfo(token));
                while (std::getline(sr, token, ','))
                    req_array.append(fetchUserInfo(token));

                user["avatar"] = avatar;
                user["registerTime"] = registerTime;
                user["friends"] = friends_array;
                user["req"] = req_array;
                json[me] = user;
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "SQL Exception in get_my_info: " << e.what() << std::endl;
    }
    return json;
}

bool sql_check(const std::string &user, const std::string &passwd) {
    bool result = false;
    try {
        pqxx::connection conn("host=127.0.0.1 port=5432 dbname=flypen user=postgres password=abc.123");
        pqxx::nontransaction txn(conn);
        std::string sql = "SELECT * FROM users WHERE username = $1 LIMIT 1";
        pqxx::result res = txn.exec_params(sql, user);
        if (((passwd != "@DEFAULT@") && !res.empty()) || ((passwd == "@DEFAULT@") && res.empty())) {
            result = true;
            if (passwd != res[0]["password"].as<std::string>() && passwd != "@DEFAULT@")
                result = false;
        }
    } catch (const std::exception &e) {
        std::cerr << "SQL Exception in sql_check: " << e.what() << std::endl;
    }
    return result;
}

Json::Value sql_find_my_msg(const std::string &me, const std::string &connect_type) 
{
    Json::Value result;
    try 
    {
        pqxx::connection conn("host=127.0.0.1 port=5432 dbname=flypen user=postgres password=abc.123");
        pqxx::nontransaction txn(conn);

        std::string sql;
        if(connect_type == "all") sql = "SELECT * FROM chat WHERE sender = $1 OR receiver = $2";
        else sql = "SELECT * FROM chat WHERE (sender = $1 AND sender_isread = 0) OR \
                                             (receiver = $2 AND receiver_isread = 0)";

        pqxx::result res = txn.exec_params(sql, me, me);

        std::string sql0To1_sender = "UPDATE chat SET sender_isread = 1 WHERE id = $1";
        std::string sql0To1_rec = "UPDATE chat SET receiver_isread = 1 WHERE id = $1";

        std::map<std::string, Json::Value> sender_messages;

        for (const auto &row : res) 
        {
            if (connect_type == "new") 
            {
                int id = row["id"].as<int>();
                std::string sender = row["sender"].as<std::string>();
                std::string receiver = row["receiver"].as<std::string>();

                if (sender == me)
                    txn.exec_params(sql0To1_sender, id);

                if (receiver == me)
                    txn.exec_params(sql0To1_rec, id);
            }

            Json::Value item;
            std::string sender = row["sender"].as<std::string>();
            std::string receiver = row["receiver"].as<std::string>();

            if (sender == me || receiver == me) 
            {
                std::string key = (sender == me) ? receiver : sender;
                Json::Value &messages = sender_messages[key];

                std::string content = row["content"].as<std::string>();
                std::string time = row["time"].as<std::string>();

                item["content"] = content;
                item["time"] = time;
                item["sender"] = sender;
                item["receiver"] = receiver;

                messages.append(item);
            }
        }

        for (auto &x : sender_messages)
            result[x.first] = x.second;

    } 
    catch (const std::exception &e)
    {
        std::cerr << "SQL Exception in sql_find_my_msg: " << e.what() << std::endl;
    }
    return result;
}

void set_avatar(const std::string &person, int avatar) {
    try {
        pqxx::connection conn("host=127.0.0.1 port=5432 dbname=flypen user=postgres password=abc.123");
        pqxx::work txn(conn);
        std::string update_sql = "UPDATE users SET avatar = $1 WHERE username = $2";
        txn.exec_params(update_sql, avatar, person);
        txn.commit();
    } catch (const std::exception &e) {
        std::cerr << "SQL Exception in set_avatar: " << e.what() << std::endl;
    }
}