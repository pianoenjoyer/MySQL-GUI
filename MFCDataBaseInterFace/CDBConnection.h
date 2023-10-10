#pragma once
#include "pch.h"
#include "jdbc.h"
#include "vector"

class CDBConnection
{

public:
    CDBConnection();
    ~CDBConnection();
    sql::ResultSet* ExecuteQuery(const sql::SQLString& query);
    bool Connect(sql::ConnectOptionsMap& settings);
    bool Connect(const sql::SQLString& server, const sql::SQLString& 
        username, const sql::SQLString& password, const sql::SQLString& database);
    bool CDBConnection::ChangeCurrentDatabase(sql::SQLString& databaseName);
    sql::ResultSet* CDBConnection::ExecuteQuery(const sql::SQLString& query, CString& error);
    std::vector <sql::SQLString> CDBConnection::GetDatabases();
    bool CDBConnection::Connect(const sql::SQLString& server, const sql::SQLString& username,
        const sql::SQLString& password);
    void Disconnect();
    bool IsConnected() const;
    std::vector<sql::SQLString> CDBConnection::GetTables();
    CString GetResultString(sql::ResultSet* resultSet);
private:
    sql::mysql::MySQL_Driver* m_driver;
    sql::Connection* m_connection;
    bool m_isConnected;
};

