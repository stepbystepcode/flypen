// mysql
#include "mysql.h"

#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/mysql_connection.h>

#include "jdbc/mysql_driver.h"
#include "json/json.h"

void sql_delete_operation(std::string sender, std::string receiver)
{
    std::vector<std::string> usernamelist;
    sql::mysql::MySQL_Driver *driver;
    driver = sql::mysql::get_mysql_driver_instance();
    sql::Connection *con;
    con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
    con->setSchema("flypen");
    std::string readdata = "SELECT friends FROM users WHERE username = ?";
    std::string Sfriendlist, Rfriendlist;
    std::string changedata = "UPDATE users SET friends=? WHERE username =?";
    sql::PreparedStatement *readdatament = con->prepareStatement(readdata);
    readdatament->setString(1, sender);
    sql::ResultSet *SresultSet = readdatament->executeQuery();
    if (SresultSet->next())
    {
        Sfriendlist = SresultSet->getString("friends");
    }
    int pos = Sfriendlist.find(receiver);
    if (pos != std::string::npos && pos != 0)
        Sfriendlist.erase(pos - 1, receiver.length() + 1);
    else if (pos == 0)
        Sfriendlist.erase(pos, receiver.length() + 1);
    std::cout << Sfriendlist << "\n";
    sql::PreparedStatement *updateStatement = con->prepareStatement(changedata);
    updateStatement->setString(1, Sfriendlist);
    updateStatement->setString(2, sender);
    updateStatement->execute();
    readdatament->setString(1, receiver);
    sql::ResultSet *RresultSet = readdatament->executeQuery();
    if (RresultSet->next())
    {
        Rfriendlist = RresultSet->getString("friends");
    }
    pos = Rfriendlist.find(sender);
    if (pos != std::string::npos && pos != 0)
        Rfriendlist.erase(pos - 1, sender.length() + 1);
    else if (pos == 0)
        Rfriendlist.erase(pos, sender.length() + 1);
    std::cout << Rfriendlist << "\n";
    updateStatement->setString(1, Rfriendlist);
    updateStatement->setString(2, receiver);
    updateStatement->execute();
}
void sql_process_request(std::string sender, std::string receiver, std::string attitude)
{
    std::vector<std::string> usernamelist;
    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
    sql::Connection *con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");

    con->setSchema("flypen");

    std::string readdata = "SELECT req FROM users WHERE username = ?";
    sql::PreparedStatement *readdatament = con->prepareStatement(readdata);

    readdatament->setString(1, receiver);

    sql::ResultSet *resultSet = readdatament->executeQuery();
    std::string req;

    if (resultSet->next())
        req = resultSet->getString("req");

    size_t pos = req.find(sender);

    if (pos != std::string::npos && pos != 0)
        req.erase(pos - 1, sender.length() + 1);
    else if (pos == 0)
        req.erase(pos, sender.length() + 1);

    std::string updateQuery = "UPDATE users SET req = ? WHERE username = ?";
    sql::PreparedStatement *updateStatement = con->prepareStatement(updateQuery);

    updateStatement->setString(1, req);
    updateStatement->setString(2, receiver);
    updateStatement->execute();

    if (attitude == "allow")
    {
        std::string readdata = "SELECT friends FROM users WHERE username = ?";
        sql::PreparedStatement *Rreaddatament = con->prepareStatement(readdata);
        sql::PreparedStatement *Sreaddatament = con->prepareStatement(readdata);

        Rreaddatament->setString(1, receiver);
        Sreaddatament->setString(1, sender);
        sql::ResultSet *RresultSet = Rreaddatament->executeQuery();
        sql::ResultSet *SresultSet = Sreaddatament->executeQuery();
        std::string Rfriendslist;
        std::string Sfriendslist;
        if (RresultSet->next())
            Rfriendslist = RresultSet->getString("friends");
        if (SresultSet->next())
            Sfriendslist = SresultSet->getString("friends");
        size_t pos = Rfriendslist.find(sender);
        if (pos != std::string::npos)
            return;
        std::string updateQuery = "UPDATE users SET friends = ? WHERE username = ?";
        sql::PreparedStatement *updateR = con->prepareStatement(updateQuery);
        sql::PreparedStatement *updateS = con->prepareStatement(updateQuery);

        if (Rfriendslist != "")
        {
            Rfriendslist += "," + sender;
            updateR->setString(1, Rfriendslist);
            updateR->setString(2, receiver);
            updateR->execute();
        }
        else
        {
            updateR->setString(1, sender);
            updateR->setString(2, receiver);
            updateR->execute();
        }

        if (Sfriendslist != "")
        {
            Sfriendslist += "," + receiver;
            updateS->setString(1, Sfriendslist);
            updateS->setString(2, sender);
            updateS->execute();
        }
        else
        {
            updateS->setString(1, receiver);
            updateS->setString(2, sender);
            updateS->execute();
        }
    }
    return;
}

