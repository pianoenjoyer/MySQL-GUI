#include "pch.h"
#include "CLoginDataSave.h"



CLoginDataSave::CLoginDataSave()
{
    // Constructor logic, if needed
}

CLoginDataSave::~CLoginDataSave()
{
    // Destructor logic, if needed
}

void CLoginDataSave::SaveData(const CString& serverIP, const CString& username, const CString& password, bool rememberMe)
{
    CRegKey key;
    if (ERROR_SUCCESS == key.Create(HKEY_CURRENT_USER, _T("Software\\MySQLGUI")))
    {
        key.SetStringValue(_T("ServerIP"), EncryptData(serverIP));
        key.SetStringValue(_T("Username"), EncryptData(username));
        key.SetStringValue(_T("Password"), EncryptData(password));

        key.SetDWORDValue(_T("RememberMe"), rememberMe ? 1 : 0);
    }
}

bool CLoginDataSave::LoadData(CString& serverIP, CString& username, CString& password, bool& rememberMe)
{
    CRegKey key;
    if (ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\MySQLGUI"), KEY_READ))
    {
        ULONG size;
        TCHAR buffer[512];

        size = sizeof(buffer);
        if (ERROR_SUCCESS == key.QueryStringValue(_T("ServerIP"), buffer, &size))
        {
            serverIP = DecryptData(buffer);
        }

        size = sizeof(buffer);
        if (ERROR_SUCCESS == key.QueryStringValue(_T("Username"), buffer, &size))
        {
            username = DecryptData(buffer);
        }

        size = sizeof(buffer);
        if (ERROR_SUCCESS == key.QueryStringValue(_T("Password"), buffer, &size))
        {
            password = DecryptData(buffer);
        }

        DWORD remMe;
        if (ERROR_SUCCESS == key.QueryDWORDValue(_T("RememberMe"), remMe))
        {
            rememberMe = remMe == 1;
        }

        return true;
    }

    return false;
}

bool CLoginDataSave::ShouldRemember()
{
    CRegKey key;
    DWORD remMe;

    if (ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\MySQLGUI"), KEY_READ))
    {
        if (ERROR_SUCCESS == key.QueryDWORDValue(_T("RememberMe"), remMe))
        {
            return remMe == 1;
        }
    }

    return false;
}

CString CLoginDataSave::EncryptData(const CString& data)
{
    DATA_BLOB DataIn;
    DATA_BLOB DataOut;

    DataIn.pbData = (BYTE*)(LPCTSTR)data;
    DataIn.cbData = (data.GetLength() + 1) * sizeof(TCHAR);

    if (!CryptProtectData(&DataIn, NULL, NULL, NULL, NULL, 0, &DataOut))
    {
        return _T("");  // Handle encryption error
    }

    CString encrypted;
    for (DWORD i = 0; i < DataOut.cbData; i++)
    {
        CString byteAsHex;
        byteAsHex.Format(_T("%02X"), DataOut.pbData[i]);
        encrypted += byteAsHex;
    }

    LocalFree(DataOut.pbData);
    return encrypted;
}

CString CLoginDataSave::DecryptData(const CString& encrypted)
{
    DATA_BLOB DataIn;
    DATA_BLOB DataOut;

    int len = encrypted.GetLength();

    DataIn.cbData = len / 2;
    DataIn.pbData = new BYTE[DataIn.cbData];

    for (int i = 0; i < len; i += 2)
    {
        CString byteAsHex = encrypted.Mid(i, 2);
        DataIn.pbData[i / 2] = (BYTE)wcstoul(byteAsHex, NULL, 16);
    }

    if (!CryptUnprotectData(&DataIn, NULL, NULL, NULL, NULL, 0, &DataOut))
    {
        delete[] DataIn.pbData;
        return _T("");  // Handle decryption error
    }

    CString data((TCHAR*)DataOut.pbData);

    delete[] DataIn.pbData;
    LocalFree(DataOut.pbData);

    return data;
}

void CLoginDataSave::ClearData()
{
    CRegKey key;
    if (ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\MySQLGUI"), KEY_WRITE))
    {
        key.DeleteValue(_T("ServerIP"));
        key.DeleteValue(_T("Username"));
        key.DeleteValue(_T("Password"));
        key.DeleteValue(_T("RememberMe"));
    }
}
