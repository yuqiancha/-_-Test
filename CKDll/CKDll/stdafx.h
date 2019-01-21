// stdafx.h: 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 项目特定的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>

// 在此处引用程序需要的其他标头
#include <iostream>
#include <SetupAPI.h>

#include <list>

#include <fstream>
using namespace std;

#include <String>

#include <vector>
#include <mutex>
#include "library/cpp/inc/CyAPI.h"
#include <queue>

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")
#pragma comment(lib,"setupapi.lib")


