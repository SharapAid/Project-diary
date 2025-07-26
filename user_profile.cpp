#include "user.hpp"
#include "user_manager.hpp"
#include "user_profile.hpp"

UserProfile::UserProfile(const User& currentUser, ManagerUser& userManager)
    : m_user(currentUser), m_userManager(userManager)
{

}

bool UserProfile::exitUserAtProfile(){
    return m_userManager.exitUser(m_user);
}

bool UserProfile::deleteUserProfile(const std::string& password){
    std::string name = m_userManager.checkPassword(password);
    if(name == m_user.getName()){
        bool is_delete = m_userManager.deleteUser(m_user);
        if(is_delete){
            std::cout << "User profile deleted successfully!\n";
        }
        else{
            std::cout<<"Error in delete user!\n";
            return false;
        }
    }
    else{
        std::cout<<"Invalid password!\n";
        return false;
    }
    return true;
}

bool UserProfile::changePasswordUser(const std::string& password, const std::string& new_password){
    std::string name = m_userManager.checkPassword(password);
    if(name == m_user.getName()){
        bool is_change = m_userManager.changeUserPassword(password, new_password);
        if(is_change){
            std::cout<<"Password is change!\n";
        }
        else{
            std::cout<<"Error in change password! Please try again.\n";
            return false;
        }
    }
    else{
        std::cout<<"Invalid password!\n";
        return false;
    }
    return true;
}

bool UserProfile::changeName(const std::string& new_name){
    std::string name = m_user.getName();
    bool is_change = m_userManager.changeUserName(name, new_name);
    if(is_change){
        std::cout<<"Name changed!\n";
        m_user.setName(new_name);
    }
    else{
        std::cout<<"This name already exists!\n";
        return false;
    }
    return true;
}

void UserProfile::writeDiario(const std::string& text){
    std::string name = m_user.getName();
    if(m_userManager.addContentUser(name, text)){
        std::cout<<"Entry added to diario!\n";
    } 
    else{
        std::cout<<"Failed to add entry to diario.\n";
    }
}

void UserProfile::readDiario(){
    std::vector<std::string> entries = m_userManager.getUserContent(m_user.getName());

    if(entries.empty()){
        std::cout<<"Your diario is empty."<<std::endl;
    } 
    else{
        for(size_t i = 0; i < entries.size(); ++i){
            std::cout<<(i + 1)<< ". "<< entries[i]<<std::endl;
        }
    }
}