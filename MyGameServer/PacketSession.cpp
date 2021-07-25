#include "PackeSession.h"
#include "Crypt.h"

CPacketSession::CPacketSession(VOID)
{
	memset(mPacketBuffer, 0, sizeof(mPacketBuffer));

	mRemainLength = 0;
	mCurrentPacketNumber = 0;

	mLastReadPacketNumber = 0;
}

CPacketSession::~CPacketSession(VOID)
{
}

BOOL CPacketSession::Begin(VOID)
{
	CThreadSync Sync;

	// CNetworkSession 에서 가져온 데이터를 담아둘 패킷 분석 버퍼를 초기화
	memset(mPacketBuffer, 0, sizeof(mPacketBuffer));

	mRemainLength			= 0;
	mCurrentPacketNumber	= 0;
	mLastReadPacketNumber	= 0;

	if (!WriteQueue.Begin())
		return FALSE;

	// 상속받은 CNetworkSession 객체를 초기화
	return CNetworkSession::Begin();
}

BOOL CPacketSession::End(VOID)
{
	CThreadSync Sync;

	// UDP 에서 사용하는 패킷정보 저장 벡터 초기화
	mLastReadPacketInfoVectorForUdp.clear();

	// 큐 종료
	if (!WriteQueue.End())
		return FALSE;
	
	// CNetwrokSession 종료
	return CNetworkSession::End();
}

/// <summary>
/// TCP 에서 받은 패킷을 분석하는 함수이다. PackeSession 클래스에서 가장 중요한 함수이다.
/// </summary>
/// <param name="protocol"></param>
/// <param name="packet"></param>
/// <param name="packetLength"></param>
/// <returns></returns>
BOOL CPacketSession::GetPacket(DWORD& protocol, BYTE* packet, DWORD& packetLength)
{
	CThreadSync Sync;

	if (!packet)
		return FALSE;

	// 4바이트 이상이 mPacketBuffer 에 없으면 처리하지 않는다.
	if (mRemainLength < sizeof(DWORD))		
		return FALSE;

	INT PacketLength = 0;
	// 패킷의 맨 앞 4바이트를 읽어서 길이로 할당한단. 패킷 구성에서 맨 앞 4바이트가 패킷 길이를 의미하기 때문이다.
	memcpy(&PacketLength, mPacketBuffer, sizeof(INT));

	// 패킷 길이가 최대치를 넘거나, 0이하인 경우 잘못된 패킷이므로 처리하지 않는다.
	if (PacketLength > MAX_BUFFER_LENGTH || PacketLength <= 0) // Invalid Packet
	{
		mRemainLength = 0;
		return FALSE;
	}

	// 읽은 패킷 길이가 버퍼에 있는 길이보다 작거나 같다는 것은, 1개 이상의 온전한 패킷이 들어있다는 뜻이고, 해당 패킷에 대한 처리를 해도 된다는 뜻이다.
	// TCP 에서는 Nagle 알고리즘 때문에 패킷이 끊어지거나 덜 올 수 있어서 이러한 과정이 필요하다고 한다.
	if (PacketLength <= mRemainLength)
	{
		// 패킷 길이 부분을 제외한, 패킷 길이 이하의 부분의 암호화를 해제
		CCrypt::Decrypt(mPacketBuffer + sizeof(DWORD)/*LENGTH*/,
			mPacketBuffer + sizeof(DWORD)/*LENGTH*/,
			PacketLength - sizeof(DWORD)/*LENGTH*/);

		DWORD PacketNumber = 0;
		DWORD Protocol = 0;

		// 프로토콜과 패킷 번호 알아온다.
		// 패킷 구조가 4바이트 패킷 길이, 4바이트 프로토콜, 4바이트 패킷 번호, 나머지 최대 4084 바이트가 Data 부분이다.
		memcpy(&PacketNumber, mPacketBuffer + sizeof(DWORD)/*LENGTH*/, sizeof(DWORD));
		memcpy(&Protocol, mPacketBuffer +
			sizeof(DWORD)/*LENGTH*/ +
			sizeof(DWORD)/*PACKETNUMBER*/, sizeof(DWORD));

		// 패러미터로 온 protocol 과 packetLength 에 패킷에서 꺼낸 정보를 할당
		protocol = Protocol;
		packetLength = PacketLength -
			sizeof(DWORD)/*LENGTH*/ -
			sizeof(DWORD)/*PACKETNUMBER*/ -
			sizeof(DWORD)/*PROTOCOL*/;

		memcpy(packet, mPacketBuffer +
			sizeof(DWORD)/*LENGTH*/ +
			sizeof(DWORD)/*PACKETNUMBER*/ +
			sizeof(DWORD)/*PROTOCOL*/, packetLength);

		// 한 패킷을 처리하고 남은 길이가 0보다 클 경우, 처리하고 남은 메모리를 앞으로 당겨준다.
		if (mRemainLength - PacketLength > 0)
			memmove(mPacketBuffer, mPacketBuffer + PacketLength, mRemainLength - PacketLength);

		// 처리한 후 남은 패킷 데이터 길이를 mRemainLength 에 반영한다.
		mRemainLength -= PacketLength;

		// 남은 길이가 0 이하면 패킷 버퍼 와 mRemainLength 를 초기화한다.
		if (mRemainLength <= 0)
		{
			mRemainLength = 0;
			memset(mPacketBuffer, 0, sizeof(mPacketBuffer));
		}

		// 패킷번호가 이전것이거나 같을 경우, 해당 패킷은 무시한다.
		if (PacketNumber <= mLastReadPacketNumber) 
			return FALSE;
		else
		{
			mLastReadPacketNumber = PacketNumber;

			return TRUE;
		}
	}

	return FALSE;
}

