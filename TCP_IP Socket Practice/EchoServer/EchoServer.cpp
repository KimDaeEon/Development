#include<WinSock2.h>
#pragma comment(lib,"ws2_32")  // 이렇게 하면 project settings 에서 link tab에 있는 input 에 쓰지 않아도 된다.
#include <stdio.h>
#pragma warning(disable:4996)

#include<Windows.h>
#include<process.h>

#define PORT_NUM 10200
#define BACK_LOG_SIZE 5
#define MAX_MSG_LEN 256


IN_ADDR GetDefaultMyIP()
{
	char localHostName[MAX_PATH];
	IN_ADDR addr = { 0, };

	if (gethostname(localHostName, MAX_PATH) == SOCKET_ERROR)
	{
		return addr;
	}

	HOSTENT* hPtr = gethostbyname(localHostName);  // host 의 이름을 받아오는 함수.
	while (hPtr && hPtr->h_name)  // host 의 이름이 없으면 while 문이 멈춥니다.
	{
		if (hPtr->h_addrtype == PF_INET)
		{
			memcpy(&addr, hPtr->h_addr_list[0], hPtr->h_length);
			break;
		}
		hPtr++;
	}

	return addr;
}

SOCKET SetTCPServer(short portNumber, int backLogSize);
void EventLoop(SOCKET sock);


SOCKET sockBase[FD_SETSIZE];  // FD = File Descriptor, 유닉스 계열에서 모든 것을 파일 형식으로 관리해서 사실상 윈도우의 핸들과 유사한 개념이라고 할 수 있다.
HANDLE hEventBase[FD_SETSIZE];
int count;

void AcceptProc(int index);
void ReadProc(int index);
void CloseProc(int index);



int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData); // 윈속 초기화
	SOCKET sock = SetTCPServer(PORT_NUM, BACK_LOG_SIZE);
	if (sock == -1)
	{
		perror("리슨 소켓 오류");
	}
	else
	{
		EventLoop(sock);
	}
	WSACleanup(); // 윈속 해제
	return 0;
}


SOCKET SetTCPServer(short portNumber, int backLogSize)
{
	SOCKET sock;

	// 인터넷 IPv4 형식의 주소체계를 쓸 것이고, Stream 형식으로 데이터를 전송할 것이며, TCP 를 쓰겠다는 뜻.
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == -1)
	{
		return -1;
	}

	SOCKADDR_IN serverAddr = { 0 }; // 통신 관련 정보를 설정하는 구조체이다.
	serverAddr.sin_family = AF_INET;  // 주소가 어떤 형식인지 설정한다. AF_INET 은 IPv4 형식을 의미. Address Family Internetwork
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // 4 바이트의 Union 구조체이다. 이를 통해서 byte 단위로 값을 가져오거나 설정할 수 있다.
	serverAddr.sin_port = htons(portNumber);  // 요 port 가 2바이트이다. 그래서 포트로 설정가능한 숫자 범위가 0~65535 인 것이다.

	int result = 0; // 소켓 통신 준비 과정 중 처리 결과 값을 받아두기 위한 변수.
	result = bind(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)); // bind 에러 체크
	if (result == -1)  // 에러인 경우 -1이다. -1은 0xFFFFFFFF 인데 이것이 SOCKET_ERROR 와 같다. 
		return -1;

	result = listen(sock, backLogSize); // listen 에러 체크
	if (result == -1)
		return -1;

	printf("Server is listening on %s:%d\n", inet_ntoa(serverAddr.sin_addr), portNumber);
	return sock;
}



HANDLE AddNetworkEvent(SOCKET sock, long netEvent)
{
	HANDLE hEvent = WSACreateEvent();

	sockBase[count] = sock;  // 새로 만든 소켓 저장 
	hEventBase[count] = hEvent;  // 새로 만든 이벤트 객체 저장
	count++;

	WSAEventSelect(sock, hEvent, netEvent);  // 소켓 객체와 이벤트 객체를 짝짓는다. netEvent 는 해당 소켓에 대해 실행할 수 있는 이벤트를 설정한다.
	return hEvent;
}

