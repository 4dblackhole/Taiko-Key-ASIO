#pragma once
#include <Windows.h>
#include <map>
#include <tchar.h>
#include <string>

#include "SingletonBase.h"

namespace std
{
	_EXPORT_STD using tstring = basic_string<TCHAR, char_traits<TCHAR>, allocator<TCHAR>>;
}

class StringToVirtualKeyCode : public SingletonBase<StringToVirtualKeyCode>
{
public:
	StringToVirtualKeyCode();
	~StringToVirtualKeyCode();
	byte Convert(const std::tstring& str);

private:
	typedef std::map<std::tstring, ::byte>				vKeyMap;
	typedef std::map<std::tstring, ::byte>::iterator	vKeyMapIter;
	vKeyMap vkMap;
};

#define STR_TO_VK_CONVERT(x) StringToVirtualKeyCode::GetInstance().Convert(x);