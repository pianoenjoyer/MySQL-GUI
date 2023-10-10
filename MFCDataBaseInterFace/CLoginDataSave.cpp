#include "pch.h"
#include "CLoginDataSave.h"


bool CLoginDataSave::SaveLoginInfo(CString& usernameCtrl, CString& passwordCtrl) 
{
    std::ifstream loginData;
    loginData.open("your_file.txt", std::ios::in | std::ios::out | std::ios::app);

    if (!loginData.is_open())
    {
        return 1;
    }

    // Read data from the file
    std::string line;
    while (std::getline(loginData, line))
    {
        AfxMessageBox(CString(line.c_str()));
    }

    // Close the file when done
    loginData.close();


}