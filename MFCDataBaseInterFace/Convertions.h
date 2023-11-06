#pragma once
#include "pch.h"
#include "jdbc.h"
#include <fstream>
#include <fstream>
#include <sstream>
#include <iomanip>

inline sql::SQLString CStringToSQLString(const CString& cstr);
inline CString SQLStringToCString(const sql::SQLString& sqlStr);
CString BinaryDataToHexString(const CString& binaryData);
std::string CStringToUTF8String(const CString& cstr);