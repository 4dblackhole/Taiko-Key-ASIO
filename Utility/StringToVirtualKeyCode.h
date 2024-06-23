#pragma once
#include "SingletonBase.h"

class StringToVirtualKeyCode : public SingletonBase<StringToVirtualKeyCode>
{
public:
	StringToVirtualKeyCode();
	~StringToVirtualKeyCode();
	byte Convert(const TCHAR* str);

private:
	typedef std::map <const TCHAR*, ::byte>				vKeyMap;
	typedef std::map <const TCHAR*, ::byte>::iterator	vKeyMapIter;
	vKeyMap vkMap;
};

#define STR_TO_VK_CONVERT(x) StringToVirtualKeyCode::GetInstance().Convert(x);