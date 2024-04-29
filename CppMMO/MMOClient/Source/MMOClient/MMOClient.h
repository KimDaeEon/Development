// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// �Ʒ��� ���� �ʹ� �������� ��Ʈ��ũ�� ��ũ�� ���� ����.
#define USING_SHARED_PTR(name) using name##Ref = TSharedPtr<class name>;
USING_SHARED_PTR(Session);
USING_SHARED_PTR(PacketSession);
USING_SHARED_PTR(SendBuffer);