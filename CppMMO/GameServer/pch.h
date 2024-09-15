#pragma once

// MFC 프로젝트가 아니면 아래의 #define을 통해 빌드시간을 단축할 수 있다고 한다.
#define WIN32_LEAN_AND_MEAN

// 아래처럼하면 링커에서 라이브러리를 추가 안해도 라이브러리가 추가된다.
// 경로 입력할 때에 \\ <- 이거 2개씩 하는거 잊지 말자.
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