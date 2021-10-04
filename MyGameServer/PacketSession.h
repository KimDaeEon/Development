#pragma once

typedef struct _READ_PACKET_INFO
{
	CHAR	RemoteAddress[14];
	USHORT	RemotePort;
	DWORD	PacketNumber;
} READ_PACKET_INFO;

class CPacketSession : public CNetworkSession
{
public:
	CPacketSession(VOID);
	virtual ~CPacketSession(VOID);

private:
	BYTE								mPacketBuffer[MAX_BUFFER_LENGTH * 3];	// NetworkSession 에서 받은 데이터를 패킷 확인을 위해 저장하는 버퍼
	INT									mRemainLength;							// mPacketBuffer 에 남은 데이터 길이
	DWORD								mCurrentPacketNumber;					// 현재 상대에게 보내는 패킷 번호

	DWORD								mLastReadPacketNumber;					// 마지막으로 받은 패킷 번호, 이를 통해 기존에 보냈던 패킷인지 검증

	CCircularQueue						WriteQueue;

	//std::map<std::string, DWORD>		mLastReadPacketInfoMapForUdp;
	std::vector<READ_PACKET_INFO>		mLastReadPacketInfoVectorForUdp;		// UDP 에서 사용할 받은 패킷정보를 저장하는 벡터 
																				// TCP 와 다르게 UDP 에서는 한 개체가 여러 접속을 관리하기 때문이다.

public:
	BOOL	Begin(VOID);
	BOOL	End(VOID);

	BOOL	ReadPacketForIocp(DWORD readLength);
	BOOL	ReadPacketForEventSelect(VOID);

	BOOL	ReadFromPacketForIocp(LPSTR remoteAddress, USHORT& remotePort, DWORD readLength);
	BOOL	ReadFromPacketForEventSelect(LPSTR remoteAddress, USHORT& remotePort);

	BOOL	WritePacket(DWORD protocol, const BYTE* packet, DWORD packetLength);
	BOOL	WriteToPacket(LPCSTR remoteAddress, USHORT remotePort, DWORD protocol, const BYTE* packet, DWORD packetLength);
	BOOL	WriteComplete(VOID);

	BOOL	ResetUdp(VOID);

	BOOL	GetPacket(DWORD& protocol, BYTE* packet, DWORD& packetLength);
	BOOL	GetPacket(LPSTR remoteAddress, USHORT remotePort, DWORD& protocol, BYTE* packet, DWORD& packetLength);
};
