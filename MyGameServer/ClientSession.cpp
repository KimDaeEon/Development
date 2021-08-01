#include "stdafx.h"
#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "CircularQueue.h"
#include "NetworkSession.h"
#include "PacketSession.h"
#include "EventSelect.h"
#include "ClientSession.h"

/// <summary>
/// TCP, UDP 모든 기능을 담고 있는 세션 클래스의 생성자이다. EventSelect 클래스를 상속받았다.
/// </summary>
/// <param name=""></param>
CClientSession::CClientSession(VOID)
{
	IsUdp = FALSE;
}

CClientSession::~CClientSession(VOID)
{
}

/// <summary>
/// TCP 를 사용할 때에 사용하는 함수이다. 접속할 상대 주소와 포트를 파라미터로 입력한다.
/// </summary>
/// <param name="remoteAddress"></param>
/// <param name="remotePort"></param>
/// <returns></returns>
BOOL CClientSession::BeginTcp(LPSTR remoteAddress, USHORT remotePort)
{
	if (!remoteAddress || remotePort <= 0)
		return FALSE;

	// 사용할 세션 개체 초기화
	if (!mSession.Begin())
	{
		End();

		return FALSE;
	}

	// TCP 로 사용하기 위한 바인딩 실시
	if (!mSession.TcpBind())
	{
		End();

		return FALSE;
	}

	// EventSelect 방식으로 해당 소켓의 이벤트를 받아오는 것을 등록한다.
	if (!CEventSelect::Begin(mSession.GetSocket()))
	{
		End();

		return FALSE;
	}

	// 접속할 주소에 접속
	if (!mSession.Connect(remoteAddress, remotePort))
	{
		End();

		return FALSE;
	}

	return TRUE;
}

/// <summary>
/// UDP 를 사용할 때에 초기화하는 함수이다. TCP 와 달리 사용할(바인딩) 포트만 입력한다.
/// </summary>
/// <param name="remotePort"></param>
/// <returns></returns>
BOOL CClientSession::BeginUdp(USHORT remotePort)
{
#if defined(_EXTERNAL_RELEASE_)
	if (remotePort <= 0)
		return FALSE;
#endif

	if (!mSession.Begin())
	{
		End();

		return FALSE;
	}

	// UDP 로 바인딩
	if (!mSession.UdpBind(remotePort))
	{
		End();

		return FALSE;
	}

	// EventSelect 에 등록
	if (!CEventSelect::Begin(mSession.GetSocket()))
	{
		End();

		return FALSE;
	}

	// UDP 로 사용하는 것 체크
	IsUdp = TRUE;

	return TRUE;
}

BOOL CClientSession::End(VOID)
{
	// 세션 종료
	mSession.End();

	// 이벤트 셀렉트 객체 종료
	CEventSelect::End();

	return TRUE;
}

BOOL CClientSession::GetLocalIP(WCHAR* pIP)
{
	return mSession.GetLocalIP(pIP);
}

USHORT CClientSession::GetLocalPort(VOID)
{
	return mSession.GetLocalPort();
}

/// <summary>
/// 패킷 1 개를 받아오는 함수이다.
/// </summary>
/// <param name="protocol"></param>
/// <param name="packet"></param>
/// <param name="packetLength"></param>
/// <returns></returns>
BOOL CClientSession::ReadPacket(DWORD& protocol, BYTE* packet, DWORD& packetLength)
{
	VOID* Object = NULL;

	// 큐에서 패킷 한 개를 받아온다.
	return mReadPacketQueue.Pop(&Object, protocol, packet, packetLength);
}

BOOL CClientSession::ReadFromPacket(DWORD& protocol, LPSTR remoteAddress, USHORT& remotePort, BYTE* packet, DWORD& packetLength)
{
	VOID* Object = NULL;

	return mReadPacketQueue.Pop(&Object, protocol, packet, packetLength, remoteAddress, remotePort);
}

/// <summary>
/// TCP 데이터를 전송하는 함수이다.
/// </summary>
/// <param name="protocol"></param>
/// <param name="packet"></param>
/// <param name="packetLength"></param>
/// <returns></returns>
BOOL CClientSession::WritePacket(DWORD protocol, const BYTE* packet, DWORD packetLength)
{
	// 데이터 전송 함수 호출
	if (!mSession.WritePacket(protocol, packet, packetLength))
		return FALSE;

	// 바로 WriteComplete 호출, EventSelect 의 경우 FD_WRITE 가 보내기 성공 이후 발생하지 않을 수도 있으므로 이렇게 체크한다.
	// IOCP 에서는 IO_WROTE 호출되어야 WriteComplete 하는데, EventSelect 는 이와 다르게 바로 호출해서 확인한다. (정말 데이터를 보낸 이후인지 확인이 안되기에..)
	if (!mSession.WriteComplete())
		return FALSE;

	return TRUE;
}

/// <summary>
/// UDP 데이터를 전송하는 함수이다.
/// </summary>
/// <param name="remoteAddress"></param>
/// <param name="remotePort"></param>
/// <param name="protocol"></param>
/// <param name="packet"></param>
/// <param name="packetLength"></param>
/// <returns></returns>
BOOL CClientSession::WriteToPacket(LPCSTR remoteAddress, USHORT remotePort, DWORD protocol, const BYTE* packet, DWORD packetLength)
{
	if (!mSession.WriteToPacket(remoteAddress, remotePort, protocol, packet, packetLength))
		return FALSE;

	if (!mSession.WriteComplete())
		return FALSE;

	return TRUE;
}


/// <summary>
/// 데이터가 발생했을 때에 호출되는 가상함수이다.
/// </summary>
/// <param name=""></param>
VOID CClientSession::OnIoRead(VOID)
{
	BYTE	Packet[MAX_BUFFER_LENGTH] = { 0, };
	DWORD	PacketLength = 0;

	DWORD	Protocol = 0;

	
	if (IsUdp) // UDP 를 사용하는 경우
	{
		// UDP 인 경우 보낸 주소와 포트 정보를 받아와야 하기 때문에 따로 변수를 선언한다.
		CHAR	RemoteAddress[32] = { 0, };
		USHORT	RemotePort = 0;

		// CNetworkSession 에서 CPacketSession 으로 데이터를 복사
		if (mSession.ReadFromPacketForEventSelect(RemoteAddress, RemotePort))
		{
			// 패킷을 읽어와서 큐에 저장한다.
			while (mSession.GetPacket(RemoteAddress, RemotePort, Protocol, Packet, PacketLength))
				mReadPacketQueue.Push(this, Protocol, Packet, PacketLength, RemoteAddress, RemotePort);
		}
	}
	else // TCP 를 사용하는 경우
	{
		// CNetworkSession 에서 CPacketSession 으로 데이터를 복사
		if (mSession.ReadPacketForEventSelect())
		{
			// 패킷을 읽어와서 큐에 저장한다.
			while (mSession.GetPacket(Protocol, Packet, PacketLength))
				mReadPacketQueue.Push(this, Protocol, Packet, PacketLength);
		}
	}
}