void Converse(SOCKET connectionSocket)
{
	char msg[MAX_MSG_LEN] = "";
	while (recv(connectionSocket, msg, sizeof(msg), 0) > 0)  // 수신한 메세지의 바이트 수를 recv 는 리턴한다.
	{
		printf("recv:%s\n", msg);
		send(connectionSocket, msg, sizeof(msg), 0);  // 수신한 메세지를 그대로 보내준 것이다.
	}
	closesocket(connectionSocket);
}

void EventLoop(SOCKET listenSocket)  // 이 input 소켓은 Listen 하려고 썼던 소켓이다.
{
	AddNetworkEvent(listenSocket, FD_ACCEPT);  // ACCEPT 이벤트를 관리하는 소켓이란 뜻이다. accept, AcceptEx, WSAAccept 함수를 안쓰면 에러 코드를 리턴한다.
	while (true)
	{
		int index = WSAWaitForMultipleEvents(count, hEventBase, false, INFINITE, false);
		WSANETWORKEVENTS netEvents;

		// 아래를 통해 소켓에서 발생한 네트워크 이벤트의 종류를 알아내고, 이후 적절한 소켓 함수를 호출하여 처리한다.
		WSAEnumNetworkEvents(sockBase[index], hEventBase[index], &netEvents);
		switch (netEvents.lNetworkEvents)
		{
		case FD_ACCEPT: AcceptProc(index); break;
		case FD_READ: ReadProc(index); break;
		case FD_CLOSE: CloseProc(index); break;
		}
	}
	closesocket(listenSocket);
}

void AcceptProc(int index)
{
	SOCKADDR_IN clientAddr = { 0 };
	int len = sizeof(clientAddr);
	SOCKET sock = accept(sockBase[0], (struct sockaddr*)&clientAddr, &len);

	if (count == FD_SETSIZE)
	{
		printf("채팅 방이 꽉 차서 %s:%d 님은 입장하지 못했습니다.\n",
			inet_ntoa(clientAddr.sin_addr),
			ntohs(clientAddr.sin_port));
		closesocket(sock);
		return;
	}

	AddNetworkEvent(sock, FD_READ | FD_CLOSE);  // Read 이벤트를 관리하는 소켓이라는 뜻이다. WSARecv, WSARecvEx, WSARecvFrom 같은 것을 쓸 수 있도록 한다. FD_CLOSE 는 사용 가능한 함수가 바뀌는 것은 없다.
	printf("%s:%d 님 입장하셨습니다.\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
}

void ReadProc(int index)
{
	char msgBuffer[MAX_MSG_LEN];  // recv 로 받은 메세지를 저장하는 변수.
	recv(sockBase[index], msgBuffer, MAX_MSG_LEN, 0);

	SOCKADDR_IN clientAddr = { 0 };
	int len = sizeof(clientAddr);
	getpeername(sockBase[index], (SOCKADDR*)&clientAddr, &len);

	char toBeSentMsg[MAX_MSG_LEN];
	// toBeSentMsg 에 받은 문자열에 해당 클라이언트 정보를 추가해서 입력.
	sprintf(toBeSentMsg, "%s:%d:%s\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), msgBuffer);

	for (int i = 1; i < count; i++)  // 0 번은 무조건 listen 소켓이다.
	{
		send(sockBase[i], toBeSentMsg, MAX_MSG_LEN, 0);  // 모든 클라이언트에게 서버가 받은 메세지 전송.
	}
}

void CloseProc(int index)
{
	SOCKADDR_IN	clientAddr = { 0 };
	int len = sizeof(clientAddr);

	getpeername(sockBase[index], (SOCKADDR*)&clientAddr, &len);
	printf("%s:%d 님께서 접속을 종료하셨습니다.\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

	closesocket(sockBase[index]);
	WSACloseEvent(hEventBase[index]);

	count--;
	sockBase[index] = sockBase[count];  // 일단 사라지는 녀석을 이곳에 두어서 전체에 메세지가 전달되도록 세팅. 예제 코드는 이렇게 되어 있는데, 사실 queue 같은 것으로 했어야 하지 않나 싶다..
	hEventBase[index] = hEventBase[count];

	char msg[MAX_MSG_LEN];
	sprintf(msg, "%s:%d 님께서 접속을 종료하셨습니다.\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

	for (int i = 1; i < count; i++) { // 0 번은 무조건 listen 소켓이다.
		send(sockBase[i], msg, MAX_MSG_LEN, 0);
	}

}