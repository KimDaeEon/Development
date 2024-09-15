#pragma once

// MFC ������Ʈ�� �ƴϸ� �Ʒ��� #define�� ���� ����ð��� ������ �� �ִٰ� �Ѵ�.
#define WIN32_LEAN_AND_MEAN

// �Ʒ�ó���ϸ� ��Ŀ���� ���̺귯���� �߰� ���ص� ���̺귯���� �߰��ȴ�.
// ��� �Է��� ���� \\ <- �̰� 2���� �ϴ°� ���� ����.
#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif

#include "CorePch.h"
#include "Protocol.pb.h"

MAKE_SHARED_PTR_TYPE(ClientSession);
MAKE_SHARED_PTR_TYPE(Player);
MAKE_SHARED_PTR_TYPE(Room);