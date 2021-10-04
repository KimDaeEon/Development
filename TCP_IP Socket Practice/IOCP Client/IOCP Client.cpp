#include <WinSock2.h>
#include <stdio.h>
#include <tchar.h>
#include<process.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT_NUM		10200		
#define BACK_LOG_SIZE	5
#define MAX_MSG_LEN		1024
#define SERVER_IP		"127.0.0.1"

struct OverlappedSocket {
	WSAOVERLAPPED	overlapped;
	WSABUF			dataBuffer;
	SOCKET			socket;
	char			messageBuffer[MAX_MSG_LEN];
	int				receiveBytes;
	int				sendBytes;
};

#pragma warning(disable:4996)

void RecvThreadPoint(void* param);

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Error, Failed on WSAStartup()\n");
		return 1;
	}

	SOCKET socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	// 소켓 생성
	if (socket == INVALID_SOCKET)
	{
		printf("Error, Failed on WSASocket(), Invalid Socket\n");
		return 1;
	}

	// 서버 정보 설정
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT_NUM);
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

	if (connect(socket, (struct sockaddr*) & serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		printf("Error, Failed on Connect()\n");
		closesocket(socket);
		WSACleanup();
		return 1;
	}
	else
	{
		printf("Server Connected!!\nEnter Message\n");
	}

	_beginthread(RecvThreadPoint, 0, (void*)socket);

	OverlappedSocket* socketInfo;
	DWORD sentBytes;
	DWORD receiveBytes;
	DWORD flags;

	while (1)
	{
		char messageBuffer[MAX_MSG_LEN];
		int i, bufferLen;
		for (i = 0; 1; i++)
		{ 
			messageBuffer[i] = getchar();
			if (messageBuffer[i] == '\n')
			{
				messageBuffer[i++] = '\0';
				break;
			}
		}
		bufferLen = i;

		socketInfo = (struct OverlappedSocket*)malloc(sizeof(struct OverlappedSocket));
		memset((void*)socketInfo, 0x00, sizeof(struct OverlappedSocket));
		socketInfo->dataBuffer.len = bufferLen;
		socketInfo->dataBuffer.buf = messageBuffer;

		sentBytes = send(socket, messageBuffer, bufferLen, 0);
		if (sentBytes > 0)
		{
			printf("Sent Message : %s (%d bytes)\n", messageBuffer, sentBytes);
		}
	}

	closesocket(socket);

	WSACleanup();

	return 0;
}


void RecvThreadPoint(void* param)
{
	SOCKET sock = (SOCKET)param;  // thread 에서 함수가 쓰일려면 인자 같은 경우는 void* 으로 받아서 이렇게 바꾸어 주어야 한다. 모든 타입에 대해 다 제네릭하게 인자를 설정하기 위해서는 이런 구현이 필요한 것이다.
	char msg[MAX_MSG_LEN];

	SOCKADDR_IN clientAddr = { 0 };
	int len = sizeof(clientAddr);

	while (recv(sock, msg, MAX_MSG_LEN, 0) > 0)  // 해당 스레드는 서버에서 메세지를 보내줄 때마다 받아서 화면에 보여준다.
	{
		printf("%s\n", msg);
	}
	closesocket(sock);
}