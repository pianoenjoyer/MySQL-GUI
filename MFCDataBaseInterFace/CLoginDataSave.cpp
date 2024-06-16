#include "pch.h"
#include "CLoginDataSave.h"


CLoginDataSave::CLoginDataSave()
{
}

CLoginDataSave::~CLoginDataSave()
{
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
        TCHAR* buffer = nullptr;
        ULONG size = 0;
        if (ERROR_SUCCESS == key.QueryStringValue(_T("ServerIP"), nullptr, &size))
        {
            buffer = new TCHAR[size];
            if (ERROR_SUCCESS == key.QueryStringValue(_T("ServerIP"), buffer, &size))
            {
                serverIP = DecryptData(buffer);
            }
        }
        size = 0;
        if (ERROR_SUCCESS == key.QueryStringValue(_T("Username"), nullptr, &size))
        {
            buffer = new TCHAR[size];
            if (ERROR_SUCCESS == key.QueryStringValue(_T("Username"), buffer, &size))
            {
                username = DecryptData(buffer);
            }
        }
        size = 0;
        if (ERROR_SUCCESS == key.QueryStringValue(_T("Password"), nullptr, &size))
        {
            buffer = new TCHAR[size];
            if (ERROR_SUCCESS == key.QueryStringValue(_T("Password"), buffer, &size))
            {
                password = DecryptData(buffer);
            }
        }
        delete[] buffer;

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
        return _T("");
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

    if (len % 2 != 0)
    {
        return _T("");
    }

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
        return _T("");
    }

    CString data((TCHAR*)DataOut.pbData, DataOut.cbData / sizeof(TCHAR));

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