void sql_addrequest(std::string sender, std::string receiver)
{
    std::vector<std::string> usernamelist;
    sql::mysql::MySQL_Driver *driver;
    driver = sql::mysql::get_mysql_driver_instance();
    sql::Connection *con;
    con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
    con->setSchema("flypen");
    sql::Statement *tool;
    tool = con->createStatement();
    std::string readdata = "SELECT req FROM users WHERE username = ?";
    sql::PreparedStatement *readdatament = con->prepareStatement(readdata);
    readdatament->setString(1, receiver);
    sql::ResultSet *resultSet = readdatament->executeQuery();
    if (resultSet->next())
    {
        std::string req = resultSet->getString("req");
        size_t pos = req.find(sender);
        if (pos != std::string::npos)
            return;
        std::string updateQuery = "UPDATE users SET req = ? WHERE username = ?";
        sql::PreparedStatement *updateStatement = con->prepareStatement(updateQuery);
        if (req != "")
        {
            req += "," + sender;
            updateStatement->setString(1, req);
            updateStatement->setString(2, receiver);
            updateStatement->execute();
            return;
        }
        updateStatement->setString(1, sender);
        updateStatement->setString(2, receiver);
        updateStatement->execute();
    }
}

void sql_addconnect(std::string connectptr)
{
    try
    {
        sql::mysql::MySQL_Driver *driver;
        driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con;
        con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");
        // sql::Statement *tool;
        // tool = con->createStatement();
        std::string classmysql = "INSERT INTO users(username,password,time,connection) VALUES (?,?,NOW(),?)";
        sql::PreparedStatement *insertconnect = con->prepareStatement(classmysql);
        insertconnect->setString(4, connectptr);
        delete insertconnect;
        // delete tool;
        delete con;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
}

void sql_addhistory(std::string sender, std::string receiver, std::string message, std::string isread)
{
    try
    {
        sql::mysql::MySQL_Driver *driver;
        driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con;
        con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");
        // sql::Statement *tool;
        // tool = con->createStatement();
        std::string classmysql = "INSERT INTO chat(content,isread, sender,receiver,time) VALUES (?,?,?,?,NOW())";
        sql::PreparedStatement *insertdata = con->prepareStatement(classmysql);
        insertdata->setString(1, message);
        insertdata->setString(2, isread);
        insertdata->setString(3, sender);
        insertdata->setString(4, receiver);
        insertdata->executeUpdate();
        delete insertdata;
        // delete tool;
        delete con;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
}

void sql_add(std::string username, std::string passwd, int avatar)
{
    try
    {
        sql::mysql::MySQL_Driver *driver;
        driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con;
        // con = driver->connect("tcp://localhost:3306", "root", "abc.123");
        // con = driver->connect("tcp://192.168.31.78:3306", "root", "abc.123");
        con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");
        sql::Statement *tool;
        tool = con->createStatement();
        std::string classmysql = "INSERT INTO users(username, password, avatar, createtime) VALUES (?, ?, ?, NOW())";
        sql::PreparedStatement *ptool = con->prepareStatement(classmysql);
        ptool->setString(1, username);
        ptool->setString(2, passwd);
        ptool->setInt(3, avatar);
        ptool->executeUpdate();
        delete ptool;
        delete tool;
        delete con;
    }
    catch (sql::SQLException &e)
    {
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

Json::Value get_chat_info(std::string me, std::string who_send_me)
{
    Json::Value json;
    std::string send[2] = {me, who_send_me};

    try
    {
        sql::mysql::MySQL_Driver *driver;
        driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con;
        con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");

        for (int i = 0; i < 2; i++)
        {
            if (!send[i].empty())
            {
                std::string sql = "SELECT * FROM users WHERE username = ? LIMIT 1";
                sql::PreparedStatement *prepStmt = con->prepareStatement(sql);
                prepStmt->setString(1, send[i]);

                sql::ResultSet *res = prepStmt->executeQuery();

                if (res->next())
                {
                    Json::Value user;
                    int avatar = res->getInt("avatar");
                    std::string friends = res->getString("friends");
                    std::string req = res->getString("req");
                    Json::Value friends_array(Json::arrayValue);
                    Json::Value req_array(Json::arrayValue);
                    std::stringstream sf(friends);
                    std::stringstream sr(req);
                    std::string token;
                    while (std::getline(sf, token, ','))
                    {
                        Json::Value afriend;
                        afriend["username"] = token;
                        std::string sql = "SELECT * FROM users WHERE username = ? LIMIT 1";
                        sql::PreparedStatement *prepStmt = con->prepareStatement(sql);
                        prepStmt->setString(1, token);
                        sql::ResultSet *res = prepStmt->executeQuery();
                        if (res->next())
                            afriend["avatar"] = res->getInt("avatar"); // 假设avatar统一为4
                        friends_array.append(afriend);
                    }
                    while (std::getline(sr, token, ','))
                    {
                        Json::Value areq;
                        areq["username"] = token;
                        std::string sql = "SELECT * FROM users WHERE username = ? LIMIT 1";
                        sql::PreparedStatement *prepStmt = con->prepareStatement(sql);
                        prepStmt->setString(1, token);
                        sql::ResultSet *res = prepStmt->executeQuery();
                        if (res->next())
                            areq["avatar"] = res->getInt("avatar"); // 假设avatar统一为4
                        req_array.append(areq);
                    }
                    user["avatar"] = avatar;
                    user["friends"] = friends_array;
                    user["req"] = req_array;

                    Json::StreamWriterBuilder builder;
                    std::string userJson = Json::writeString(builder, user);

                    json[send[i]] = user;
                }
            }
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }

    return json;
}

bool sql_check(std::string user, std::string passwd)
{
    bool result = false;
    try
    {
        sql::mysql::MySQL_Driver *driver;
        driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con;
        // con = driver->connect("tcp://127.0.0.1:3306", "root", "abc.123");
        // con = driver->connect("tcp://192.168.31.78:3306", "root", "abc.123");
        con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");
        std::string sql = "SELECT * FROM users WHERE username = ? LIMIT 1";
        sql::PreparedStatement *prepStmt = con->prepareStatement(sql);
        prepStmt->setString(1, user);
        sql::ResultSet *res = prepStmt->executeQuery();
        //    std::cout << passwd << std::endl;

        // 获取查询结果
        // (¬A ∧B) ∨ (A ∧¬B)
        if (((passwd != "@DEFAULT@") && (res->next())) || ((passwd == "@DEFAULT@") && (!(res->next()))))
        {
            result = true;
            // std::cout << "in" << std::endl;
            // 提取所有列的值
            std::string username = res->getString("username");
            std::string password = res->getString("password");
            int createtime = res->getInt("createtime");
            if ((passwd != password) && (passwd != "@DEFAULT@"))
                result = false;
            // 在这里输出或使用提取的值
            // std::cout << "SQL: Username: " << username << std::endl;
            // std::cout << "SQL: Password: " << password << std::endl;
            // std::cout << "SQL: CreateTime: " << createtime << std::endl;
        }

        delete res;
        delete prepStmt;
        delete con;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
    std::cout << result << std::endl;

    return result;
}

Json::Value sql_find_my_msg(std::string me)
{
    std::cout << "login user: " << me << std::endl;
    try
    {
        sql::mysql::MySQL_Driver *driver;
        driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con;
        con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");
        ////////////////////////////////////////////find begin
        std::string sqlFind = "SELECT * FROM chat WHERE sender = ? OR receiver = ?";
        sql::PreparedStatement *prepStmt = con->prepareStatement(sqlFind);
        prepStmt->setString(1, me);
        prepStmt->setString(2, me);
        sql::ResultSet *res = prepStmt->executeQuery();
        ////////////////////////////////////////////find end and  update begin
        std::string sql0To1 = "UPDATE chat SET isread = 1 WHERE id = ?";
        // sql::PreparedStatement *prepStmt0To1 = con->prepareStatement(sql0To1);
        int id;
        ///////////////////////////////////////////update end

        Json::Value result;
        std::string sender;
        std::string receiver;
        std::map<std::string, Json::Value> sender_messages;
        while (res->next())
        {
            Json::Value item;
            sender = res->getString("sender");
            receiver = res->getString("receiver");
            if (sender == me || receiver == me)
            {
                std::string key = (sender == me) ? receiver : sender;
                Json::Value &messages = sender_messages[key];
                std::string content = res->getString("content");
                std::string time = res->getString("time");
                sender = res->getString("sender");
                item["content"] = content;
                item["time"] = time;
                item["sender"] = sender;
                item["receiver"] = receiver;
                // 构造item
                messages.append(item);
            }
        }
        for (auto &x : sender_messages)
            result[x.first] = x.second;

        delete res;
        delete prepStmt;
        delete con;
        return result;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
        return "error";
    }
}