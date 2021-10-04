#include <WinSock2.h>
#include <stdio.h>
#include <tchar.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT_NUM	10200		
#define BACK_LOG_SIZE 5
#define MAX_MSG_LEN 1024

#pragma warning(disable:4996)

struct OverlappedSocket {
	WSAOVERLAPPED	overlapped;
	WSABUF			dataBuffer;
	SOCKET			socket;
	char			messageBuffer[MAX_MSG_LEN];
	int				receiveBytes;
	int				sendBytes;
};

DWORD WINAPI makeThread(LPVOID hIOCP);

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Error, Failed on WSAStartup()\n");
		return 1;
	}

	SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	
	// ���� ����
	if (listenSocket == INVALID_SOCKET)
	{
		printf("Error, Failed on WSASocket(), Invalid Socket\n");
		return 1;
	}

	// ���� ���� ���� ����
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(PORT_NUM);
	serverAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	// ���� bind
	if (bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		printf("Error, Failed on Bind()\n");
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	// ���� listen
	if (listen(listenSocket, BACK_LOG_SIZE) == SOCKET_ERROR)
	{
		printf("Error, Failed on Listen()\n");
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	printf("Server is running on %s:%d\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));
	
	// ��� ó���ϴ� IO Completion Port �����ϰ�, �ڵ鿡 ����
	HANDLE hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);  // IOCP �ڵ��� ó�� ������ ������ �̷��� �ϰ�, ���� �� ������ ����� ������ ��� ����� �޶�����.
	//HANDLE WINAPI CreateIoCompletionPort(
	//	_In_     HANDLE    FileHandle, ���⼱ ���⿡ INVALID_HANDLE_VALUE �� ���� IOCP �� �����Ѵ�. ���Ŀ� �Ʒ� �ڵ忡���� ���⿡ �ڵ��� �־ ����Ѵ�.
	//	_In_opt_ HANDLE    ExistingCompletionPort, ���� ���� INVALID_HANDLE_VALUE �� ��� NULL �� �ȴ�. ����� ������ �̸� �����Ǿ� �ִ� IOCP �ڵ� ���� �־��ش�.
	//	_In_     ULONG_PTR CompletionKey, FileHandle �� INVALID_HANDLE_VALUE �̸� �� �з����Ͱ� ���õǾ 0�� �Է�. ����� ������ �ڵ� ���� Ű�� �Է��Ѵ�. ��κ��� ��� ���� ��ü�� �Է��Ѵ�. �̸� ���� � �۾��� ���������� �����Ѵ�.
	//	_In_     DWORD     NumberOfConcurrentThreads 0 �̸� �ý����� ���μ��� �� ��ŭ �ڵ����� ����. ExistingCompletionPort �� NULL �� ������ �̰��� ���� �����ϴ�. (NULL �� �ƴϸ� �� �� ������ ���õȴ�.)
	//);

	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);  // ���� �ý����� ������ �о�´�. ������ ũ�⳪ ���μ��� ���� ��� ���� �ý����� �۵��ǰ� �ִ� ������ �����´�. ���⼭�� CPU �������� ����Ѵ�.
	int threadCount = systemInfo.dwNumberOfProcessors * 2;  // CPU * 2 �� ������ ����, ��������� �� ��ġ�� ���� ���ٰ� �Ѵ�. �� �� ������ ������ �޾Ƶ� ���ÿ� �������� �۾��� �ϰ� �ʹٸ� �����带 �ø� ���� �ִٰ� �Ͽ���.
	unsigned long threadId;
	HANDLE* hThread = (HANDLE*)malloc(threadCount * sizeof(HANDLE));  // thread handle ����
	for (int i = 0; i < threadCount; i++)
	{
		hThread[i] = CreateThread(NULL, 0, makeThread, &hIOCP, 0, &threadId);
	}

	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(SOCKADDR_IN);
	memset(&clientAddr, 0, addrLen);
	SOCKET clientSocket;
	OverlappedSocket* socketInfo;
	DWORD receiveBytes;
	DWORD flags;

	while (1)
	{
		clientSocket = accept(listenSocket, (struct sockaddr*)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
		{
			printf("Error, Failed on Accept()\n");
			return 1;
		}

		socketInfo = (struct OverlappedSocket*)malloc(sizeof(struct OverlappedSocket));
		memset((void*)socketInfo, 0x00, sizeof(struct OverlappedSocket));
		socketInfo->socket = clientSocket;
		socketInfo->receiveBytes = 0;
		socketInfo->sendBytes = 0;
		socketInfo->dataBuffer.len = MAX_MSG_LEN;
		socketInfo->dataBuffer.buf = socketInfo->messageBuffer;
		flags = 0;

		// �տ��� ������� IOCP �ڵ鿡 Ŭ���̾�Ʈ ���� �ڵ� �� Ű�� ����մϴ�.
		hIOCP = CreateIoCompletionPort((HANDLE)clientSocket, hIOCP, (DWORD)socketInfo, 0);

		if (WSARecv(socketInfo->socket, &socketInfo->dataBuffer, 1, &receiveBytes, &flags, &(socketInfo->overlapped), NULL))
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				printf("Error, Failed on WSARecv(), IO pending Failure\n");
				return 1;
			}
		}
	}

	closesocket(listenSocket);

	WSACleanup();

	return 0;
}

