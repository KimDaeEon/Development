#include "pch.h"

#include <iostream>
#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

void HandleError(const char* cause)
{
	int32 errCode = ::WSAGetLastError();
	cout << cause << "Socket ErrorCode : " << errCode << endl;
}

int main()
{
	WSAData wsaData;

	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return 0;
	}
	
	SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		HandleError("Socket");
		return 0;
	}

	u_long on = 1;
	if (::ioctlsocket(clientSocket, FIONBIO, &on) == INVALID_SOCKET)
	{
		return 0;
	}

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	//serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	serverAddr.sin_port = ::htons(7777);

	while (true)
	{
		if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		{
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
			{
				continue;
			}

			// 연결된 상황이니 다음으로 넘어간다.
			if (::WSAGetLastError() == WSAEISCONN)
			{
				break;
			}

			// 문제 상황
			break;
		}
	}

	cout << "Connected to Server" << endl;

	
	// Send
	char sendBuffer[100] = "Hello World";
	WSAEVENT wsaEvent = ::WSACreateEvent();
	OVERLAPPED overlapped = {};
	overlapped.hEvent = wsaEvent;

	while (true)
	{
		WSABUF wsaBuf;
		wsaBuf.buf = sendBuffer; // 절대 밖에서 건드리면 안된다. WSA함수만 건드려야 한다.
		wsaBuf.len = 100;

		DWORD sendLen = 0;
		DWORD flags = 0;

		if (::WSASend(clientSocket, &wsaBuf, 1, &sendLen, flags, &overlapped, nullptr) == SOCKET_ERROR)
		{
			if (::WSAGetLastError() == WSA_IO_PENDING)
			{
				::WSAWaitForMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, FALSE);
				WSAGetOverlappedResult(clientSocket, &overlapped, &sendLen, FALSE, &flags);
			}
			else
			{
				// 정말 문제 있는 상황
				cout << "Something is worng" << endl;
				break;
			}

			break;
		}

		cout << "Send Data! Len = " << sizeof(sendBuffer) << endl;

		this_thread::sleep_for(1s);
	}

	::closesocket(clientSocket);
	::WSACloseEvent(wsaEvent);
	::WSACleanup();

	return 0;
}

