#include "mysql.h"
#include "jdbc/cppconn/prepared_statement.h"
#include "jdbc/mysql_connection.h"
#include "jdbc/mysql_driver.h"
#include "json/json.h"

void sql_unlocked(const std::string& DeleteName)
{
    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
    sql::Connection *con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
    con->setSchema("flypen");
    sql::Statement *stmt = con->createStatement();
    try
    {
        std::string DeleteQuery = "DELETE FROM file WHERE filename = " + DeleteName;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQL Exception: in sql_unlocked() function" << e.what() << std::endl;
    }
    delete stmt;
    delete con;
}

// relate to chat

int sql_findexist(const std::string& receiver)
{
    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
    sql::Connection *con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
    con->setSchema("flypen");
    std::string readdata = "SELECT username FROM users";
    sql::PreparedStatement *readdatament = con->prepareStatement(readdata);
    sql::ResultSet *resultSet = readdatament->executeQuery();
    std::string usernamelist;
    while (resultSet->next())
    {
        if (receiver == resultSet->getString("username"))
            return 1;
    }
    return 0;
}
int lockcheck(const std::string& filename)
{
    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
    sql::Connection *con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
    con->setSchema("flypen");
    std::string readData = "SELECT filename FROM file";
    sql::PreparedStatement *readdatament = con->prepareStatement(readData);
    sql::ResultSet *resultSet = readdatament->executeQuery();

    std::string name;
    while (resultSet->next())
    {
        if (resultSet->getString("filename") == filename)
            return 1;
    }
    std::string changestate = "INSERT INTO file(filename) VALUES (?)";
    sql::PreparedStatement *changestatement = con->prepareStatement(changestate);
    changestatement->setString(1, filename);
    changestatement->executeUpdate();
    return 0;
}
void process(sql::PreparedStatement *readDatament, std::vector<std::string> s, sql::Connection *con)
{
    for (int i = 0; i < 2; i++)
    {
        readDatament->setString(1, s[i]);
        sql::ResultSet *resultSet = readDatament->executeQuery();
        std::string friendlist;
        if (resultSet->next())
        {
            friendlist = resultSet->getString("friends");
        }
        int pos = friendlist.find(s[i + 1]);
        if (pos != std::string::npos && pos != 0)
            friendlist.erase(pos - 1, s[i + 1].length() + 1);
        else if (pos == 0)
            friendlist.erase(pos, s[i + 1].length() + 1);
        std::string changedata = "UPDATE users SET friends=? WHERE username =?";
        sql::PreparedStatement *updateStatement = con->prepareStatement(changedata);
        updateStatement->setString(1, friendlist);
        updateStatement->setString(2, s[i]);
        updateStatement->execute();
    }
}

void sql_delete_operation(const std::string& sender, const std::string& receiver)
{
    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
    sql::Connection *con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");

    con->setSchema("flypen");
    std::vector<std::string> s;
    s.push_back(sender);
    s.push_back(receiver);
    std::string readdata = "SELECT friends FROM users WHERE username = ?";

    sql::PreparedStatement *readDatament = con->prepareStatement(readdata);
    process(readDatament, s, con);
}

void sql_process_request(const std::string& sender, const std::string& receiver, const std::string& attitude)
{
    std::vector<std::string> usernamelist;
    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
    sql::Connection *con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
    con->setSchema("flypen");

    std::string readData = "SELECT req FROM users WHERE username = ?";
    sql::PreparedStatement *readDatament = con->prepareStatement(readData);
    readDatament->setString(1, receiver);

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
        auto *RS = new std::string[2]{receiver, sender}; // R, S

        for (int i = 0; i < 2; i++)
        {
            readDatament = con->prepareStatement(readData);
            readDatament->setString(1, RS[i]);
            resultSet = readDatament->executeQuery();

            std::string friendsList;
            if (resultSet->next())
                friendsList = resultSet->getString("friends");

            if (RS[i] == receiver) // i == 0;
            {
                size_t pos = friendsList.find(sender);
                if (pos != std::string::npos)
                    return;
            }

            std::string updateQuery = "UPDATE users SET friends = ? WHERE username = ?";
            sql::PreparedStatement *update = con->prepareStatement(updateQuery);

            if (!friendsList.empty())
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
            delete update;
        }
        delete[] RS;
    }
    delete updateStatement;
    delete resultSet;
    delete readDatament;
    delete con;
}

void sql_addrequest(const std::string& sender, const std::string& receiver)
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

        if (!req.empty())
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
        delete updateStatement;
    }

    delete resultSet;
    delete readDatament;
    delete tool;
    delete con;
}

void sql_addhistory(const std::string& sender, const std::string& receiver, const std::string& message, const std::string& isread)
{
    try
    {
        sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");

        std::string classMysql = "INSERT INTO chat(content, sender_isread,receiver_isread, sender, receiver, time) VALUES (?, ?, ? , ?, ?, NOW())";
        sql::PreparedStatement *insertData = con->prepareStatement(classMysql);

        insertData->setString(1, message);
        insertData->setString(2, isread);
        insertData->setString(3, isread);
        insertData->setString(4, sender);
        insertData->setString(5, receiver);
        insertData->executeUpdate();

        delete insertData;
        delete con;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
}

// relate to user
void sql_add(const std::string& username, const std::string& passwd, int avatar)
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

        delete ptool;
        delete tool;
        delete con;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
}

