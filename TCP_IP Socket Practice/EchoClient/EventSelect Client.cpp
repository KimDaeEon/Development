#include<WinSock2.h>
#pragma comment(lib,"ws2_32")  // 이렇게 하면 project settings 에서 link tab에 있는 input 에 쓰지 않아도 된다.
#include <stdio.h>
#include<process.h>
#pragma warning(disable:4996)

#define PORT_NUM 10200
#define BACK_LOG_SIZE 5
#define MAX_MSG_LEN 256
#define SERVER_IP "127.0.0.1"

void RecvThreadPoint(void* param);

int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData); // 윈속 초기화

	int result = 0; // 소켓 통신 준비 과정 중 처리 결과 값을 받아두기 위한 변수.

	SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == -1)
	{
		return -1;
	}

	SOCKADDR_IN serverAddr = { 0 }; // 통신 관련 정보를 설정하는 구조체이다.
	serverAddr.sin_family = AF_INET;  // 주소가 어떤 형식인지 설정한다. AF_INET 은 IPv4 형식을 의미. Address Family Internetwork
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);  // 문자열을 4바이트 형식 IP 주소로 리턴
	serverAddr.sin_port = htons(PORT_NUM);  // 요 port 가 2바이트이다. 그래서 포트로 설정가능한 숫자 범위가 0~65535 인 것이다.

	result = connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (result == -1)
		return -1;

	_beginthread(RecvThreadPoint, 0, (void*)sock);
	char msg[MAX_MSG_LEN] = "";

	while (true)
	{
		gets_s(msg, MAX_MSG_LEN);  // 여기서 서버에 메세지를 보낸다. 메세지를 받는 것은 RecvThreadPoint 를 통해 다른 스레드에서 처리한다.
		send(sock, msg, sizeof(msg), 0);
		if (strcmp(msg, "exit") == 0)
		{
			break;
		}
	}
	
	closesocket(sock);
	WSACleanup(); // 윈속 해제
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