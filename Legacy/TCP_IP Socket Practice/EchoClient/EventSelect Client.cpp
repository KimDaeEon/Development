#include<WinSock2.h>
#pragma comment(lib,"ws2_32")  // �̷��� �ϸ� project settings ���� link tab�� �ִ� input �� ���� �ʾƵ� �ȴ�.
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
	WSAStartup(MAKEWORD(2, 2), &wsaData); // ���� �ʱ�ȭ

	int result = 0; // ���� ��� �غ� ���� �� ó�� ��� ���� �޾Ƶα� ���� ����.

	SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == -1)
	{
		return -1;
	}

	SOCKADDR_IN serverAddr = { 0 }; // ��� ���� ������ �����ϴ� ����ü�̴�.
	serverAddr.sin_family = AF_INET;  // �ּҰ� � �������� �����Ѵ�. AF_INET �� IPv4 ������ �ǹ�. Address Family Internetwork
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);  // ���ڿ��� 4����Ʈ ���� IP �ּҷ� ����
	serverAddr.sin_port = htons(PORT_NUM);  // �� port �� 2����Ʈ�̴�. �׷��� ��Ʈ�� ���������� ���� ������ 0~65535 �� ���̴�.

	result = connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (result == -1)
		return -1;

	_beginthread(RecvThreadPoint, 0, (void*)sock);
	char msg[MAX_MSG_LEN] = "";

	while (true)
	{
		gets_s(msg, MAX_MSG_LEN);  // ���⼭ ������ �޼����� ������. �޼����� �޴� ���� RecvThreadPoint �� ���� �ٸ� �����忡�� ó���Ѵ�.
		send(sock, msg, sizeof(msg), 0);
		if (strcmp(msg, "exit") == 0)
		{
			break;
		}
	}
	
	closesocket(sock);
	WSACleanup(); // ���� ����
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