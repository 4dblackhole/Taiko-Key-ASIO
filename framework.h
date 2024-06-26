﻿// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
#include <CommCtrl.h>
// C 런타임 헤더 파일입니다.
#include <atlstr.h>
#include <codecvt>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <malloc.h>
#include <iostream>
#include <sstream>
#include <map>
#include <memory.h>
#include <tchar.h>
#include <thread>
#include <queue>

#include <fmod.hpp>

using namespace std;

#include "Trace/Trace.h"
#include "Utility/StringToVirtualKeyCode.h"
#include "Utility/UTF8ToWstring.h"

namespace std
{
	_EXPORT_STD using tstring = basic_string<TCHAR, char_traits<TCHAR>, allocator<TCHAR>>;
	_EXPORT_STD using tifstream = basic_ifstream<TCHAR, char_traits<TCHAR>>;
}