#include<WinSock2.h>
#pragma comment(lib,"ws2_32")  // 이렇게 하면 project settings 에서 link tab에 있는 input 에 쓰지 않아도 된다.
#include <stdio.h>
#pragma warning(disable:4996)

#include<Windows.h>
#include<process.h>

#define PORT_NUM 10200
#define BACK_LOG_SIZE 5
#define MAX_MSG_LEN 256

SOCKET SetTCPServer(short portNumber, int backLogSize);
IN_ADDR GetDefaultMyIP();
void AcceptLoop(SOCKET sock);



int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData); // 윈속 초기화
	SOCKET sock = SetTCPServer(PORT_NUM, BACK_LOG_SIZE);
	AcceptLoop(sock);
	closesocket(sock);
	WSACleanup(); // 윈속 해제
	return 0;
}


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


SOCKET SetTCPServer(short portNumber, int backLogSize)  // 백 로그란 동시에 연결을 기다리는(연결된 클라이언트는 제외) 최대 클라이언트 수를 의미한다.
{
	int result = 0; // 소켓 통신 준비 과정 중 처리 결과 값을 받아두기 위한 변수.
	SOCKET sock;

	// 인터넷 IPv4 형식의 주소체계를 쓸 것이고, Stream 형식으로 데이터를 전송할 것이며, TCP 를 쓰겠다는 뜻.
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	  
	SOCKADDR_IN serverAddr = { 0 }; // 통신 관련 정보를 설정하는 구조체이다.
	serverAddr.sin_family = AF_INET;  // 주소가 어떤 형식인지 설정한다. AF_INET 은 IPv4 형식을 의미. Address Family Internetwork
	serverAddr.sin_addr = GetDefaultMyIP();  // 4 바이트의 Union 구조체이다. 이를 통해서 byte 단위로 값을 가져오거나 설정할 수 있다.
	serverAddr.sin_port = htons(portNumber);  // 요 port 가 2바이트이다. 그래서 포트로 설정가능한 숫자 범위가 0~65535 인 것이다.
	
	result = bind(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)); // bind 에러 체크
	if (result == -1)  // 에러인 경우 -1이다. -1은 0xFFFFFFFF 인데 이것이 SOCKET_ERROR 와 같다. 
		return -1;

	result = listen(sock, backLogSize); // listen 에러 체크, 클라이언트와는 다르게 서버는 listen 이라는 절차가 있다.
	if (result == -1)
		return -1;
	
	// 서버가 listen 을 시작하면 서버 주소 출력
	printf("Server IP is listening on %s:%d\n", inet_ntoa(serverAddr.sin_addr),portNumber);
	return sock;
}

void Converse(void* _connectionSocket)
{
	SOCKET connectionSocket = (SOCKET)_connectionSocket;
	SOCKADDR_IN clientAddr = { 0 };
	int len = sizeof(clientAddr);
	
	getpeername(connectionSocket, (SOCKADDR*)&clientAddr, &len);  // 연결된 호스트의 정보를 소켓을 통해 받아와서 clientAddr 에 저장한다.
	char msg[MAX_MSG_LEN] = "";
	while (recv(connectionSocket, msg, sizeof(msg), 0) > 0)  // 수신한 메세지의 바이트 수를 recv 는 리턴한다. block 함수이다.
	{
		printf("%s:%d로 부터 받은 메세지:%s\n", 
			inet_ntoa(clientAddr.sin_addr),
			ntohs(clientAddr.sin_port),
			msg);
		send(connectionSocket, msg, sizeof(msg), 0);  // 수신한 메세지를 그대로 보내준 것이다. block 함수이다.
	}

	printf("%s:%d와 통신 종료\n",
		inet_ntoa(clientAddr.sin_addr),
		ntohs(clientAddr.sin_port));
	closesocket(connectionSocket);
}

void AcceptLoop(SOCKET listenSocket)  // 이 input 소켓은 Listen 하려고 썼던 소켓이다.
{
	SOCKET connectionSocket;  // 실제 클라이언트와 연결이 맺어지는 소켓.
	SOCKADDR_IN clientAddr = { 0 };  // 클라이언트 정보를 담기 위한 구조체.
	int len = sizeof(clientAddr);
	while (true)
	{
		connectionSocket = accept(listenSocket, (SOCKADDR*)&clientAddr, &len); // 연결 수락, block 함수이다.

		if (connectionSocket == -1)
		{
			perror("accept 실패");
			break;
		}

		printf("%s:%d의 연결 요청 수락\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
		//Converse((void*)connectionSocket);
		_beginthread(Converse, 0, (void*)connectionSocket);
	}
}