#include "stdafx.h"
#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "CircularQueue.h"
#include "NetworkSession.h"
#include "PacketSession.h"
#include "EventSelect.h"
#include "ClientSession.h"

/// <summary>
/// TCP, UDP ��� ����� ��� �ִ� ���� Ŭ������ �������̴�. EventSelect Ŭ������ ��ӹ޾Ҵ�.
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
/// TCP �� ����� ���� ����ϴ� �Լ��̴�. ������ ��� �ּҿ� ��Ʈ�� �Ķ���ͷ� �Է��Ѵ�.
/// </summary>
/// <param name="remoteAddress"></param>
/// <param name="remotePort"></param>
/// <returns></returns>
BOOL CClientSession::BeginTcp(LPSTR remoteAddress, USHORT remotePort)
{
	if (!remoteAddress || remotePort <= 0)
		return FALSE;

	// ����� ���� ��ü �ʱ�ȭ
	if (!mSession.Begin())
	{
		End();

		return FALSE;
	}

	// TCP �� ����ϱ� ���� ���ε� �ǽ�
	if (!mSession.TcpBind())
	{
		End();

		return FALSE;
	}

	// EventSelect ������� �ش� ������ �̺�Ʈ�� �޾ƿ��� ���� ����Ѵ�.
	if (!CEventSelect::Begin(mSession.GetSocket()))
	{
		End();

		return FALSE;
	}

	// ������ �ּҿ� ����
	if (!mSession.Connect(remoteAddress, remotePort))
	{
		End();

		return FALSE;
	}

	return TRUE;
}

/// <summary>
/// UDP �� ����� ���� �ʱ�ȭ�ϴ� �Լ��̴�. TCP �� �޸� �����(���ε�) ��Ʈ�� �Է��Ѵ�.
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

	// UDP �� ���ε�
	if (!mSession.UdpBind(remotePort))
	{
		End();

		return FALSE;
	}

	// EventSelect �� ���
	if (!CEventSelect::Begin(mSession.GetSocket()))
	{
		End();

		return FALSE;
	}

	// UDP �� ����ϴ� �� üũ
	IsUdp = TRUE;

	return TRUE;
}

BOOL CClientSession::End(VOID)
{
	// ���� ����
	mSession.End();

	// �̺�Ʈ ����Ʈ ��ü ����
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
/// ��Ŷ 1 ���� �޾ƿ��� �Լ��̴�.
/// </summary>
/// <param name="protocol"></param>
/// <param name="packet"></param>
/// <param name="packetLength"></param>
/// <returns></returns>
BOOL CClientSession::ReadPacket(DWORD& protocol, BYTE* packet, DWORD& packetLength)
{
	VOID* Object = NULL;

	// ť���� ��Ŷ �� ���� �޾ƿ´�.
	return mReadPacketQueue.Pop(&Object, protocol, packet, packetLength);
}

BOOL CClientSession::ReadFromPacket(DWORD& protocol, LPSTR remoteAddress, USHORT& remotePort, BYTE* packet, DWORD& packetLength)
{
	VOID* Object = NULL;

	return mReadPacketQueue.Pop(&Object, protocol, packet, packetLength, remoteAddress, remotePort);
}

/// <summary>
/// TCP �����͸� �����ϴ� �Լ��̴�.
/// </summary>
/// <param name="protocol"></param>
/// <param name="packet"></param>
/// <param name="packetLength"></param>
/// <returns></returns>
BOOL CClientSession::WritePacket(DWORD protocol, const BYTE* packet, DWORD packetLength)
{
	// ������ ���� �Լ� ȣ��
	if (!mSession.WritePacket(protocol, packet, packetLength))
		return FALSE;

	// �ٷ� WriteComplete ȣ��, EventSelect �� ��� FD_WRITE �� ������ ���� ���� �߻����� ���� ���� �����Ƿ� �̷��� üũ�Ѵ�.
	// IOCP ������ IO_WROTE ȣ��Ǿ�� WriteComplete �ϴµ�, EventSelect �� �̿� �ٸ��� �ٷ� ȣ���ؼ� Ȯ���Ѵ�. (���� �����͸� ���� �������� Ȯ���� �ȵǱ⿡..)
	if (!mSession.WriteComplete())
		return FALSE;

	return TRUE;
}

/// <summary>
/// UDP �����͸� �����ϴ� �Լ��̴�.
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
/// �����Ͱ� �߻����� ���� ȣ��Ǵ� �����Լ��̴�.
/// </summary>
/// <param name=""></param>
VOID CClientSession::OnIoRead(VOID)
{
	BYTE	Packet[MAX_BUFFER_LENGTH] = { 0, };
	DWORD	PacketLength = 0;

	DWORD	Protocol = 0;

	
	if (IsUdp) // UDP �� ����ϴ� ���
	{
		// UDP �� ��� ���� �ּҿ� ��Ʈ ������ �޾ƿ;� �ϱ� ������ ���� ������ �����Ѵ�.
		CHAR	RemoteAddress[32] = { 0, };
		USHORT	RemotePort = 0;

		// CNetworkSession ���� CPacketSession ���� �����͸� ����
		if (mSession.ReadFromPacketForEventSelect(RemoteAddress, RemotePort))
		{
			// ��Ŷ�� �о�ͼ� ť�� �����Ѵ�.
			while (mSession.GetPacket(RemoteAddress, RemotePort, Protocol, Packet, PacketLength))
				mReadPacketQueue.Push(this, Protocol, Packet, PacketLength, RemoteAddress, RemotePort);
		}
	}
	else // TCP �� ����ϴ� ���
	{
		// CNetworkSession ���� CPacketSession ���� �����͸� ����
		if (mSession.ReadPacketForEventSelect())
		{
			// ��Ŷ�� �о�ͼ� ť�� �����Ѵ�.
			while (mSession.GetPacket(Protocol, Packet, PacketLength))
				mReadPacketQueue.Push(this, Protocol, Packet, PacketLength);
		}
	}
}