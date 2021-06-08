#pragma once
#include<Windows.h>
#include<tchar.h>
#include<fstream>
#include<pchannel.h>
#include<assert.h>
#include<minidumpapiset.h>
#include<WinSock2.h>

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


