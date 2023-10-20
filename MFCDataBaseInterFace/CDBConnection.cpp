#include "pch.h"
#include "CDBConnection.h"
#include <codecvt>

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
    if (IsConnected()) return true;
    try {
        m_connection = m_driver->connect(settings);
        delete m_connection;
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

        return nullptr;  // Return nullptr indicating failure
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
        /*CStringW wideErrorMessage(e.what());
        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wideErrorMessage,
            -1, nullptr, 0, nullptr, nullptr);
        std::string errorMessage(bufferSize, '\0');
        WideCharToMultiByte(CP_UTF8, 0, wideErrorMessage, -1,
            &errorMessage[0], bufferSize, nullptr, nullptr);*/
        error = CStringW(e.what());

        if (resultSet) {
            delete resultSet;
        }

        if (statement) {
            delete statement;
        }

        return nullptr;  // Return nullptr indicating failure
    }
}
// No longer in use
CString CDBConnection::GetResultString(sql::ResultSet* resultSet)
{
    CString result;
    if (resultSet)
    {
        int columnCount = resultSet->getMetaData()->getColumnCount();
        const int columnWidth = 30; // Adjust the column width as needed

        // Retrieve column names
        for (int i = 1; i <= columnCount; i++)
        {
            std::string columnName = resultSet->getMetaData()->getColumnName(i);
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            std::wstring utf16ColumnName = converter.from_bytes(columnName);
            result += utf16ColumnName.c_str();

            // Pad the column name to match the column width
            int padding = columnWidth - utf16ColumnName.length();
            result += CString(L' ', padding + 1); // Add extra space after each column name
        }
        result += L"\r\n";

        // Add a line between column names and data
        for (int i = 1; i <= columnCount; i++)
        {
            result += L"-----------------------"; // Adjust the line length as needed
        }
        result += L"\r\n";

        while (resultSet->next())
        {
            for (int i = 1; i <= columnCount; i++)
            {
                std::string utf8String = resultSet->getString(i);
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                std::wstring utf16String = converter.from_bytes(utf8String);
                result += utf16String.c_str();

                // Pad the column value to match the column width
                int padding = columnWidth - utf16String.length();
                result += CString(L' ', padding + 1); // Add extra space after each column value
            }
            result += L"\r\n";
        }
    }

    /*if (resultSet)
    {
        delete resultSet;
    }*/

    return result;
}

std::vector<sql::SQLString> CDBConnection::GetTables()
{
    std::vector<sql::SQLString> tables;
    sql::Statement* statement = m_connection->createStatement();
    sql::ResultSet* resultSet = statement->executeQuery("SHOW TABLES");

    while (resultSet->next())
    {
        sql::SQLString tableName = resultSet->getString(1).asStdString();
        tables.push_back(tableName);
    }

    delete resultSet;
    delete statement;

    return tables;
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