/// <summary>
/// UDP 에서 받은 패킷을 분석하는 함수이다.
/// </summary>
/// <param name="remoteAddress"></param>
/// <param name="remotePort"></param>
/// <param name="protocol"></param>
/// <param name="packet"></param>
/// <param name="packetLength"></param>
/// <returns></returns>
BOOL CPacketSession::GetPacket(LPSTR remoteAddress, USHORT remotePort, DWORD& protocol, BYTE* packet, DWORD& packetLength)
{
	CThreadSync Sync;

	if (!packet)
		return FALSE;

	if (mRemainLength < sizeof(DWORD))
		return FALSE;

	// 패킷 사이즈 할당
	INT PacketLength = 0;
	memcpy(&PacketLength, mPacketBuffer, sizeof(INT));

	// 패킷 사이즈가 버퍼보다 크거나 0 이하일 경우 잘못된 패킷이므로 무시
	if (PacketLength > MAX_BUFFER_LENGTH || PacketLength <= 0) // Invalid Packet
	{
		mRemainLength = 0;

		return FALSE;
	}

	// 1개 이상의 온전한 패킷이 있을 때에
	if (PacketLength <= mRemainLength)
	{
		// 패킷 사이즈 부분, 다음의 암호화된 부분을 복호화
		CCrypt::Decrypt(mPacketBuffer + sizeof(DWORD)/*LENGTH*/,
			mPacketBuffer + sizeof(DWORD)/*LENGTH*/,
			PacketLength - sizeof(DWORD)/*LENGTH*/);

		DWORD PacketNumber = 0;
		DWORD Protocol = 0;

		// 패킷 번호, 프로토콜 입력
		memcpy(&PacketNumber, mPacketBuffer + sizeof(DWORD)/*LENGTH*/, sizeof(DWORD));
		memcpy(&Protocol, mPacketBuffer +
			sizeof(DWORD)/*LENGTH*/ +
			sizeof(DWORD)/*PACKETNUMBER*/, sizeof(DWORD));

		// 패러미터에 읽은 값 할당
		protocol = Protocol;
		packetLength = PacketLength -
			sizeof(DWORD)/*LENGTH*/ -
			sizeof(DWORD)/*PACKETNUMBER*/ -
			sizeof(DWORD)/*PROTOCOL*/;

		// 패킷 데이터 패러미터에 할당
		memcpy(packet, mPacketBuffer +
			sizeof(DWORD)/*LENGTH*/ +
			sizeof(DWORD)/*PACKETNUMBER*/ +
			sizeof(DWORD)/*PROTOCOL*/, packetLength);

		// 패킷 데이터 읽은 후 남은 데이터들 당겨준다.
		if (mRemainLength - PacketLength > 0)
			memmove(mPacketBuffer, mPacketBuffer + PacketLength, mRemainLength - PacketLength);

		// 당긴만큼 남은 패킷 데이터 길이 감소
		mRemainLength -= PacketLength;

		// 패킷 데이터 남은 것이 없으므로 길이 및 패킷 버퍼 초기화
		if (mRemainLength <= 0)
		{
			mRemainLength = 0;
			memset(mPacketBuffer, 0, sizeof(mPacketBuffer));
		}

		// TCP 와 다른 부분이다.
		// mLastReadPacketInfoVectorForUdp 에서 데이터를 받은 적이 있는지 검사하고, 받은 적이 있는 정보에 대해 패킷 번호를 업데이트한다.
		// TCP 처럼 1:1로 연결되어서 패킷을 주고받는 것이 아니라서, 여러 주소에 대해 받는 패킷정보를 이렇게 추적해주어야 하는 것이다.
		for (DWORD i = 0; i < mLastReadPacketInfoVectorForUdp.size(); i++)
		{
			READ_PACKET_INFO ReadPacketInfo = mLastReadPacketInfoVectorForUdp[i];

			// 같은 주소 + 같은 포트의 정보가 이미 벡터에 들어있는지 확인
			if (!strcmp(ReadPacketInfo.RemoteAddress, remoteAddress) && ReadPacketInfo.RemotePort == remotePort)
			{
				// 같은 주소 + 같은 포트의 정보가 있고, 이 주소에 대해 받은 패킷 번호가 지금까지 보다 큰 번호라면 패킷 번호를 업데이트한다.
				if (ReadPacketInfo.PacketNumber < PacketNumber)
				{
					mLastReadPacketInfoVectorForUdp[i].PacketNumber = PacketNumber;

					return TRUE;
				}
				else
					return FALSE;
			}
		}

		// 받은 적이 없는 정보인 경우 정보를 세팅한 후에 벡터에 넣는다.
		READ_PACKET_INFO ReadPacketInfo;
		ReadPacketInfo.PacketNumber = PacketNumber;
		ReadPacketInfo.RemotePort = remotePort;
		strcpy(ReadPacketInfo.RemoteAddress, remoteAddress);

		mLastReadPacketInfoVectorForUdp.push_back(ReadPacketInfo);

		return TRUE;
	}

	return FALSE;
}

