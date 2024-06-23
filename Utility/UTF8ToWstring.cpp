#include "framework.h"
#include "UTF8ToWstring.h"

wstring UTF8ToWstring(const char* buf, int size)
{
    wstring uni;
    int nLen = MultiByteToWideChar(CP_UTF8, 0, buf, (int)size, NULL, NULL);
    uni.resize((size_t)nLen);
    MultiByteToWideChar(CP_UTF8, 0, buf, (int)size, &uni[0], nLen);

    return uni;
}
