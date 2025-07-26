#pragma once
#include <iostream>
#include <string>

class User{
private:
    std::string m_name;
    std::string m_password;

public:

    User();

    User(const std::string& name, const std::string& password);

    std::string getName()const{return m_name;}
    std::string getPassword()const{return m_password;}

    void setName(const std::string& name){m_name = name;}
    void setPassword(const std::string& password){m_password = password;}
};