/// <summary>
/// TCP IOCP 용 CNetworkSession 에서 데이터를 받아오는 함수이다.
/// 이 함수는 FALSE가 떨어질때 까지 while문에서 돌린다. 읽을 수 있는 한 데이터를 끝까지 읽어온다는 의미이다.
/// GetPacket 이전에 실행한다.
/// </summary>
/// <param name="readLength"></param>
/// <returns></returns>
BOOL CPacketSession::ReadPacketForIocp(DWORD readLength)
{
	CThreadSync Sync;

	// CNetworkSession 을 통해서 mPacketBuffer 에 받은 데이터를 입력한다.
	if (!CNetworkSession::ReadForIocp(mPacketBuffer + mRemainLength, readLength))
		return FALSE;

	// mOacjetBuffer에 받은 만큼의 데이터 길이를 증가시킨다.
	mRemainLength += readLength;

	//return getPacket(protocol, packet, packetLength);
	return TRUE;
}

/// <summary>
/// TCP EventSelect 용 CNetworkSession 에서 데이터를 받아오는 함수이다.
/// 이 함수는 FALSE가 떨어질때 까지 while문에서 돌린다. 읽을 수 있는 한 데이터를 끝까지 읽어온다는 의미이다.
/// 위의 IOCP 용 함수와 구조적으로 유사하다.
/// </summary>
/// <param name=""></param>
/// <returns></returns>
BOOL CPacketSession::ReadPacketForEventSelect(VOID)
{
	CThreadSync Sync;

	DWORD ReadLength = 0;

	if (!CNetworkSession::ReadForEventSelect(mPacketBuffer + mRemainLength, ReadLength))
		return FALSE;

	mRemainLength += ReadLength;

	//return getPacket(protocol, packet, packetLength);
	return TRUE;
}

