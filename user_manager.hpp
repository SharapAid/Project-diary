#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "manager_db.hpp"

class ManagerUser{
private:
    DataBaseManager m_db_manager;
public:
    ManagerUser();

    bool verifyUserCredentials(const std::string& name, const std::string& password);

    bool addUser(const User& user);

    bool enterUser(const User& user);

    bool exitUser(const User& user);

    bool deleteUser(const User& user);

    std::string checkPassword(const std::string& password);

    bool changeUserPassword(const std::string& old_password, const std::string& new_password);

    bool changeUserName(const std::string& old_name, const std::string& new_name);

    bool addContentUser(const std::string& name, const std::string& text);

    std::vector<std::string> getUserContent(const std::string& name);
};