#pragma once
#pragma warning(disable:4996)
#include<WinSock2.h>
#include<Windows.h> // TODO: <- 여기에 winsock.h 이 포함되어 있다. 이로 인해 합수가 겹치는 것 막으려면 WinSock2.h 를 먼저 include 하라고 쓰여 있다. 맞는지 실험해보자.
#include<tchar.h>
#include<fstream>
#include<pchannel.h>
#include<assert.h>
#include<minidumpapiset.h>
#include<mswsock.h>
#include<vector>