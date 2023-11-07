#pragma once
#include "afxdialogex.h"
#define RED RGB(255, 0, 0)
#define GREEN RGB(0, 128, 0)
#define BLACK RGB(0, 0, 0)

//CONSOLE MESSEGES
//Query msgs
const CString MSG_QUERY_OK("Query completed");
//Export msgs
const CString MSG_EXPORT_START("Export started");
const CString MSG_EXPORT_OK("Export completed");
const CString MSG_EXPORT_ERR("Export error");
const CString MSG_EXPORT_CANCEL("Export canceled");
//DB change msgs
// 
// CMainDlg dialog
const CString MSG_DBCHANGE_OK("Databased selected");
const CString MSG_DBCHANGE_ERR("Databased select error");