// mysql
#include "mysql.h"

#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/mysql_connection.h>

#include "jdbc/mysql_driver.h"
#include "json/json.h"

void sql_process_request(std::string sender, std::string receiver, std::string attitude)
{
    std::vector<std::string> usernamelist;
    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
    sql::Connection *con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");

    con->setSchema("flypen");

    std::string readData = "SELECT req FROM users WHERE username = ?";
    sql::PreparedStatement *readDatament = con->prepareStatement(readData);
    readDatament->setString(1, sender);

    sql::ResultSet *resultSet = readDatament->executeQuery();
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
        std::string readData = "SELECT friends FROM users WHERE username = ?";
        std::string* RS = new std::string[2]{receiver, sender};       // R, S

        for(int i = 0; i < 2; i++)
        {
            readDatament = con->prepareStatement(readData);
            readDatament->setString(1, RS[i]);
            resultSet = readDatament->executeQuery();

            std::string friendsList;
            if(resultSet->next())
                friendsList = resultSet->getString("friends");
            
            if(RS[i] == receiver)   // i == 0;
            {
                size_t pos = friendsList.find(sender);
                if(pos != std::string::npos)
                    return;
            }

            std::string updateQuery = "UPDATE users SET friends = ? WHERE username = ?";
            sql::PreparedStatement *update = con->prepareStatement(updateQuery);

            if(friendsList != "")
            {
                friendsList += "," + RS[1 - i];
                update->setString(1, friendsList);
                update->setString(2, RS[i]);
            }
            else
            {
                update->setString(1, RS[1 - i]);
                update->setString(2, RS[i]);
            }

            update->execute();
            delete update, update = nullptr;
        }
        delete RS, RS = nullptr;
    }

    delete updateStatement, updateStatement = nullptr;
    delete resultSet, resultSet = nullptr;
    delete readDatament, readDatament = nullptr;
    delete con, con = nullptr;
    delete driver, driver = nullptr;

    return;
}

void sql_addrequest(std::string sender, std::string receiver)
{
    std::vector<std::string> usernamelist;

    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
    sql::Connection *con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
    con->setSchema("flypen");

    sql::Statement *tool = con->createStatement();

    std::string readData = "SELECT req FROM users WHERE username = ?";

    sql::PreparedStatement *readDatament = con->prepareStatement(readData);
    readDatament->setString(1, receiver);

    sql::ResultSet *resultSet = readDatament->executeQuery();

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
        }
        else
        {
            updateStatement->setString(1, sender);
            updateStatement->setString(2, receiver);
        }
        updateStatement->execute();
        delete updateStatement, updateStatement = nullptr;
    }  

    delete resultSet, resultSet = nullptr;
    delete readDatament, readDatament = nullptr;
    delete tool, tool = nullptr;
    delete con, con = nullptr;
    delete driver, driver = nullptr;
}

void sql_addconnect(std::string connectptr)
{
    try
    {
        sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");

        std::string classMysql = "INSERT INTO users(username, password, time, connection) VALUES (?, ?, NOW(), ?)";
        sql::PreparedStatement *insertConnect = con->prepareStatement(classMysql);
        insertConnect->setString(4, connectptr);

        delete insertConnect, insertConnect = nullptr;
        delete con, con = nullptr;
        delete driver, driver = nullptr;
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
        sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");

        std::string classMysql = "INSERT INTO chat(content, isread, sender, receiver, time) VALUES (?, ?, ?, ?, NOW())";
        sql::PreparedStatement *insertData = con->prepareStatement(classMysql);

        insertData->setString(1, message);
        insertData->setString(2, isread);
        insertData->setString(3, sender);
        insertData->setString(4, receiver);
        insertData->executeUpdate();

        delete insertData, insertData = nullptr;
        delete con, con = nullptr;
        delete driver, driver = nullptr;
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
        sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");

        sql::Statement *tool = con->createStatement();
        std::string classMysql = "INSERT INTO users(username, password, avatar, createtime) VALUES (?, ?, ?, NOW())";
        sql::PreparedStatement *ptool = con->prepareStatement(classMysql);

        ptool->setString(1, username);
        ptool->setString(2, passwd);
        ptool->setInt(3, avatar);
        ptool->executeUpdate();

        delete ptool, ptool = nullptr;
        delete tool, tool = nullptr;
        delete con, con = nullptr;
        delete driver, driver = nullptr;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
}

Json::Value get_chat_info(std::string me, std::string who_send_me)
{
    Json::Value json;

    try
    { 
        sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");

        std::string* send = new std::string[2]{me, who_send_me};

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
                    std::string* who = new std::string[2]{"req", "friends"};

                    for(int i = 0; i < 2; i++)
                    {
                        who[i] = res->getString(who[i]);

                        Json::Value array(Json::arrayValue);
                        std::stringstream strStream(who[i]);
                        std::string token;

                        while(std::getline(strStream, token, ','))
                        {
                            Json::Value aVisit;
                            aVisit["username"] = token;

                            std::string sql = "SELECT * FROM users WHERE username = ? LIMIT 1";
                            prepStmt->setString(1, token);
                            sql::ResultSet *res = prepStmt->executeQuery();

                            if(res->next())
                                aVisit["avatar"] = res->getInt("avatar");    // 假设avatar统一为4
                            
                            array.append(aVisit);
                            user[who[i]] = array;
                        }
                    }
                    user["avatar"] = avatar;

                    Json::StreamWriterBuilder builder;
                    std::string userJson = Json::writeString(builder, user);
                    json[send[i]] = user;
                    
                    delete who, who = nullptr;
                }

                delete res, res = nullptr;
                delete prepStmt, prepStmt = nullptr;
            }
        }

        delete send, send = nullptr;
        delete con, con = nullptr;
        delete driver, driver = nullptr;
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
        sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");

        std::string sql = "SELECT * FROM users WHERE username = ? LIMIT 1";
        sql::PreparedStatement *prepStmt = con->prepareStatement(sql);
        prepStmt->setString(1, user);

        sql::ResultSet *res = prepStmt->executeQuery();

        // 获取查询结果
        // (¬A ∧B) ∨ (A ∧¬B)
        if (((passwd != "@DEFAULT@") && (res->next())) || ((passwd == "@DEFAULT@") && (!(res->next()))))
        {
            result = true;

            // 提取所有列的值
            std::string username = res->getString("username");
            std::string password = res->getString("password");
            int createtime = res->getInt("createtime");

            if ((passwd != password) && (passwd != "@DEFAULT@"))
                result = false;
        }

        delete res, res = nullptr;
        delete prepStmt, prepStmt = nullptr;
        delete con, con = nullptr;
        delete driver, driver = nullptr;
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
    Json::Value result;
    std::cout << "login user: " << me << std::endl;

    try
    {
        sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");

        //===================    find begin

        std::string sqlFind = "SELECT * FROM chat WHERE sender = ? OR receiver = ?";
        sql::PreparedStatement *prepStmt = con->prepareStatement(sqlFind);
        prepStmt->setString(1, me);
        prepStmt->setString(2, me);  
        sql::ResultSet *res = prepStmt->executeQuery();

        //===================    find end and  update begin

        std::string sql0To1 = "UPDATE chat SET isread = 1 WHERE id = ?";
        int id;

        //===================    update end

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

        delete res, res = nullptr;
        delete prepStmt, prepStmt = nullptr;
        delete con, con = nullptr;
        delete driver, driver = nullptr;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
        result = (Json::Value)"error";
    }

    return result;
}