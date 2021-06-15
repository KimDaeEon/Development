#include<WinSock2.h>
#pragma comment(lib,"ws2_32")  // �̷��� �ϸ� project settings ���� link tab�� �ִ� input �� ���� �ʾƵ� �ȴ�.
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

SOCKET SetTCPServer(short portNumber, int backLogSize);
void EventLoop(SOCKET sock);


SOCKET sockBase[FD_SETSIZE];  // FD = File Descriptor, ���н� �迭���� ��� ���� ���� �������� �����ؼ� ��ǻ� �������� �ڵ�� ������ �����̶�� �� �� �ִ�.
HANDLE hEventBase[FD_SETSIZE];
int count;

void AcceptProc(int index);
void ReadProc(int index);
void CloseProc(int index);



int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData); // ���� �ʱ�ȭ
	SOCKET sock = SetTCPServer(PORT_NUM, BACK_LOG_SIZE);
	if (sock == -1)
	{
		perror("���� ���� ����");
	}
	else
	{
		EventLoop(sock);
	}
	WSACleanup(); // ���� ����
	return 0;
}


SOCKET SetTCPServer(short portNumber, int backLogSize)
{
	SOCKET sock;

	// ���ͳ� IPv4 ������ �ּ�ü�踦 �� ���̰�, Stream �������� �����͸� ������ ���̸�, TCP �� ���ڴٴ� ��.
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == -1)
	{
		return -1;
	}

	SOCKADDR_IN serverAddr = { 0 }; // ��� ���� ������ �����ϴ� ����ü�̴�.
	serverAddr.sin_family = AF_INET;  // �ּҰ� � �������� �����Ѵ�. AF_INET �� IPv4 ������ �ǹ�. Address Family Internetwork
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // 4 ����Ʈ�� Union ����ü�̴�. �̸� ���ؼ� byte ������ ���� �������ų� ������ �� �ִ�.
	serverAddr.sin_port = htons(portNumber);  // �� port �� 2����Ʈ�̴�. �׷��� ��Ʈ�� ���������� ���� ������ 0~65535 �� ���̴�.

	int result = 0; // ���� ��� �غ� ���� �� ó�� ��� ���� �޾Ƶα� ���� ����.
	result = bind(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)); // bind ���� üũ
	if (result == -1)  // ������ ��� -1�̴�. -1�� 0xFFFFFFFF �ε� �̰��� SOCKET_ERROR �� ����. 
		return -1;

	result = listen(sock, backLogSize); // listen ���� üũ
	if (result == -1)
		return -1;

	printf("Server is listening on %s:%d\n", inet_ntoa(serverAddr.sin_addr), portNumber);
	return sock;
}



HANDLE AddNetworkEvent(SOCKET sock, long netEvent)
{
	HANDLE hEvent = WSACreateEvent();

	sockBase[count] = sock;  // ���� ���� ���� ���� 
	hEventBase[count] = hEvent;  // ���� ���� �̺�Ʈ ��ü ����
	count++;

	WSAEventSelect(sock, hEvent, netEvent);  // ���� ��ü�� �̺�Ʈ ��ü�� ¦���´�. netEvent �� �ش� ���Ͽ� ���� ������ �� �ִ� �̺�Ʈ�� �����Ѵ�.
	return hEvent;
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

void EventLoop(SOCKET listenSocket)  // �� input ������ Listen �Ϸ��� ��� �����̴�.
{
	AddNetworkEvent(listenSocket, FD_ACCEPT);  // ACCEPT �̺�Ʈ�� �����ϴ� �����̶� ���̴�. accept, AcceptEx, WSAAccept �Լ��� �Ⱦ��� ���� �ڵ带 �����Ѵ�.
	while (true)
	{
		int index = WSAWaitForMultipleEvents(count, hEventBase, false, INFINITE, false);
		WSANETWORKEVENTS netEvents;

		// �Ʒ��� ���� ���Ͽ��� �߻��� ��Ʈ��ũ �̺�Ʈ�� ������ �˾Ƴ���, ���� ������ ���� �Լ��� ȣ���Ͽ� ó���Ѵ�.
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
		printf("ä�� ���� �� ���� %s:%d ���� �������� ���߽��ϴ�.\n",
			inet_ntoa(clientAddr.sin_addr),
			ntohs(clientAddr.sin_port));
		closesocket(sock);
		return;
	}

	AddNetworkEvent(sock, FD_READ | FD_CLOSE);  // Read �̺�Ʈ�� �����ϴ� �����̶�� ���̴�. WSARecv, WSARecvEx, WSARecvFrom ���� ���� �� �� �ֵ��� �Ѵ�. FD_CLOSE �� ��� ������ �Լ��� �ٲ�� ���� ����.
	printf("%s:%d �� �����ϼ̽��ϴ�.\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
}

void ReadProc(int index)
{
	char msgBuffer[MAX_MSG_LEN];  // recv �� ���� �޼����� �����ϴ� ����.
	recv(sockBase[index], msgBuffer, MAX_MSG_LEN, 0);

	SOCKADDR_IN clientAddr = { 0 };
	int len = sizeof(clientAddr);
	getpeername(sockBase[index], (SOCKADDR*)&clientAddr, &len);

	char toBeSentMsg[MAX_MSG_LEN];
	// toBeSentMsg �� ���� ���ڿ��� �ش� Ŭ���̾�Ʈ ������ �߰��ؼ� �Է�.
	sprintf(toBeSentMsg, "%s:%d:%s\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), msgBuffer);

	for (int i = 1; i < count; i++)  // 0 ���� ������ listen �����̴�.
	{
		send(sockBase[i], toBeSentMsg, MAX_MSG_LEN, 0);  // ��� Ŭ���̾�Ʈ���� ������ ���� �޼��� ����.
	}
}

void CloseProc(int index)
{
	SOCKADDR_IN	clientAddr = { 0 };
	int len = sizeof(clientAddr);

	getpeername(sockBase[index], (SOCKADDR*)&clientAddr, &len);
	printf("%s:%d �Բ��� ������ �����ϼ̽��ϴ�.\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

	closesocket(sockBase[index]);
	WSACloseEvent(hEventBase[index]);

	count--;
	sockBase[index] = sockBase[count];  // �ϴ� ������� �༮�� �̰��� �ξ ��ü�� �޼����� ���޵ǵ��� ����. ���� �ڵ�� �̷��� �Ǿ� �ִµ�, ��� queue ���� ������ �߾�� ���� �ʳ� �ʹ�..
	hEventBase[index] = hEventBase[count];

	char msg[MAX_MSG_LEN];
	sprintf(msg, "%s:%d �Բ��� ������ �����ϼ̽��ϴ�.\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

	for (int i = 1; i < count; i++) { // 0 ���� ������ listen �����̴�.
		send(sockBase[i], msg, MAX_MSG_LEN, 0);
	}

}