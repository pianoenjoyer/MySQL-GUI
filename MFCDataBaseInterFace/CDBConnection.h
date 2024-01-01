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
    void CDBConnection::ChangeUserPassword(const std::string& user, const std::string& password);
    // Database Operations
    sql::ResultSet* ExecuteQuery(const sql::SQLString& query);
    sql::ResultSet* ExecuteQuery(const sql::SQLString& query, CString& error);
    std::vector<sql::SQLString> GetDatabases();
    std::vector<sql::SQLString> GetTables();
    std::vector<sql::SQLString> GetTableColumns(const sql::SQLString& tableName);
    bool CDBConnection::ExecuteNonQuery(const sql::SQLString& query);
    // Utility methods
    CString GetResultString(sql::ResultSet* resultSet);

private:
    sql::mysql::MySQL_Driver* m_driver;
    sql::Connection* m_connection;
    bool m_isConnected;
};
