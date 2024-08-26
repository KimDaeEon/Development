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
	
	// 소켓 생성
	if (listenSocket == INVALID_SOCKET)
	{
		printf("Error, Failed on WSASocket(), Invalid Socket\n");
		return 1;
	}

	// 서버 소켓 정보 설정
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(PORT_NUM);
	serverAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	// 소켓 bind
	if (bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		printf("Error, Failed on Bind()\n");
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	// 소켓 listen
	if (listen(listenSocket, BACK_LOG_SIZE) == SOCKET_ERROR)
	{
		printf("Error, Failed on Listen()\n");
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	printf("Server is running on %s:%d\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));
	
	// 결과 처리하는 IO Completion Port 생성하고, 핸들에 연결
	HANDLE hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);  // IOCP 핸들을 처음 생성할 때에는 이렇게 하고, 파일 및 소켓을 등록할 때에는 사용 방식이 달라진다.
	//HANDLE WINAPI CreateIoCompletionPort(
	//	_In_     HANDLE    FileHandle, 여기선 여기에 INVALID_HANDLE_VALUE 가 들어가서 IOCP 를 생성한다. 이후에 아래 코드에서는 여기에 핸들을 넣어서 등록한다.
	//	_In_opt_ HANDLE    ExistingCompletionPort, 위의 값이 INVALID_HANDLE_VALUE 일 경우 NULL 이 된다. 등록할 때에는 미리 생성되어 있는 IOCP 핸들 값을 넣어준다.
	//	_In_     ULONG_PTR CompletionKey, FileHandle 이 INVALID_HANDLE_VALUE 이면 이 패러미터가 무시되어서 0을 입력. 등록할 때에는 핸들 값의 키를 입력한다. 대부분의 경우 연결 개체를 입력한다. 이를 통해 어떤 작업이 끝났는지를 구분한다.
	//	_In_     DWORD     NumberOfConcurrentThreads 0 이면 시스템의 프로세스 수 만큼 자동으로 설정. ExistingCompletionPort 가 NULL 일 때에만 이것이 설정 가능하다. (NULL 이 아니면 이 값 설정은 무시된다.)
	//);

	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);  // 현재 시스템의 정보를 읽어온다. 페이지 크기나 프로세스 개수 등등 현재 시스템이 작동되고 있는 정보를 가져온다. 여기서는 CPU 정보만을 사용한다.
	int threadCount = systemInfo.dwNumberOfProcessors * 2;  // CPU * 2 개 스레드 생성, 통계적으로 이 수치가 가장 낫다고 한다. 좀 더 느리게 응답을 받아도 동시에 여러개의 작업을 하고 싶다면 스레드를 늘릴 수도 있다고 하였다.
	unsigned long threadId;
	HANDLE* hThread = (HANDLE*)malloc(threadCount * sizeof(HANDLE));  // thread handle 선언
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

		// 앞에서 만들었던 IOCP 핸들에 클라이언트 소켓 핸들 및 키를 등록합니다.
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
		//	HANDLE CompletionPort,  // CP 오브젝트 핸들  
		//	LPDWORD lpNumberOfBytes,  // 입출력 과정에서 송수신 된 바이트 수를 저장할 변수의 주소값
		//	PULONG_PTR lpCompletionKey,   // CreateIoCompletioPort함수의 세번쨰 인자로 전달된 값의 저장을 위한 변수의 주소값
		//	LPOVERLAPPED * lpOverlapped,  // WSASend, WSARecv 함수호출시 전달하는 OVERLAPPED 구조체 변수의 주소값이 저장되는 변수의 포인터
		//	DWORD dwMillisecones   // 타임아웃 설정, FALSE를 반환하면서 함수를 빠져나감. INFINITE 를 입력 시에 완료된 IO가 CP오브젝트에 등록될때까지 블로킹상태.
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