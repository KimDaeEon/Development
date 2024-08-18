// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// �Ʒ��� ���� �ʹ� �������� ��Ʈ��ũ�� ��ũ�� ���� ����.
#define USING_SHARED_PTR(name) using name##Ref = TSharedPtr<class name>;
USING_SHARED_PTR(Session);
USING_SHARED_PTR(PacketSession);
USING_SHARED_PTR(SendBuffer);


// Ȥ�ó�.. ��Ŷ ������ ����Ǹ� FPacketHeader�� �Բ� ����
struct PacketHeader
{
	uint16 size;
	uint16 id; // ��������ID (ex. 1=�α���, 2=�̵���û)
};

#include "ServerPacketHandler.h"
#include "PacketSession.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"