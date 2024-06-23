#include "framework.h"
#include "UTF8ToWstring.h"

wstring UTF8ToWstring(const string& buf)
{
    wstring uni;
    int nLen = MultiByteToWideChar(CP_UTF8, 0, buf.c_str(), (int)buf.length(), NULL, NULL);
    uni.resize((size_t)nLen);
    MultiByteToWideChar(CP_UTF8, 0, buf.c_str(), (int)buf.length(), &uni[0], nLen);

    return uni;
}

string WstringToUTF8(const wstring& buf)
{
    int nLen = WideCharToMultiByte(CP_UTF8, 0, buf.c_str(), (int)buf.length(), NULL, 0, NULL, NULL);
    string strUtf8;
    strUtf8.resize((size_t)nLen);
    WideCharToMultiByte(CP_UTF8, 0, buf.c_str(), (int)buf.length(), &strUtf8[0], nLen, NULL, NULL);

    return strUtf8;
}