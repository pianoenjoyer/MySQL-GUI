#pragma once
#include "afxdialogex.h"
#include <afx.h>
#include <fstream>
#include <iostream>
#include <string>

class CLoginDataSave {
private:
	CString m_strFileName;
public:
	CLoginDataSave(const CString& strFileName) : m_strFileName(strFileName) {};
	bool SaveLoginInfo(CString& usernameCtrl, CString& passwordCtrl);

};