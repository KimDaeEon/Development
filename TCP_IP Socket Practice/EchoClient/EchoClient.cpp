#include<WinSock2.h>
#pragma comment(lib,"ws2_32")  // �̷��� �ϸ� project settings ���� link tab�� �ִ� input �� ���� �ʾƵ� �ȴ�.
#include <stdio.h>
#pragma warning(disable:4996)

#define PORT_NUM 10200
#define BACK_LOG_SIZE 5
#define MAX_MSG_LEN 256

SOCKET SetTCPServer(int portNumber, int backLogSize);
IN_ADDR GetDefaultMyIP();
void AcceptLoop(SOCKET sock);



int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData); // ���� �ʱ�ȭ
	char serverIP[40] = "";
	gets_s(serverIP, sizeof(serverIP));

	int result = 0; // ���� ��� �غ� ���� �� ó�� ��� ���� �޾Ƶα� ���� ����.

	SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN serverAddr = { 0 }; // ��� ���� ������ �����ϴ� ����ü�̴�.
	serverAddr.sin_family = AF_INET;  // �ּҰ� � �������� �����Ѵ�. AF_INET �� IPv4 ������ �ǹ�. Address Family Internetwork
	serverAddr.sin_addr.s_addr = inet_addr(serverIP);  // ���ڿ��� 4����Ʈ ���� IP �ּҷ� ����
	serverAddr.sin_port = htons(PORT_NUM);  // �� port �� 2����Ʈ�̴�. �׷��� ��Ʈ�� ���������� ���� ������ 0~65535 �� ���̴�.

	result = connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (result == -1)
		return -1;

	char msg[MAX_MSG_LEN] = "";
	while (true)
	{
		gets_s(msg, MAX_MSG_LEN);
		send(sock, msg, sizeof(msg), 0);
		if (strcmp(msg, "exit") == 0)
		{
			break;
		}

		recv(sock, msg, sizeof(msg), 0);
		printf("�������� ������ �޼���: %s\n", msg);
	}

	closesocket(sock);
	WSACleanup(); // ���� ����
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

	HOSTENT* hPtr = gethostbyname(localHostName);  // host �� �̸��� �޾ƿ��� �Լ�.
	while (hPtr && hPtr->h_name)  // host �� �̸��� ������ while ���� ����ϴ�.
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


SOCKET SetTCPServer(int portNumber, int backLogSize)
{
	int result = 0; // ���� ��� �غ� ���� �� ó�� ��� ���� �޾Ƶα� ���� ����.
	SOCKET sock;

	// ���ͳ� IPv4 ������ �ּ�ü�踦 �� ���̰�, Stream �������� �����͸� ������ ���̸�, TCP �� ���ڴٴ� ��.
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN serverAddr = { 0 }; // ��� ���� ������ �����ϴ� ����ü�̴�.
	serverAddr.sin_family = AF_INET;  // �ּҰ� � �������� �����Ѵ�. AF_INET �� IPv4 ������ �ǹ�. Address Family Internetwork
	serverAddr.sin_addr = GetDefaultMyIP();  // 4 ����Ʈ�� Union ����ü�̴�. �̸� ���ؼ� byte ������ ���� �������ų� ������ �� �ִ�.
	serverAddr.sin_port = htons(portNumber);  // �� port �� 2����Ʈ�̴�. �׷��� ��Ʈ�� ���������� ���� ������ 0~65535 �� ���̴�.

	result = bind(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)); // bind ���� üũ
	if (result == -1)  // ������ ��� -1�̴�. -1�� 0xFFFFFFFF �ε� �̰��� SOCKET_ERROR �� ����. 
		return -1;

	result = listen(sock, backLogSize); // listen ���� üũ
	if (result == -1)
		return -1;

	return 0;
}

void Converse(SOCKET connectionSocket)
{
	char msg[MAX_MSG_LEN] = "";
	while (recv(connectionSocket, msg, sizeof(msg), 0) > 0)  // ������ �޼����� ����Ʈ ���� recv �� �����Ѵ�.
	{
		printf("recv:%s\n", msg);
		send(connectionSocket, msg, sizeof(msg), 0);  // ������ �޼����� �״�� ������ ���̴�.
	}
	closesocket(connectionSocket);
}

void AcceptLoop(SOCKET listenSocket)  // �� input ������ Listen �Ϸ��� ��� �����̴�.
{
	SOCKET connectionSocket;  // ���� Ŭ���̾�Ʈ�� ������ �ξ����� ����.
	SOCKADDR_IN clientAddr = { 0 };  // Ŭ���̾�Ʈ ������ ��� ���� ����ü.
	int len = sizeof(clientAddr);
	while (true)
	{
		connectionSocket = accept(listenSocket, (SOCKADDR*)&clientAddr, &len); // ���� ����

		if (connectionSocket == -1)
		{
			perror("accept ����");
			break;
		}

		printf("%s:%d�� ���� ��û ����\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
		Converse(connectionSocket);
	}
}