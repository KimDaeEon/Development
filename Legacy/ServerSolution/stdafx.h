#pragma once

#define MAX_USER 100
#define DEFAULT_PORT 1820

#include "../MyGameServer/Global.h"

typedef struct _USER
{
	WCHAR szUserID[32];
	WCHAR szUserName[32];
	DWORD dwAge;
	BYTE cSex;
	WCHAR szAddress[32];
} USER;

typedef struct _COMPUTER
{
	WCHAR szComputerName[32];
	WCHAR szIPAddress[32];
	BYTE cCPUType;
	DWORD dwRam;
	DWORD dwHDD;
} COMPUTER;

typedef struct _PROGRAM
{
	WCHAR szProgramName[32];
	DWORD dwCost;
	WCHAR szComment[32];
} PROGRAM;

#include "../MyGameServer/CriticalSection.h"
#include "../MyGameServer/MultiThreadSync.h"
#include "../MyGameServer/MemoryPool.h"
#include "../MyGameServer/CircularQueue.h"
#include "../MyGameServer/NetworkSession.h"
#include "../MyGameServer/PacketSession.h"
#include "../MyGameServer/Iocp.h"
#include "../MyGameServer/Stream.h"
#include "../MyGameServer/MiniDump.h"
#include "../MyGameServer/IniFile.h"

#include "Protocol.h"
#include "PT_Structure.h"
#include "PT_ReadPacket.h"
#include "PT_WritePacket.h"

#include "ConnectedSession.h"
#include "ConnectedSessionManager.h"
#include "ServerIocp.h"
