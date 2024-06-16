#include "pch.h"
#include "CDBConnection.h"


CDBConnection::CDBConnection()
    : m_driver(nullptr), m_connection(nullptr), m_isConnected(false)
{
    m_driver = sql::mysql::get_mysql_driver_instance();
}

CDBConnection::~CDBConnection()
{
    Disconnect();
}

bool CDBConnection::Connect(sql::ConnectOptionsMap& settings)
{
    if (IsConnected())
    {
        return true;
    }
    try {
        m_connection = m_driver->connect(settings);
        delete m_connection;
        return true;
    }
    catch (sql::SQLException& e) {
        CStringW wideErrorMessage(e.what());
        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wideErrorMessage, 
            -1, nullptr, 0, nullptr, nullptr);
        std::string errorMessage(bufferSize, '\0');
        WideCharToMultiByte(CP_UTF8, 0, wideErrorMessage, -1, &errorMessage[0], 
            bufferSize, nullptr, nullptr);
        AfxMessageBox(CString(errorMessage.c_str()));
        return false;
    }
}

bool CDBConnection::Connect(const sql::SQLString& server, const sql::SQLString& username,
const sql::SQLString& password, const sql::SQLString& database)
{
    if (IsConnected()) return true;
    try
    {
        m_connection = m_driver->connect(server, username, password);
        m_connection->setSchema(database);
        m_isConnected = true;
        return true;
    }
    catch (sql::SQLException& e)
    {
        CStringW wideErrorMessage(e.what());
        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, 
            wideErrorMessage, -1, nullptr, 0, nullptr, nullptr);
        std::string errorMessage(bufferSize, '\0');
        WideCharToMultiByte(CP_UTF8, 0, wideErrorMessage, -1, 
            &errorMessage[0], bufferSize, nullptr, nullptr);
        AfxMessageBox(CString(errorMessage.c_str()));
        return false;
    }
}

bool CDBConnection::Connect(const sql::SQLString& server, const sql::SQLString& username,
    const sql::SQLString& password)
{
    if (IsConnected()) return true;
    try
    {
        m_connection = m_driver->connect(server, username, password);
        m_isConnected = true;
        return true;
    }
    catch (sql::SQLException& e)
    {
        CStringW wideErrorMessage(e.what());
        int bufferSize = WideCharToMultiByte(CP_UTF8, 0,
            wideErrorMessage, -1, nullptr, 0, nullptr, nullptr);
        std::string errorMessage(bufferSize, '\0');
        WideCharToMultiByte(CP_UTF8, 0, wideErrorMessage, -1,
            &errorMessage[0], bufferSize, nullptr, nullptr);
        AfxMessageBox(CString(errorMessage.c_str()));
        return false;
    }
}


void CDBConnection::Disconnect()
{
    if (IsConnected())
    {
        delete m_connection;
        m_connection = nullptr;
        m_isConnected = false;
    }
}

bool CDBConnection::IsConnected() const
{
    return m_connection;
}

sql::ResultSet* CDBConnection::ExecuteQuery(const sql::SQLString& query)
{
    sql::Statement* statement = nullptr;
    sql::ResultSet* resultSet = nullptr;

    try {
        statement = m_connection->createStatement();
        resultSet = statement->executeQuery(query);

        delete statement;

        return resultSet;
    }
    catch (sql::SQLException& e) {
        CStringW wideErrorMessage(e.what());
        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wideErrorMessage,
            -1, nullptr, 0, nullptr, nullptr);
        std::string errorMessage(bufferSize, '\0');
        WideCharToMultiByte(CP_UTF8, 0, wideErrorMessage, -1,
            &errorMessage[0], bufferSize, nullptr, nullptr);
        AfxMessageBox(CString(errorMessage.c_str()));

        if (resultSet) {
            delete resultSet;
        }

        if (statement) {
            delete statement;
        }

        return nullptr;
    }
}

sql::ResultSet* CDBConnection::ExecuteQuery(const sql::SQLString& query, CStringW& error)
{
    sql::Statement* statement = nullptr;
    sql::ResultSet* resultSet = nullptr;

    try {
        statement = m_connection->createStatement();
        resultSet = statement->executeQuery(query);

        delete statement;

        return resultSet;
    }
    catch (sql::SQLException& e) {
        error = CStringW(e.what());

        if (resultSet) {
            delete resultSet;
        }

        if (statement) {
            delete statement;
        }

        return nullptr;
    }
}

std::vector<sql::SQLString> CDBConnection::GetTables()
{
    std::vector<sql::SQLString> tables;

    try {
        sql::Statement* statement = m_connection->createStatement();
        sql::ResultSet* resultSet = statement->executeQuery("SHOW TABLES");

        while (resultSet->next())
        {
            sql::SQLString tableName = resultSet->getString(1).asStdString();
            tables.push_back(tableName);
        }

        delete resultSet;
        delete statement;
    }
    catch (const sql::SQLException& e) {
        CString errorMessage = CString(e.what());
        AfxMessageBox(errorMessage);
    }

    return tables;
}


std::vector<sql::SQLString> CDBConnection::GetTableColumns(const sql::SQLString& tableName)
{
    std::vector<sql::SQLString> columns;
    if (tableName == "")
    {
        return columns;
    }
    try {
        sql::Statement* statement = m_connection->createStatement();
        sql::ResultSet* resultSet = statement->executeQuery("DESCRIBE " + tableName);

        while (resultSet->next())
        {
            sql::SQLString columnName = resultSet->getString("Field").asStdString();
            columns.push_back(columnName);
        }

        delete resultSet;
        delete statement;
    }
    catch (const sql::SQLException& e) 
    {
        CString errorMessage = CString(e.what());
        AfxMessageBox(errorMessage);
    }
    catch (const std::exception& e) 
    {
        CString errorMessage = CString(e.what());
        AfxMessageBox(errorMessage);
    }

    return columns;
}




std::vector<sql::SQLString> CDBConnection::GetDatabases() 
{

    std::vector<sql::SQLString> databases;
    sql::Statement* statement = m_connection->createStatement();
    sql::ResultSet* resultSet = statement->executeQuery("SHOW DATABASES");

    while (resultSet->next())
    {
        sql::SQLString tableName = resultSet->getString(1).asStdString();
        databases.push_back(tableName);
    }


    delete resultSet;
    delete statement;

    return databases;
}



bool CDBConnection::ChangeCurrentDatabase(const sql::SQLString& databaseName)
{
    try
    {
        m_connection->setSchema(databaseName);
    }
    catch (sql::SQLException& e)
    {
        CStringW wideErrorMessage(e.what());
        AfxMessageBox(wideErrorMessage);
        return false;
    }

    return true;
}

bool CDBConnection::CheckConnection() 
{
    return (m_connection->isValid() && !m_connection->isClosed());
}

bool CDBConnection::ExecuteNonQuery(const sql::SQLString& query)
{
    sql::Statement* statement = nullptr;

    try {
        statement = m_connection->createStatement();
        statement->executeUpdate(query);

        delete statement;
        return true;
    }
    catch (const sql::SQLException& e) {
        CStringW wideErrorMessage(e.what());
        AfxMessageBox(wideErrorMessage);

        if (statement) {
            delete statement;
        }

        return false;
    }
}


