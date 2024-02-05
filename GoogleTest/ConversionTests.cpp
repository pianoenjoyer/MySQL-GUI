#include "pch.h"
#include "Convertions.h"


TEST(StringConversionTest, CStringToSQLString) {
    CString inputCString = _T("Hello, World!");
    sql::SQLString outputSQLString = CStringToSQLString(inputCString);

    EXPECT_EQ(outputSQLString, "Hello, World!");
}

TEST(StringConversionTest, SQLStringToCString) {
    sql::SQLString inputSQLString = "Hello, World!";
    CString outputCString = SQLStringToCString(inputSQLString);

    EXPECT_STREQ(outputCString, _T("Hello, World!"));
}

TEST(StringConversionTest, BinaryDataToHexString) {
    CString binaryData = _T("TestBinaryData");
    CString hexString = BinaryDataToHexString(binaryData);

    EXPECT_STREQ(hexString, _T("5465737442696E61727944617461"));  // Replace with the expected hex string
}