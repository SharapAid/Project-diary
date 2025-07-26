#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <sqlite3.h>


using RowData = std::vector<std::string>;
using QueryResult = std::vector<RowData>;

class DataBaseManager{
private:
    sqlite3* m_db;
    std::string m_lastError;
    std::string m_dbPath;

    static int callback(void* data, int count, char** values, char** columnNames);

public:
    explicit DataBaseManager(const std::string& dbPath);

    ~DataBaseManager();

    bool execute(const std::string& sqlQuery, const std::function<void(sqlite3_stmt*)>& bindCallback = nullptr);

    QueryResult query(const std::string& sqlQuery, const std::function<void(sqlite3_stmt*)>& bindCallback = nullptr);

    std::string getLastError() const;
};