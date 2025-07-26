#include "user.hpp"
#include "user_manager.hpp"
#include "user_profile.hpp"
#include "application.hpp"

User::User(): m_name(""), m_password("")
{

}

User::User(const std::string& name, const std::string& password)
    :m_name(name), m_password(password)
{

}