// ReadPacketForIocp는 FALSE가 떨어질때 까지 while문을 돌린다.

/// <summary>
/// UDP IOCP 용 CNetworkSession 에서 데이터를 받아오는 함수이다.
/// 이 함수는 FALSE가 떨어질때 까지 while문에서 돌린다. 읽을 수 있는 한 데이터를 끝까지 읽어온다는 의미이다.
/// </summary>
/// <param name="remoteAddress"></param>
/// <param name="remotePort"></param>
/// <param name="readLength"></param>
/// <returns></returns>
BOOL CPacketSession::ReadFromPacketForIocp(LPSTR remoteAddress, USHORT& remotePort, DWORD readLength)
{
	CThreadSync Sync;

	if (!CNetworkSession::ReadFromForIocp(remoteAddress, remotePort, mPacketBuffer + mRemainLength, readLength))
		return FALSE;

	mRemainLength += readLength;

	//return getPacket(remoteAddress, remotePort, protocol, packet, packetLength);
	return TRUE;
}

/// <summary>
/// UDP EventSelect 용 CNetworkSession 에서 데이터를 받아오는 함수이다.
/// 이 함수는 FALSE가 떨어질때 까지 while문에서 돌린다. 읽을 수 있는 한 데이터를 끝까지 읽어온다는 의미이다.
/// </summary>
/// <param name="remoteAddress"></param>
/// <param name="remotePort"></param>
/// <returns></returns>
BOOL CPacketSession::ReadFromPacketForEventSelect(LPSTR remoteAddress, USHORT& remotePort)
{
	CThreadSync Sync;

	DWORD ReadLength = 0;

	if (!CNetworkSession::ReadFromForEventSelect(remoteAddress, remotePort, mPacketBuffer + mRemainLength, ReadLength))
		return FALSE;

	mRemainLength += ReadLength;

	//return getPacket(remoteAddress, remotePort, protocol, packet, packetLength);
	return TRUE;
}

/// <summary>
/// TCP 패킷 보내는 함수이다.
/// </summary>
/// <param name="protocol"></param>
/// <param name="packet"></param>
/// <param name="packetLength"></param>
/// <returns></returns>
BOOL CPacketSession::WritePacket(DWORD protocol, const BYTE* packet, DWORD packetLength)
{
	CThreadSync Sync;

	if (!packet)
		return FALSE;

	// LENGTH(4) | PACKET_NUMBER(4) | PROTOCOL(4) | DATA (4084)
	// 패킷 전체 길이 = 패킷 길이(4바이트) + 패킷 고유 번호(4바이트) + 프로토콜(4바이트) + 데이터 길이(4084 이하의 값)
	DWORD PacketLength = sizeof(DWORD)/*LENGTH*/ +
		sizeof(DWORD)/*PACKETNUMBER*/ +
		sizeof(DWORD)/*PROTOCOL*/ +
		packetLength;

	// 패킷 길이가 최대 버퍼 길이보다 크면 실패
	if (PacketLength >= MAX_BUFFER_LENGTH)
		return FALSE;

	// 패킷 번호 1개 증가
	mCurrentPacketNumber++;

	// 패킷을 만들기 위한 임시 버퍼
	BYTE TempBuffer[MAX_BUFFER_LENGTH] = { 0, };

	// 패킷 만들기 시작
	// 패킷 길이 입력
	memcpy(TempBuffer, &PacketLength, sizeof(DWORD));

	// 패킷에 인덱스(번호)를 붙여 순서 보정을 해 주어야 한다.
	memcpy(TempBuffer +
		sizeof(DWORD)/*LENGTH*/,
		&mCurrentPacketNumber, sizeof(DWORD));

	// 프로토콜 입력
	memcpy(TempBuffer +
		sizeof(DWORD)/*LENGTH*/ +
		sizeof(DWORD)/*PACKETNUMBER*/,
		&protocol, sizeof(DWORD));

	// 패킷 데이터 입력
	memcpy(TempBuffer +
		sizeof(DWORD)/*LENGTH*/ +
		sizeof(DWORD)/*PACKETNUMBER*/ +
		sizeof(DWORD)/*PROTOCOL*/,
		packet, packetLength);

	// 입력한 데이터 암호화, 패킷 길이 부분은 제외하고 암호화한다.
	CCrypt::Encrypt(TempBuffer + sizeof(DWORD), TempBuffer + sizeof(DWORD), PacketLength - sizeof(DWORD));

	// WriteQueue를 이용해서 패킷이 전송 완료가 되었을 때까지 메모리를 살려둔다. 차후에 전송이 완료되면 WriteComplete 에서 Pop 해서 제거한다.
	// WriteQueue 내부에서 데이터를 복사해서 push 하고 그 복사된 데이터의 포인터를 넘겨주기에 이 데이터는 이 함수가 끝나도 사라지지 않는다.
	BYTE* WriteData = WriteQueue.Push(this, TempBuffer, PacketLength);

	return CNetworkSession::Write(WriteData, PacketLength);
}

