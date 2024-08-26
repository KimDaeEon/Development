#include<WinSock2.h>
#pragma comment(lib,"ws2_32")  // 이렇게 하면 project settings 에서 link tab에 있는 input 에 쓰지 않아도 된다.
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

int main()
{
	WSADATA wsaData;
	//MAKEWORD(2,2) = 0x0202 이런 
	// 형식의 데이터를 만들어주는 것이다.
	WSAStartup(MAKEWORD(2, 2), &wsaData);



	// WSA 데이터 구조
	//printf("wHighVersion:%#x\n", wsaData.wHighVersion);  // 사용할 수 있는 상위 버전
	//printf("wVersion:%#x\n", wsaData.wVersion);  // 현재 사용 중인 버전
	//printf("szDescription:%s\n", wsaData.szDescription);  // 윈속 설명 
	//printf("szSystemStatus:%s\n", wsaData.szSystemStatus);  // 상태 문자열
	//printf("iMaxSocket:%d\n", wsaData.iMaxSockets);  // 최대 소켓 수
	//printf("iMaxUdpDg:%d\n", wsaData.iMaxUdpDg);  // 데이터 그램 최대 크기
	


	// Byte 정렬
	//unsigned int idata = 0x12345678;
	//unsigned short sdata = 0x1234;
	//printf("host:%#x network:%#x \n", idata, htonl(idata));
	//printf("host:%#x network:%#x \n", sdata, htonl(sdata));
	


	// IPv4 주소 변환
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



	// 로컬 호스트 IPv4 주소 얻어오기
	IN_ADDR addr;
	addr = GetDefaultMyIP();
	printf("%s\n", inet_ntoa(addr));
	WSACleanup();
	return 0;
}