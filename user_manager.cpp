#include "user.hpp"
#include "manager_db.hpp"
#include "user_profile.hpp"
#include "application.hpp"
#include "user_manager.hpp"

ManagerUser::ManagerUser()
    : m_db_manager("users.db")
{
    std::string sqlCreateTable = R"(
        CREATE TABLE IF NOT EXISTS USERS(
            NAME TEXT NOT NULL UNIQUE,
            PASSWORD TEXT NOT NULL,
            ONLINE INTEGER NOT NULL DEFAULT 0
        );
    )";

    if(!m_db_manager.execute(sqlCreateTable)) {
        std::cerr<<"Error creating USERS table: "<<m_db_manager.getLastError()<<std::endl;
    }

    std::string sqlCreateTableContents = R"(
        CREATE TABLE IF NOT EXISTS CONTENTS(
            ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
            USERNAME TEXT NOT NULL,
            NoteText TEXT,
            Timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (USERNAME) REFERENCES USERS(NAME)
                ON DELETE CASCADE
        );
    )";

    if(!m_db_manager.execute(sqlCreateTableContents)) {
        std::cerr<<"Error creating CONTENTS table: "<<m_db_manager.getLastError()<<std::endl;
    }
}

bool ManagerUser::addUser(const User& user){
    std::string sqlInsertUser = "INSERT INTO USERS(NAME, PASSWORD, ONLINE) VALUES(?, ?, 0)";

    if(!m_db_manager.execute(sqlInsertUser, [&](sqlite3_stmt* stmt){
        sqlite3_bind_text(stmt, 1, user.getName().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, user.getPassword().c_str(), -1, SQLITE_TRANSIENT);
    })) {
        std::cerr<<"Error insertion user in table: "<<m_db_manager.getLastError()<<std::endl;
        return false;
    }
    return true;
}

bool ManagerUser::verifyUserCredentials(const std::string& name, const std::string& password){
    std::string sqlQuery = "SELECT COUNT(*) FROM USERS WHERE NAME = ? AND PASSWORD = ?";
    QueryResult results = m_db_manager.query(sqlQuery, [&](sqlite3_stmt* stmt){
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);
    });

    if(!m_db_manager.getLastError().empty()){
        std::cerr<<"Error verifying credentials: "<< m_db_manager.getLastError()<<std::endl;
        return false;
    }

    if(!results.empty() && !results[0].empty()){
        try{
            int count = std::stoi(results[0][0]);
            return count > 0;
        } 
        catch(const std::exception& e){
            std::cerr<<"Error converting count to int: "<< e.what()<< std::endl;
            return false;
        }
    }
    return false;
}

bool ManagerUser::enterUser(const User& user){
    std::string sqlUpdateOnline = "UPDATE USERS SET ONLINE = 1 WHERE NAME = ? AND PASSWORD = ?";

    if(!m_db_manager.execute(sqlUpdateOnline, [&](sqlite3_stmt* stmt){
        sqlite3_bind_text(stmt, 1, user.getName().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, user.getPassword().c_str(), -1, SQLITE_TRANSIENT);
    })) {
        std::cerr<<"Data update error: "<<m_db_manager.getLastError()<<std::endl;
        return false;
    }
    return true;
}

bool ManagerUser::exitUser(const User& user){
    std::string sqlUpdateOnline = "UPDATE USERS SET ONLINE = 0 WHERE NAME = ? AND PASSWORD = ?";

    if(!m_db_manager.execute(sqlUpdateOnline, [&](sqlite3_stmt* stmt){
        sqlite3_bind_text(stmt, 1, user.getName().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, user.getPassword().c_str(), -1, SQLITE_TRANSIENT);
    })) {
        std::cerr<<"Data update error: "<<m_db_manager.getLastError()<< std::endl;
        return false;
    }
    return true;
}

bool ManagerUser::deleteUser(const User& user){
    std::string sqlDeleteUser = "DELETE FROM USERS WHERE NAME = ? AND PASSWORD = ?";

    if(!m_db_manager.execute(sqlDeleteUser, [&](sqlite3_stmt* stmt){
        sqlite3_bind_text(stmt, 1, user.getName().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, user.getPassword().c_str(), -1, SQLITE_TRANSIENT);
    })){
        std::cerr<<"Error in delete data: "<<m_db_manager.getLastError()<< std::endl;
        return false;
    }
    return true;
}

std::string ManagerUser::checkPassword(const std::string& password){
    std::string sqlFindName = "SELECT NAME FROM USERS WHERE PASSWORD = ?";

    QueryResult results = m_db_manager.query(sqlFindName, [&](sqlite3_stmt* stmt){
        sqlite3_bind_text(stmt, 1, password.c_str(), -1, SQLITE_TRANSIENT);
    });

    if(results.empty() && !m_db_manager.getLastError().empty()){
        std::cerr<< "Data sampling error: "<<m_db_manager.getLastError()<< std::endl;
        return "";
    }
    if(results.empty()){
        std::cout<<"Invalid password."<<std::endl;
        return "";
    }
    const auto& row = results[0];
    std::string name = row[0];
    return name;
}

bool ManagerUser::changeUserPassword(const std::string& old_password,const std::string& new_password){
    std::string sqlUpdatePassword = "UPDATE USERS SET PASSWORD = ? WHERE PASSWORD = ?";

    if(!m_db_manager.execute(sqlUpdatePassword, [&](sqlite3_stmt* stmt){
        sqlite3_bind_text(stmt, 1, new_password.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, old_password.c_str(), -1, SQLITE_TRANSIENT);
    })) {
        std::cerr<<"Data update error: "<<m_db_manager.getLastError()<<std::endl;
        return false;
    }
    return true;
}

bool ManagerUser::changeUserName(const std::string& old_name,const std::string& new_name){
    std::string sqlUpdateName = "UPDATE USERS SET NAME = ? WHERE NAME = ?";

    if(!m_db_manager.execute(sqlUpdateName, [&](sqlite3_stmt* stmt){
        sqlite3_bind_text(stmt, 1, new_name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, old_name.c_str(), -1, SQLITE_TRANSIENT);
    })){
        std::cerr<<"Data update error: "<<m_db_manager.getLastError()<<std::endl;
        return false;
    }
    return true;
}

bool ManagerUser::addContentUser(const std::string& name, const std::string& text){
    std::string sqlInsertContent = "INSERT INTO CONTENTS(USERNAME, NoteText) VALUES(?, ?)";

    if(!m_db_manager.execute(sqlInsertContent, [&](sqlite3_stmt* stmt){
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, text.c_str(), -1, SQLITE_TRANSIENT);
    })) {
        std::cerr<<"Error adding content for user "<<name<<": "<<m_db_manager.getLastError()<<std::endl;
        return false;
    }
    return true;
}

std::vector<std::string> ManagerUser::getUserContent(const std::string& name){
    std::vector<std::string> userNotes;
    std::string sqlQueryContent = "SELECT NoteText FROM CONTENTS WHERE USERNAME = ? ORDER BY Timestamp ASC";

    QueryResult results = m_db_manager.query(sqlQueryContent, [&](sqlite3_stmt* stmt){
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    });

    if(!m_db_manager.getLastError().empty()){
        std::cerr<<"Error getting content for user "<<name<<": "<<m_db_manager.getLastError()<<std::endl;
        return {};
    }

    for(const auto& row : results){
        if(!row.empty()){
            userNotes.push_back(row[0]);
        }
    }
    return userNotes;
}