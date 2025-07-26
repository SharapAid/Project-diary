#include "user.hpp"
#include "user_manager.hpp"
#include "user_profile.hpp"
#include "application.hpp"


void Application::enterChoise(int& choise){
    std::cout<<"Enter your choise: ";
    std::cin>>choise;
    while(std::cin.fail() || choise < 0 || choise > 2){
        std::cerr<<"The choice can be between 0 and 2 and cannot be a letter!\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout<<"Enter your choise: ";
        std::cin>>choise;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Application::enterChoiseUserMenu(int& choise){
    std::cout<<"Enter your choise: ";
    std::cin>>choise;
    while(std::cin.fail() || choise < 0 || choise > 5){
        std::cerr<<"The choice can be between 0 and 5 and cannot be a letter!\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout<<"Enter your choise: ";
        std::cin>>choise;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Application::enterNameUser(std::string& name){
    std::cout<<"Enter name*: ";
    std::getline(std::cin, name);
    while(name == ""){
        std::cerr<<"The name can not be empty!\n";
        std::cin.clear();
        std::cout<<"Enter name*: ";
        std::getline(std::cin, name);
    }
}

void Application::enterPassword(std::string& password){
    std::cout<<"Enter password*: ";
    std::getline(std::cin, password);
    while(password == "" || password.size() < 6){
        std::cerr<<"The password field must not be empty and must not be less than 6 characters.!\n";
        std::cin.clear();
        std::cout<<"Enter password*: ";
        std::getline(std::cin, password);
    }
}

void Application::enterDiarioContent(std::string& text){
    std::cout<<"Enter text of diary*: ";
    std::getline(std::cin, text);
}


void Application::stateRegistration(){
    std::cout<<"_______________REGISTRATION____________\n";
    std::string name;
    std::string password;
    enterNameUser(name);
    enterPassword(password);
    User new_user(name, password);
    bool is_create = m_user_manager.addUser(new_user);
    if(is_create){
        std::cout<<"Now you can log into your profile\n";
        setState(state::MAIN_MENU);
    }
    else{
        setState(state::REGISTER);
    }
}

void Application::stateUserProfile(){
    std::cout<<"_______________SING IN____________\n";
    std::string name;
    std::string password;
    enterNameUser(name);
    enterPassword(password);
    User current_user(name, password);
    if(m_user_manager.verifyUserCredentials(name, password)){
        bool is_enter = m_user_manager.enterUser(current_user);
        if(is_enter){
            std::cout<<"Login successful!\n";
            if(m_current_user_profile){
                delete m_current_user_profile;
                m_current_user_profile = nullptr;
            }
            m_current_user_profile = new UserProfile(current_user, this->m_user_manager); // Передаем ссылку
            setState(state::USER_PROFILE_MENU);
        }
        else {
            std::cout<<"Login failed. Could not set user online status.\n";
            setState(state::MAIN_MENU);
        }
    }else{
        std::cout<<"Login failed. Invalid username or password.\n";
        setState(state::MAIN_MENU);
    }
}

int Application::stateMenuUserProfile(){
    std::cout<<"________________MENU USER "<<m_current_user_profile->getNameUser()<<"____________________\n";
    std::cout<<"What you want to do?\n"
             <<"1 - Write part in diario,\n"
             <<"2 - Read all diario,\n"
             <<"3 - Change name,\n"
             <<"4 - Change password,\n"
             <<"5 - Delete user,\n"
             <<"0 - Exit in main menu.\n";
    int choise = -1;
    enterChoiseUserMenu(choise);
    return choise;
}

void Application::stateWriteDiario(){
    std::cout<<"________________WRITE DIARIO_______________\n";
    std::string text;
    enterDiarioContent(text);
    m_current_user_profile->writeDiario(text);
    setState(state::USER_PROFILE_MENU);
}

void Application::stateReadDiario(){
    std::cout<<"________________READ DIARIO_______________\n";
    m_current_user_profile->readDiario();
    setState(state::USER_PROFILE_MENU);
}

void Application::stateChangeName(){
    std::cout<<"_______________CHANGE NAME______________\n";
    std::string new_name;
    enterNameUser(new_name);
    bool is_change = m_current_user_profile->changeName(new_name);
    if(is_change){
        
        setState(state::USER_PROFILE_MENU);
    }
    else{
        setState(state::CHANGE_NAME);
    }
}

void Application::stateChangePassword(){
    std::cout<<"______________CHANGE PASSWORD_____________\n";
    std::string password;
    std::string new_password;
    std::cout<<"Enter old password\n";
    enterPassword(password);
    std::cout<<"Enter new password\n";
    enterPassword(new_password);
    bool is_change = m_current_user_profile->changePasswordUser(password, new_password);
    if(is_change){
        setState(state::USER_PROFILE_MENU);
    }
    else{
        setState(state::CHANGE_PASSWORD);
    }
}

void Application::stateDeleteUser(){
    std::cout<<"______________DELETE USER PROFILE_____________\n";
    std::string password;
    enterPassword(password);
    bool is_change = m_current_user_profile->deleteUserProfile(password);
    if(is_change){
        setState(state::MAIN_MENU);
    }
    else{
        setState(state::DELETE_USER);
    }
}

void Application::stateExitUser(){
    bool is_exit = m_current_user_profile->exitUserAtProfile();
    if(is_exit){
        std::cout<<"Logged out successfully.\n";
        delete m_current_user_profile;
        m_current_user_profile = nullptr;
        setState(state::MAIN_MENU);
    }
    else{
        std::cout << "Logout failed.\n";
        setState(state::USER_PROFILE_MENU);
    }
}

void Application::updataStateUserMenu(){
    int choise = stateMenuUserProfile();
    switch (choise){
        case 0:
            setState(state::EXIT_USER);
            break;
        case 1:
            setState(state::WRITE_DIARIO);
            break;
        case 2:
            setState(state::READ_DIARIO);
            break;
        case 3:
            setState(state::CHANGE_NAME);
            break;
        case 4:
            setState(state::CHANGE_PASSWORD);
            break;
        case 5:
            setState(state::DELETE_USER);
            break;
    }
}

int Application::stateMainMenu(){
    std::cout<<"_______________MENU_____________\n";
    std::cout<<"What you want to do?\n"
             <<"1 - Sing in,\n"
             <<"2 - Register,\n"
             <<"0 - Exit.\n";
    int choise = -1;
    enterChoise(choise);
    return choise;
}

void Application::updateState(){
    int choise = stateMainMenu();
    switch (choise){
        case 0:
            setState(state::EXIT);
            break;
        case 1:
            setState(state::LOGIN);
            break;
        case 2:
            setState(state::REGISTER);
            break;
    }
}

void Application::Run(){
    while(m_state != state::EXIT){
        switch (m_state){
            case state::MAIN_MENU:
                updateState();
                break;
            case state::LOGIN:
                stateUserProfile();
                break;
            case state::REGISTER:
                stateRegistration();
                break;
            case state::USER_PROFILE_MENU:
                updataStateUserMenu();
                break;
            case state::WRITE_DIARIO:
                stateWriteDiario();
                break;
            case state::READ_DIARIO:
                stateReadDiario();
                break;
            case state::CHANGE_NAME:
                stateChangeName();
                break;
            case state::CHANGE_PASSWORD:
                stateChangePassword();
                break;
             case state::DELETE_USER:
                stateDeleteUser();
                break;
            case state::EXIT_USER:
                stateExitUser();
                break;
            case state::EXIT:
                break;
        }
    }
}