DWORD WINAPI makeThread(LPVOID hIOCP)
{
	HANDLE threadHandler = *((HANDLE*)hIOCP);

	DWORD receiveBytes;
	DWORD sendBytes;
	DWORD completionKey;
	DWORD flags;
	struct OverlappedSocket* eventSocket;
	
	while (1)
	{
		//BOOL GetQueuedCompletionStatus(
		//	HANDLE CompletionPort,  // CP ������Ʈ �ڵ�  
		//	LPDWORD lpNumberOfBytes,  // ����� �������� �ۼ��� �� ����Ʈ ���� ������ ������ �ּҰ�
		//	PULONG_PTR lpCompletionKey,   // CreateIoCompletioPort�Լ��� ������ ���ڷ� ���޵� ���� ������ ���� ������ �ּҰ�
		//	LPOVERLAPPED * lpOverlapped,  // WSASend, WSARecv �Լ�ȣ��� �����ϴ� OVERLAPPED ����ü ������ �ּҰ��� ����Ǵ� ������ ������
		//	DWORD dwMillisecones   // Ÿ�Ӿƿ� ����, FALSE�� ��ȯ�ϸ鼭 �Լ��� ��������. INFINITE �� �Է� �ÿ� �Ϸ�� IO�� CP������Ʈ�� ��ϵɶ����� ���ŷ����.
		//);

		if (GetQueuedCompletionStatus(threadHandler, &receiveBytes, &completionKey, (LPOVERLAPPED*)&eventSocket, INFINITE) == 0)
		{
			printf("Error, Failed on GetQueuedCompletionStatus()\n");
			closesocket(eventSocket->socket);
			free(eventSocket);
			return 1;
		}

		eventSocket->dataBuffer.len = receiveBytes;

		if (receiveBytes == 0)
		{
			closesocket(eventSocket->socket);
			free(eventSocket);
			continue;
		}
		else
		{
			printf("Received Message : %s (%d bytes)\n", eventSocket->dataBuffer.buf, eventSocket->dataBuffer.len);

			if (WSASend(eventSocket->socket, &(eventSocket->dataBuffer), 1, &sendBytes, 0, NULL, NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					printf("Error, Failed on WSASend(), Error Code = %d\n", WSAGetLastError());
				}
			}

			printf("Sent Message : %s (%d bytes)\n", eventSocket->dataBuffer.buf, eventSocket->dataBuffer.len);

			memset(eventSocket->messageBuffer, 0x00, MAX_MSG_LEN);
			eventSocket->receiveBytes = 0;
			eventSocket->sendBytes = 0;
			eventSocket->dataBuffer.len = MAX_MSG_LEN;
			eventSocket->dataBuffer.buf = eventSocket->messageBuffer;
			flags = 0;

			if (WSARecv(eventSocket->socket, &(eventSocket->dataBuffer), 1, &receiveBytes, &flags, &eventSocket->overlapped, NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					printf("Error, Failed on WSARecv(), Error Code = %d\n", WSAGetLastError());
				}
			}
		}
	}
}