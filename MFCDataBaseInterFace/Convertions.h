#pragma once
#include "pch.h"
#include "afxdialogex.h"
#include "jdbc.h"
#include <codecvt>
#include <fstream>
#include <sstream>
#include <iomanip>

inline sql::SQLString CStringToSQLString(const CString& cstr)
{
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, cstr, -1, NULL, 0, NULL, NULL);
    std::string utf8Str;
    utf8Str.resize(size_needed);
    WideCharToMultiByte(CP_UTF8, 0, cstr, -1, &utf8Str[0], size_needed, NULL, NULL);
    return sql::SQLString(utf8Str.c_str());
}


inline CString SQLStringToCString(const sql::SQLString& sqlStr)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, sqlStr.c_str(), -1, NULL, 0);
    CString utf16CString;
    utf16CString.GetBufferSetLength(size_needed);
    MultiByteToWideChar(CP_UTF8, 0, sqlStr.c_str(), -1, utf16CString.GetBuffer(), size_needed);
    utf16CString.ReleaseBuffer();
    return utf16CString;
}


inline CString BinaryDataToHexString(const CString& binaryData)
{
    CString hexString;
    const int length = binaryData.GetLength();

    for (int i = 0; i < length; i++) {
        char c = binaryData[i];
        CString hexByte;
        hexByte.Format(_T("%02X"), static_cast<unsigned char>(c));
        hexString += hexByte;
    }

    return hexString;
}