BOOL CPacketSession::WriteToPacket(LPCSTR remoteAddress, USHORT remotePort, DWORD protocol, const BYTE* packet, DWORD packetLength)
{
	CThreadSync Sync;

	if (!packet)
		return FALSE;

	// LENGTH(4) | PACKET_NUMBER(4) | PROTOCOL(4) | DATA (4084)
	DWORD PacketLength = sizeof(DWORD)/*LENGTH*/ +
		sizeof(DWORD)/*PACKETNUMBER*/ +
		sizeof(DWORD)/*PROTOCOL*/ +
		packetLength;

	if (PacketLength >= MAX_BUFFER_LENGTH)
		return FALSE;

	mCurrentPacketNumber++;

	BYTE TempBuffer[MAX_BUFFER_LENGTH] = { 0, };

	//  패킷에 인덱스를 붙여 순서 보정을 해 주어야 한다.
	memcpy(TempBuffer, &PacketLength, sizeof(DWORD));

	memcpy(TempBuffer +
		sizeof(DWORD)/*LENGTH*/,
		&mCurrentPacketNumber, sizeof(DWORD));

	memcpy(TempBuffer +
		sizeof(DWORD)/*LENGTH*/ +
		sizeof(DWORD)/*PACKETNUMBER*/,
		&protocol, sizeof(DWORD));

	memcpy(TempBuffer +
		sizeof(DWORD)/*LENGTH*/ +
		sizeof(DWORD)/*PACKETNUMBER*/ +
		sizeof(DWORD)/*PROTOCOL*/,
		packet, packetLength);

	CCrypt::Encrypt(TempBuffer + sizeof(DWORD), TempBuffer + sizeof(DWORD), PacketLength - sizeof(DWORD));

	// WriteQueue를 이용해서 패킷이 전송 완료가 되었을까지 메모리를 살려둔다.
	BYTE* WriteData = WriteQueue.Push(this, TempBuffer, PacketLength);

	return CNetworkSession::WriteTo(remoteAddress, remotePort, WriteData, PacketLength);
}

/// <summary>
/// 보내기 완료 시 큐에서 데이터를 빼주는 함수. 그 외에 것은 하지 않는다.
/// </summary>
/// <param name=""></param>
/// <returns></returns>
BOOL CPacketSession::WriteComplete(VOID)
{
	CThreadSync Sync;

	// WriteQueue에서 Pop을 해 주면 된다.
	return WriteQueue.Pop();
}

BOOL CPacketSession::ResetUdp(VOID)
{
	CThreadSync	Sync;

	mLastReadPacketInfoVectorForUdp.clear();

	return TRUE;
}