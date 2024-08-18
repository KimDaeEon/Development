// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// 아래는 추후 너무 많아지면 네트워크용 매크로 따로 빼자.
#define USING_SHARED_PTR(name) using name##Ref = TSharedPtr<class name>;
USING_SHARED_PTR(Session);
USING_SHARED_PTR(PacketSession);
USING_SHARED_PTR(SendBuffer);


// 혹시나.. 패킷 사이즈 변경되면 FPacketHeader와 함께 수정
struct PacketHeader
{
	uint16 size;
	uint16 id; // 프로토콜ID (ex. 1=로그인, 2=이동요청)
};

#include "ServerPacketHandler.h"
#include "PacketSession.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"