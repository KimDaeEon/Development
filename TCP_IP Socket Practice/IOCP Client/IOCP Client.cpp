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

	// ���� ����
	if (socket == INVALID_SOCKET)
	{
		printf("Error, Failed on WSASocket(), Invalid Socket\n");
		return 1;
	}

	// ���� ���� ����
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
	SOCKET sock = (SOCKET)param;  // thread ���� �Լ��� ���Ϸ��� ���� ���� ���� void* ���� �޾Ƽ� �̷��� �ٲپ� �־�� �Ѵ�. ��� Ÿ�Կ� ���� �� ���׸��ϰ� ���ڸ� �����ϱ� ���ؼ��� �̷� ������ �ʿ��� ���̴�.
	char msg[MAX_MSG_LEN];

	SOCKADDR_IN clientAddr = { 0 };
	int len = sizeof(clientAddr);

	while (recv(sock, msg, MAX_MSG_LEN, 0) > 0)  // �ش� ������� �������� �޼����� ������ ������ �޾Ƽ� ȭ�鿡 �����ش�.
	{
		printf("%s\n", msg);
	}
	closesocket(sock);
}