// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <Windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO:  在此处引用程序需要的其他头文件
#include <gl/GL.h>
#include <gl/GLU.h>
#include <windowsx.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <atlimage.h>
#include <objbase.h>
#include <algorithm>
#include <array>
#include <vector>
#include <mutex>
#include <thread>

#define PI (3.141592653589793)
#define MAX_LOADSTRING 100
#define MAX_NAMESTRING 15
