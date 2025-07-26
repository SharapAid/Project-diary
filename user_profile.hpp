#pragma once
#include <iostream>
#include <string>
#include "user_manager.hpp"

class UserProfile{
private:
    User m_user;
    ManagerUser& m_userManager;
public:

    UserProfile(const User& currentUser, ManagerUser& userManager);

    std::string getNameUser()const{return m_user.getName();}

    bool deleteUserProfile(const std::string& password);

    bool changePasswordUser(const std::string& password, const std::string& new_password);

    bool changeName(const std::string& new_name);

    void writeDiario(const std::string& text);

    void readDiario();
    
    bool exitUserAtProfile();
};