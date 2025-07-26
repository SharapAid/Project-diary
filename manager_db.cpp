#include "manager_db.hpp"

DataBaseManager::DataBaseManager(const std::string& dbPath)
    : m_db(nullptr), m_dbPath(dbPath){
    int rc = sqlite3_open(m_dbPath.c_str(), &m_db);
    if(rc != SQLITE_OK){
        m_lastError = "Do not open BD: " + std::string(sqlite3_errmsg(m_db));
        if(m_db){
            sqlite3_close(m_db);
            m_db = nullptr;
        }
        std::cerr<<"Error in DatabaseManager constructor: "<<m_lastError<<std::endl;
    } 
    else{
        std::cout<<"Data base open: "<<m_dbPath<< std::endl;
    }
}

DataBaseManager::~DataBaseManager(){
    if(m_db){
        sqlite3_close(m_db);
        std::cout<<"Data base close: "<<m_dbPath<<std::endl;
    }
}

bool DataBaseManager::execute(const std::string& sqlQuery, const std::function<void(sqlite3_stmt*)>& bindCallback){
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, sqlQuery.c_str(), -1, &stmt, 0);

    if(rc != SQLITE_OK){
        m_lastError = "Failed to prepare statement '" + sqlQuery + "': " + std::string(sqlite3_errmsg(m_db));
        std::cerr<<"Error in DataBaseManager::execute prepare: "<<m_lastError<<std::endl;
        return false;
    }

    if(bindCallback){
        bindCallback(stmt);
    }

    rc = sqlite3_step(stmt);

    if(rc != SQLITE_DONE){
        m_lastError = "Request execution error '" + sqlQuery + "': " + std::string(sqlite3_errmsg(m_db));
        std::cerr<<"Error in DataBaseManager::execute step: "<<m_lastError<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

QueryResult DataBaseManager::query(const std::string& sqlQuery, const std::function<void(sqlite3_stmt*)>& bindCallback){
    QueryResult results;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, sqlQuery.c_str(), -1, &stmt, 0);

    if(rc != SQLITE_OK){
        m_lastError = "Failed to prepare statement '" + sqlQuery + "': " + std::string(sqlite3_errmsg(m_db));
        std::cerr<<"Error in DataBaseManager::query prepare: "<<m_lastError<<std::endl;
        return {};
    }

    if(bindCallback){ 
        bindCallback(stmt);
    }

    while((rc = sqlite3_step(stmt)) == SQLITE_ROW){
        RowData row;
        int colCount = sqlite3_column_count(stmt);
        for(int i = 0; i < colCount; i++){
            row.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, i)) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, i)) : "NULL");
        }
        results.push_back(row);
    }

    if(rc != SQLITE_DONE){
        m_lastError = "Request execution error '" + sqlQuery + "': " + std::string(sqlite3_errmsg(m_db));
        std::cerr<<"Error in DataBaseManager::query step: "<<m_lastError<<std::endl;
        sqlite3_finalize(stmt);
        return {};
    }

    sqlite3_finalize(stmt);
    return results;
}

int DataBaseManager::callback(void* data, int count, char** values, char** columnNames){
    QueryResult* results = static_cast<QueryResult*>(data);
    RowData row;
    for(int i = 0; i < count; i++){
        row.push_back(values[i] ? values[i] : "NULL");
    }
    results->push_back(row);
    return 0;
}

std::string DataBaseManager::getLastError() const{
    return m_lastError;
}

