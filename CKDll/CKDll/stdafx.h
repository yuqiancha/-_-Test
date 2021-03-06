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


#include <String>

#include <vector>
#include <mutex>
#include "library/cpp/inc/CyAPI.h"
#include <queue>

#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/loggingmacros.h>

using namespace std;

using namespace log4cplus;
using namespace log4cplus::helpers;

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")
#pragma comment(lib,"setupapi.lib")