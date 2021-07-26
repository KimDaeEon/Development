#pragma once
#include "Global.h"
#include "PacketSession.h"
#include "EventSelect.h"

class CClientSession : public CEventSelect
{
public:
	CClientSession(VOID);
	virtual ~CClientSession(VOID);

private:
	CPacketSession	mSession;				// 통신을 담당하는 세션 개체
	CCircularQueue	mReadPacketQueue;		// 받은 패킷을 저장하는 큐, ReadPacket 으로 패킷을 하나씩 받아간다.

	BOOL			IsUdp;					// 현재 UDP 로 사용 중인지 확인하는 변수

public:
	BOOL			BeginTcp(LPSTR remoteAddress, USHORT remotePort);
	BOOL			BeginUdp(USHORT remotePort);

	BOOL			End(VOID);

	BOOL			GetLocalIP(WCHAR* pIP);
	USHORT			GetLocalPort(VOID);

	BOOL			ReadPacket(DWORD& protocol, BYTE* packet, DWORD& packetLength);
	BOOL			ReadFromPacket(DWORD& protocol, LPSTR remoteAddress, USHORT& remotePort, BYTE* packet, DWORD& packetLength);

	BOOL			WritePacket(DWORD protocol, const BYTE* packet, DWORD packetLength);
	BOOL			WriteToPacket(LPCSTR remoteAddress, USHORT remotePort, DWORD protocol, const BYTE* packet, DWORD packetLength);

protected:
	VOID			OnIoRead(VOID);
};
