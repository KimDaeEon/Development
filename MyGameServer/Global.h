#pragma once
#include<WinSock2.h>
#include<Windows.h> // TODO: <- 여기에 winsock.h 이 포함되어 있다. 이로 인해 합수가 겹치는 것 막으려면 WinSock2.h 를 먼저 include 하라고 쓰여 있다. 맞는지 실험해보자.
#include<tchar.h>
#include<fstream>
#include<pchannel.h>
#include<assert.h>
#include<minidumpapiset.h>
#include<mswsock.h>
#include<vector>

enum IO_TYPE
{
	IO_ACCEPT,
	IO_READ,
	IO_WRITE
};

typedef struct _OVERLAPPED_EX
{
	OVERLAPPED	Overlapped;
	IO_TYPE		IoType;
	VOID* Object;
} OVERLAPPED_EX;

#define MAX_BUFFER_LENGTH	4096

#ifdef _SERVER_SIDE
#define MAX_QUEUE_LENGTH	50  // TODO: 서버가 더 부담이 커서 그런가? 왜 50개로 하셨을까..
#else
#define MAX_QUEUE_LENGTH	500
#endif