Json::Value get_my_info(const std::string& me)
{
    Json::Value json;

    try
    {
        sql::mysql::MySQL_Driver *driver;
        driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con;
        con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");
        if (!me.empty())
        {
            std::string sql = "SELECT * FROM users WHERE username = ? LIMIT 1";
            sql::PreparedStatement *prepStmt = con->prepareStatement(sql);
            prepStmt->setString(1, me);

            sql::ResultSet *res = prepStmt->executeQuery();

            if (res->next())
            {
                Json::Value user;
                int avatar = res->getInt("avatar");
                std::string friends = res->getString("friends");
                std::string req = res->getString("req");

                // 使用lambda函数来查询用户信息
                auto fetchUserInfo = [&](const std::string &token) -> Json::Value
                {
                    Json::Value info;
                    info["username"] = token;
                    std::string sql = "SELECT * FROM users WHERE username = ? LIMIT 1";
                    sql::PreparedStatement *prepStmt = con->prepareStatement(sql);
                    prepStmt->setString(1, token);
                    sql::ResultSet *res = prepStmt->executeQuery();
                    if (res->next())
                    {
                        info["avatar"] = res->getInt("avatar");
                    }
                    return info;
                };

                Json::Value friends_array(Json::arrayValue);
                Json::Value req_array(Json::arrayValue);
                std::stringstream sf(friends);
                std::stringstream sr(req);
                std::string token;

                // 处理好友列表
                while (std::getline(sf, token, ','))
                {
                    Json::Value afriend = fetchUserInfo(token);
                    friends_array.append(afriend);
                }

                // 处理请求列表
                while (std::getline(sr, token, ','))
                {
                    Json::Value areq = fetchUserInfo(token);
                    req_array.append(areq);
                }

                user["avatar"] = avatar;
                user["friends"] = friends_array;
                user["req"] = req_array;

                Json::StreamWriterBuilder builder;
                std::string userJson = Json::writeString(builder, user);

                json[me] = user;
            }
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }

    return json;
}

bool sql_check(const std::string& user, const std::string& passwd)
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

            if ((passwd != password) && (passwd != "@DEFAULT@"))
                result = false;
        }

        delete res;
        delete prepStmt;
        delete con;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }

  //  std::cout << result << std::endl;
    return result;
}
Json::Value sql_find_my_msg(const std::string& me, const std::string& connect_type)

{
    // std::cout << "login user: " << me << std::endl;
    try
    {
        sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");
        sql::PreparedStatement *prepStmt;
        //===================    find begin
        // SELECT * FROM chat WHERE (sender = 'lglglglgy' OR receiver = 'lglglglgy') AND isread = 0
        std::string sqlFind_new_connect = "SELECT * FROM chat WHERE sender = ? OR receiver = ?";
        std::string sqlFind_isread_is_zero = "SELECT * FROM chat WHERE (sender = ? AND sender_isread = 0 ) OR ( receiver = ? AND receiver_isread = 0 ) ";
        if (connect_type == "all")
        {
            prepStmt = con->prepareStatement(sqlFind_new_connect);
        }
        else // if (connect_type == "new")
        {
            prepStmt = con->prepareStatement(sqlFind_isread_is_zero);
        }
        // else
        // {
        //     std::cout << "connect_type error" << std::endl;
        //     return;
        // }
        // sql::PreparedStatement *prepStmt = con->prepareStatement(sqlFind);
        prepStmt->setString(1, me);
        prepStmt->setString(2, me);
        sql::ResultSet *res = prepStmt->executeQuery();

        //===================    find end and  update begin
        std::string sql0To1_sender = "UPDATE chat SET sender_isread = 1 WHERE id = ?";
        std::string sql0To1_rec = "UPDATE chat SET receiver_isread = 1 WHERE id = ?";
        

        //===================    update end
        Json::Value result;
        std::string sender;
        std::string receiver;
        std::map<std::string, Json::Value> sender_messages;

        while (res->next())
        {
            // update isread to 1
            if (connect_type == "new")
            {
                int id = res->getInt("id");
                if (res->getString("sender")==me)
                {
                    sql::PreparedStatement *updateStmt = con->prepareStatement(sql0To1_sender);
                    updateStmt->setInt(1, id);
                    updateStmt->executeUpdate();
                    delete updateStmt;
                }
                if(res->getString("receiver")==me)
                {
                    sql::PreparedStatement *updateStmt = con->prepareStatement(sql0To1_rec);
                    updateStmt->setInt(1, id);
                    updateStmt->executeUpdate();
                    delete updateStmt;
                }    
                // std::cout <<"change"<<std::endl;
            }

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

void set_avatar(const std::string& person, int avatar)
{
    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
    sql::Connection *con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
    con->setSchema("flypen");
    std::string update_sql = "UPDATE users SET avatar = ? WHERE username = ?";
    sql::PreparedStatement *updateStatement = con->prepareStatement(update_sql);
    updateStatement->setInt(1, avatar);
    updateStatement->setString(2, person);
    updateStatement->execute();
}