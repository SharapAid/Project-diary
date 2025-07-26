#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <limits>
#include "user.hpp"
#include "user_profile.hpp"
#include "user_manager.hpp"

enum class state {
    MAIN_MENU,
    LOGIN,
    REGISTER,
    USER_PROFILE_MENU,
    WRITE_DIARIO,
    READ_DIARIO,
    CHANGE_NAME,
    CHANGE_PASSWORD,
    DELETE_USER,
    EXIT_USER,
    EXIT
};

class Application{
private:
    state m_state;
    ManagerUser m_user_manager;
    UserProfile* m_current_user_profile;

    void enterChoise(int& choise);
    void enterChoiseUserMenu(int& choise);
    void enterNameUser(std::string& name);
    void enterPassword(std::string& password);
    void enterDiarioContent(std::string& text);
    
    void stateWriteDiario();
    void stateReadDiario();
    void stateChangePassword();
    void stateChangeName();
    void stateDeleteUser();
    void stateExitUser();

    void stateRegistration();
    void stateUserProfile();
    

    int stateMainMenu();
    void updateState();

    void updataStateUserMenu();
    int stateMenuUserProfile();
public:

    Application():m_state(state::MAIN_MENU),m_user_manager(), m_current_user_profile(nullptr){
        std::cout<<"Welcome to the Diary App!\n";
    }

    ~Application(){
        if(m_current_user_profile){
            delete m_current_user_profile;
            m_current_user_profile = nullptr;
        }
    }

    state getState()const{return m_state;}
    void setState(const state& state){m_state = state;}

    void Run();
};