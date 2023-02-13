#pragma once
#include "NetworkAddress.h"

// ------------------------
//		 SocketUtils
// ------------------------

class SocketUtils
{
public:
	// 아래 함수 포인터들은 동적으로 할당된다. AcceptEx를 그냥 호출하는 것이 내부적으로 부하가 더 크기 때문이라고 한다.
	// 자세한 내용은 링크 참고 https://stackoverflow.com/questions/4470645/acceptex-without-wsaioctl
	static LPFN_CONNECTEX ConnectEx;
	static LPFN_DISCONNECTEX DisconnectEx;
	static LPFN_ACCEPTEX AcceptEx;

public:
	static void Init();
	static void Clear();

	static bool BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn);
	static SOCKET CreateSocket();

	// 아래 함수들은 socket 옵션 세팅용 함수들이다.
	static bool SetLinger(SOCKET socket, uint16 onoff, uint16 linger);
	static bool SetReuseAddress(SOCKET socket, bool flag);
	static bool SetRecvBufferSize(SOCKET socket, int32 size);
	static bool SetSendBufferSize(SOCKET socket, int32 size);
	static bool SetTcpNoDelay(SOCKET socket, bool flag );
	static bool SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket);


	static bool Bind(SOCKET socket, NetworkAddress netAddr);
	static bool BindAnyAddress(SOCKET socket, uint16 port);
	static bool Listen(SOCKET socket, int32 backLog = SOMAXCONN);
	static void Close(SOCKET& socket);
};

// setsockopt 편하게 쓰기 위해 만든 템플릿 함수
template<typename T>
static inline bool SetSockOpt(SOCKET socket, int32 level, int32 optName, T optVal)
{
	return SOCKET_ERROR != ::setsockopt(socket, level, optName, reinterpret_cast<char*>(&optVal), sizeof(T));
}