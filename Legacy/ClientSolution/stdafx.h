#pragma once

#define DEFAULT_PORT 1820

#include "../MyGameServer/Global.h"

#include "../MyGameServer/CriticalSection.h"
#include "../MyGameServer/MultiThreadSync.h"
#include "../MyGameServer/MemoryPool.h"
#include "../MyGameServer/CircularQueue.h"
#include "../MyGameServer/NetworkSession.h"
#include "../MyGameServer/PacketSession.h"
#include "../MyGameServer/EventSelect.h"
#include "../MyGameServer/Stream.h"
#include "../MyGameServer/MiniDump.h"
#include "../MyGameServer/IniFile.h"
#include "../MyGameServer/ClientSession.h"

#include "../ServerSolution/Protocol.h"
#include "../ServerSolution/PT_Structure.h"
#include "../ServerSolution/PT_ReadPacket.h"
#include "../ServerSolution/PT_WritePacket.h"