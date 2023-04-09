#pragma once
// PCH = PreCompiled Header
#include "Types.h"
#include "CoreGlobal.h"
#include "CoreMacro.h"
#include "CoreTLS.h"
#include "Container.h" // vector, map 등이 여기에 들어 있다.

#include <Windows.h>
#include <iostream>
#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

#include "Lock.h"
#include "ObjectPool.h"
#include "TypeCast.h"
#include "Memory.h"
#include "SendBuffer.h"
#include "Session.h"
#include "LockQueue.h"
