#pragma once

#include "pch.h"
#include "jdbc.h"
#include <vector>

class CDBConnection
{
public:
    CDBConnection();
    ~CDBConnection();

    // Connection Methods
    bool Connect(sql::ConnectOptionsMap& settings);
    bool Connect(const sql::SQLString& server, const sql::SQLString& username, const sql::SQLString& password);
    bool Connect(const sql::SQLString& server, const sql::SQLString& username, const sql::SQLString& password, const sql::SQLString& database);
    void Disconnect();
    bool ChangeCurrentDatabase(const sql::SQLString& databaseName);
    bool IsConnected() const;
    bool CheckConnection();

    // Database Operations
    sql::ResultSet* ExecuteQuery(const sql::SQLString& query);
    sql::ResultSet* ExecuteQuery(const sql::SQLString& query, CString& error);
    std::vector<sql::SQLString> GetDatabases();
    std::vector<sql::SQLString> GetTables();

    // Utility methods
    CString GetResultString(sql::ResultSet* resultSet);

private:
    sql::mysql::MySQL_Driver* m_driver;
    sql::Connection* m_connection;
    bool m_isConnected;
};
