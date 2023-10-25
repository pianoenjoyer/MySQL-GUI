#pragma once
#pragma comment(lib, "Crypt32.lib")
#include "afxdialogex.h"
#include <afx.h>
#include <fstream>
#include <iostream>
#include <string>
#include <Windows.h>
#include <Wincrypt.h>

class CLoginDataSave
{
public:
    CLoginDataSave();
    ~CLoginDataSave();

    void SaveData(const CString& serverIP, const CString& username, const CString& password, bool rememberMe);
    bool LoadData(CString& serverIP, CString& username, CString& password, bool& rememberMe);
    bool ShouldRemember();
    void ClearData();
private:
    CString EncryptData(const CString& data);
    CString DecryptData(const CString& encrypted);
};
