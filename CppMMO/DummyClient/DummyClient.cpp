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

#pragma region UDP_TEST
//{
	//	WSAData wsaData;

	//	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	//	{
	//		return 0;
	//	}

	//	SOCKET clientSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
	//	if (clientSocket == INVALID_SOCKET)
	//	{
	//		HandleError("Socket");
	//		return 0;
	//	}

	//	SOCKADDR_IN serverAddr;
	//	::memset(&serverAddr, 0, sizeof(serverAddr));
	//	serverAddr.sin_family = AF_INET;
	//	//serverAddr.sin_addr.s_addr = ::inet_addr("127.0.0.1"); 
	//	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	//	serverAddr.sin_port = ::htons(7777);

	//	::connect(clientSocket, (PSOCKADDR)&serverAddr, sizeof(serverAddr));

	//	while (true)
	//	{
	//		char sendBuffer[1000] = "Hello Server!";

	//		int32 resultCode = ::send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);

//		if (resultCode == SOCKET_ERROR)
//		{
//			HandleError("SendTo");
//			return 0;
//		}

//		cout << "[Send] Data len : " << sizeof(sendBuffer) << " Data : " << sendBuffer << endl;

//		SOCKADDR_IN senderAddr;
//		::memset(&senderAddr, 0, sizeof(senderAddr));
//		int32 addrLen = sizeof(senderAddr);

//		char recvBuffer[1000] = "";

//		int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);

//		if (recvLen <= 0)
//		{
//			int32 errCode = ::WSAGetLastError();
//			cout << "Recv ErrorCode : " << errCode << endl;
//			return 0;
//		}

//		cout << "[Recv] Data len : " << recvLen << " Data : " << recvBuffer << endl;

//	}

//	::WSACleanup();
//}
#pragma endregion

#pragma region TCP_TEST
//{
	//	WSAData wsaData;

	//	// 윈속 DLL 초기화
	//	// 관련 정보가 wsaData에 채워진다.
	//	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	//	{
	//		cout << "WSAStartup Error!" << endl;
	//		return 0;
	//	}

	//	SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	//	if (clientSocket == INVALID_SOCKET)
	//	{
	//		int32 errCode = ::WSAGetLastError();
	//		cout << "Socket ErrorCode : " << errCode << endl;
	//		return 0;
	//	}

	//	SOCKADDR_IN serverAddr; // IPv4 주소
	//	::memset(&serverAddr, 0, sizeof(serverAddr));
	//	serverAddr.sin_family = AF_INET;
	//	//serverAddr.sin_addr.s_addr = ::inet_addr("127.0.0.1"); // deprecated 경고 뜬다.
	//	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr); // 경고 안뜨는 버전
	//	serverAddr.sin_port = ::htons(7777); // Host to Netork Short, Little-Endian -> Big-Endian 으로 변환

	//	if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	//	{
	//		int32 errCode = ::WSAGetLastError();
	//		cout << "Connect ErrorCode : " << errCode << endl;
	//		return 0;
	//	}

	//	cout << "Connected To Server!" << endl;

	//	while (true)
	//	{
	//		char buf[] = "Hi Server ~~~~~~~~~~~!!!!!!!!!!!!!";
	//		int32 resultCode = ::send(clientSocket, buf, strlen(buf), 0);
	//		if (resultCode == SOCKET_ERROR)
	//		{
	//			int32 errCode = ::WSAGetLastError();
	//			cout << "Send ErrorCode : " << errCode << endl;
	//			return 0;
	//		}
	//		cout << "[Send] " << buf << endl;

	//		//char recvBuffer[1000] = "";

	//		//resultCode = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
	//		//if (resultCode == SOCKET_ERROR)
	//		//{
	//		//	int32 errCode = ::WSAGetLastError();
	//		//	cout << "Recv ErrorCode : " << errCode << endl;
	//		//	return 0;
	//		//}

	//		//cout << "[Recv] " << recvBuffer << endl;

	//		this_thread::sleep_for(1ms);
	//	}

	//	::closesocket(clientSocket);
	//	::WSACleanup();
	//}
#pragma endregion

#pragma region Select_WSAEventSelectTest
//{
//WSAData wsaData;
//
//if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//{
//	return 0;
//}
//
//SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
//if (clientSocket == INVALID_SOCKET)
//{
//	HandleError("Socket");
//	return 0;
//}
//
//u_long on = 1;
//if (::ioctlsocket(clientSocket, FIONBIO, &on) == INVALID_SOCKET)
//{
//	return 0;
//}
//
//SOCKADDR_IN serverAddr;
//::memset(&serverAddr, 0, sizeof(serverAddr));
//serverAddr.sin_family = AF_INET;
//::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
////serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
//serverAddr.sin_port = ::htons(7777);
//
//while (true)
//{
//	if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
//	{
//		if (::WSAGetLastError() == WSAEWOULDBLOCK)
//		{
//			continue;
//		}
//
//		// 연결된 상황이니 다음으로 넘어간다.
//		if (::WSAGetLastError() == WSAEISCONN)
//		{
//			break;
//		}
//
//		// 문제 상황
//		break;
//	}
//}
//
//cout << "Connected to Server" << endl;
//
//// Send
//char sendBuffer[100] = "Hello World";
//
//while (true)
//{
//	if (::send(clientSocket, sendBuffer, sizeof(sendBuffer), 0) == SOCKET_ERROR)
//	{
//		if (::WSAGetLastError() == WSAEWOULDBLOCK)
//		{
//			continue;
//		}
//
//		break;
//	}
//
//	cout << "Send Data! Len = " << sizeof(sendBuffer) << endl;
//
//	while (true)
//	{
//		char recvBuffer[1000];
//		int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
//		if (recvLen == SOCKET_ERROR)
//		{
//			if (::WSAGetLastError() == WSAEWOULDBLOCK)
//			{
//				continue;
//			}
//
//			// Error
//			break;
//		}
//		else if (recvLen == 0)
//		{
//			break;
//		}
//
//		cout << "Recv Data Len = " << recvLen << endl;
//
//		break;
//	}
//
//	this_thread::sleep_for(1s);
//}
//
//::WSACleanup();
//}
#pragma endregion

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

