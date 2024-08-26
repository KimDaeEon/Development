#include<WinSock2.h>
#pragma comment(lib,"ws2_32")  // �̷��� �ϸ� project settings ���� link tab�� �ִ� input �� ���� �ʾƵ� �ȴ�.
#include <stdio.h>
#pragma warning(disable:4996)

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

int main()
{
	WSADATA wsaData;
	//MAKEWORD(2,2) = 0x0202 �̷� 
	// ������ �����͸� ������ִ� ���̴�.
	WSAStartup(MAKEWORD(2, 2), &wsaData);



	// WSA ������ ����
	//printf("wHighVersion:%#x\n", wsaData.wHighVersion);  // ����� �� �ִ� ���� ����
	//printf("wVersion:%#x\n", wsaData.wVersion);  // ���� ��� ���� ����
	//printf("szDescription:%s\n", wsaData.szDescription);  // ���� ���� 
	//printf("szSystemStatus:%s\n", wsaData.szSystemStatus);  // ���� ���ڿ�
	//printf("iMaxSocket:%d\n", wsaData.iMaxSockets);  // �ִ� ���� ��
	//printf("iMaxUdpDg:%d\n", wsaData.iMaxUdpDg);  // ������ �׷� �ִ� ũ��
	


	// Byte ����
	//unsigned int idata = 0x12345678;
	//unsigned short sdata = 0x1234;
	//printf("host:%#x network:%#x \n", idata, htonl(idata));
	//printf("host:%#x network:%#x \n", sdata, htonl(sdata));
	


	// IPv4 �ּ� ��ȯ
	/*IN_ADDR addr;
	addr.s_addr = htonl(12 << 24 | 34 << 16 | 56 << 8 | 78);
	printf("%s\n", inet_ntoa(addr));

	u_int nAddr = inet_addr("192.168.34.0");
	u_int hAddr = ntohl(nAddr);

	printf("%d.%d.%d.%d\n",
		hAddr >> 24,
		(u_char)(hAddr >> 16),
		(u_char)(hAddr >> 8),
		(u_char)hAddr);*/



	// ���� ȣ��Ʈ IPv4 �ּ� ������
	IN_ADDR addr;
	addr = GetDefaultMyIP();
	printf("%s\n", inet_ntoa(addr));
	WSACleanup();
	return 0;
}