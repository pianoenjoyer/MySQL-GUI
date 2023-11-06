#pragma once
#include "CDBConnection.h"
#include <fstream>

inline sql::SQLString CStringToSQLString(const CString& cstr);
inline CString SQLStringToCString(const sql::SQLString& sqlStr);
CString BinaryDataToHexString(const CString& binaryData);