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

	// CNetworkSession ���� ������ �����͸� ��Ƶ� ��Ŷ �м� ���۸� �ʱ�ȭ
	memset(mPacketBuffer, 0, sizeof(mPacketBuffer));

	mRemainLength			= 0;
	mCurrentPacketNumber	= 0;
	mLastReadPacketNumber	= 0;

	if (!WriteQueue.Begin())
		return FALSE;

	// ��ӹ��� CNetworkSession ��ü�� �ʱ�ȭ
	return CNetworkSession::Begin();
}

BOOL CPacketSession::End(VOID)
{
	CThreadSync Sync;

	// UDP ���� ����ϴ� ��Ŷ���� ���� ���� �ʱ�ȭ
	mLastReadPacketInfoVectorForUdp.clear();

	// ť ����
	if (!WriteQueue.End())
		return FALSE;
	
	// CNetwrokSession ����
	return CNetworkSession::End();
}

/// <summary>
/// TCP ���� ���� ��Ŷ�� �м��ϴ� �Լ��̴�. PackeSession Ŭ�������� ���� �߿��� �Լ��̴�.
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

	// 4����Ʈ �̻��� mPacketBuffer �� ������ ó������ �ʴ´�.
	if (mRemainLength < sizeof(DWORD))		
		return FALSE;

	INT PacketLength = 0;
	// ��Ŷ�� �� �� 4����Ʈ�� �о ���̷� �Ҵ��Ѵ�. ��Ŷ �������� �� �� 4����Ʈ�� ��Ŷ ���̸� �ǹ��ϱ� �����̴�.
	memcpy(&PacketLength, mPacketBuffer, sizeof(INT));

	// ��Ŷ ���̰� �ִ�ġ�� �Ѱų�, 0������ ��� �߸��� ��Ŷ�̹Ƿ� ó������ �ʴ´�.
	if (PacketLength > MAX_BUFFER_LENGTH || PacketLength <= 0) // Invalid Packet
	{
		mRemainLength = 0;
		return FALSE;
	}

	// ���� ��Ŷ ���̰� ���ۿ� �ִ� ���̺��� �۰ų� ���ٴ� ����, 1�� �̻��� ������ ��Ŷ�� ����ִٴ� ���̰�, �ش� ��Ŷ�� ���� ó���� �ص� �ȴٴ� ���̴�.
	// TCP ������ Nagle �˰��� ������ ��Ŷ�� �������ų� �� �� �� �־ �̷��� ������ �ʿ��ϴٰ� �Ѵ�.
	if (PacketLength <= mRemainLength)
	{
		// ��Ŷ ���� �κ��� ������, ��Ŷ ���� ������ �κ��� ��ȣȭ�� ����
		CCrypt::Decrypt(mPacketBuffer + sizeof(DWORD)/*LENGTH*/,
			mPacketBuffer + sizeof(DWORD)/*LENGTH*/,
			PacketLength - sizeof(DWORD)/*LENGTH*/);

		DWORD PacketNumber = 0;
		DWORD Protocol = 0;

		// �������ݰ� ��Ŷ ��ȣ �˾ƿ´�.
		// ��Ŷ ������ 4����Ʈ ��Ŷ ����, 4����Ʈ ��������, 4����Ʈ ��Ŷ ��ȣ, ������ �ִ� 4084 ����Ʈ�� Data �κ��̴�.
		memcpy(&PacketNumber, mPacketBuffer + sizeof(DWORD)/*LENGTH*/, sizeof(DWORD));
		memcpy(&Protocol, mPacketBuffer +
			sizeof(DWORD)/*LENGTH*/ +
			sizeof(DWORD)/*PACKETNUMBER*/, sizeof(DWORD));

		// �з����ͷ� �� protocol �� packetLength �� ��Ŷ���� ���� ������ �Ҵ�
		protocol = Protocol;
		packetLength = PacketLength -
			sizeof(DWORD)/*LENGTH*/ -
			sizeof(DWORD)/*PACKETNUMBER*/ -
			sizeof(DWORD)/*PROTOCOL*/;

		memcpy(packet, mPacketBuffer +
			sizeof(DWORD)/*LENGTH*/ +
			sizeof(DWORD)/*PACKETNUMBER*/ +
			sizeof(DWORD)/*PROTOCOL*/, packetLength);

		// �� ��Ŷ�� ó���ϰ� ���� ���̰� 0���� Ŭ ���, ó���ϰ� ���� �޸𸮸� ������ ����ش�.
		if (mRemainLength - PacketLength > 0)
			memmove(mPacketBuffer, mPacketBuffer + PacketLength, mRemainLength - PacketLength);

		// ó���� �� ���� ��Ŷ ������ ���̸� mRemainLength �� �ݿ��Ѵ�.
		mRemainLength -= PacketLength;

		// ���� ���̰� 0 ���ϸ� ��Ŷ ���� �� mRemainLength �� �ʱ�ȭ�Ѵ�.
		if (mRemainLength <= 0)
		{
			mRemainLength = 0;
			memset(mPacketBuffer, 0, sizeof(mPacketBuffer));
		}

		// ��Ŷ��ȣ�� �������̰ų� ���� ���, �ش� ��Ŷ�� �����Ѵ�.
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
/// UDP ���� ���� ��Ŷ�� �м��ϴ� �Լ��̴�.
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

	// ��Ŷ ������ �Ҵ�
	INT PacketLength = 0;
	memcpy(&PacketLength, mPacketBuffer, sizeof(INT));

	// ��Ŷ ����� ���ۺ��� ũ�ų� 0 ������ ��� �߸��� ��Ŷ�̹Ƿ� ����
	if (PacketLength > MAX_BUFFER_LENGTH || PacketLength <= 0) // Invalid Packet
	{
		mRemainLength = 0;

		return FALSE;
	}

	// 1�� �̻��� ������ ��Ŷ�� ���� ����
	if (PacketLength <= mRemainLength)
	{
		// ��Ŷ ������ �κ�, ������ ��ȣȭ�� �κ��� ��ȣȭ
		CCrypt::Decrypt(mPacketBuffer + sizeof(DWORD)/*LENGTH*/,
			mPacketBuffer + sizeof(DWORD)/*LENGTH*/,
			PacketLength - sizeof(DWORD)/*LENGTH*/);

		DWORD PacketNumber = 0;
		DWORD Protocol = 0;

		// ��Ŷ ��ȣ, �������� �Է�
		memcpy(&PacketNumber, mPacketBuffer + sizeof(DWORD)/*LENGTH*/, sizeof(DWORD));
		memcpy(&Protocol, mPacketBuffer +
			sizeof(DWORD)/*LENGTH*/ +
			sizeof(DWORD)/*PACKETNUMBER*/, sizeof(DWORD));

		// �з����Ϳ� ���� �� �Ҵ�
		protocol = Protocol;
		packetLength = PacketLength -
			sizeof(DWORD)/*LENGTH*/ -
			sizeof(DWORD)/*PACKETNUMBER*/ -
			sizeof(DWORD)/*PROTOCOL*/;

		// ��Ŷ ������ �з����Ϳ� �Ҵ�
		memcpy(packet, mPacketBuffer +
			sizeof(DWORD)/*LENGTH*/ +
			sizeof(DWORD)/*PACKETNUMBER*/ +
			sizeof(DWORD)/*PROTOCOL*/, packetLength);

		// ��Ŷ ������ ���� �� ���� �����͵� ����ش�.
		if (mRemainLength - PacketLength > 0)
			memmove(mPacketBuffer, mPacketBuffer + PacketLength, mRemainLength - PacketLength);

		// ��丸ŭ ���� ��Ŷ ������ ���� ����
		mRemainLength -= PacketLength;

		// ��Ŷ ������ ���� ���� �����Ƿ� ���� �� ��Ŷ ���� �ʱ�ȭ
		if (mRemainLength <= 0)
		{
			mRemainLength = 0;
			memset(mPacketBuffer, 0, sizeof(mPacketBuffer));
		}

		// TCP �� �ٸ� �κ��̴�.
		// mLastReadPacketInfoVectorForUdp ���� �����͸� ���� ���� �ִ��� �˻��ϰ�, ���� ���� �ִ� ������ ���� ��Ŷ ��ȣ�� ������Ʈ�Ѵ�.
		// TCP ó�� 1:1�� ����Ǿ ��Ŷ�� �ְ�޴� ���� �ƴ϶�, ���� �ּҿ� ���� �޴� ��Ŷ������ �̷��� �������־�� �ϴ� ���̴�.
		for (DWORD i = 0; i < mLastReadPacketInfoVectorForUdp.size(); i++)
		{
			READ_PACKET_INFO ReadPacketInfo = mLastReadPacketInfoVectorForUdp[i];

			// ���� �ּ� + ���� ��Ʈ�� ������ �̹� ���Ϳ� ����ִ��� Ȯ��
			if (!strcmp(ReadPacketInfo.RemoteAddress, remoteAddress) && ReadPacketInfo.RemotePort == remotePort)
			{
				// ���� �ּ� + ���� ��Ʈ�� ������ �ְ�, �� �ּҿ� ���� ���� ��Ŷ ��ȣ�� ���ݱ��� ���� ū ��ȣ��� ��Ŷ ��ȣ�� ������Ʈ�Ѵ�.
				if (ReadPacketInfo.PacketNumber < PacketNumber)
				{
					mLastReadPacketInfoVectorForUdp[i].PacketNumber = PacketNumber;

					return TRUE;
				}
				else
					return FALSE;
			}
		}

		// ���� ���� ���� ������ ��� ������ ������ �Ŀ� ���Ϳ� �ִ´�.
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
/// TCP IOCP �� CNetworkSession ���� �����͸� �޾ƿ��� �Լ��̴�.
/// �� �Լ��� FALSE�� �������� ���� while������ ������. ���� �� �ִ� �� �����͸� ������ �о�´ٴ� �ǹ��̴�.
/// GetPacket ������ �����Ѵ�.
/// </summary>
/// <param name="readLength"></param>
/// <returns></returns>
BOOL CPacketSession::ReadPacketForIocp(DWORD readLength)
{
	CThreadSync Sync;

	// CNetworkSession �� ���ؼ� mPacketBuffer �� ���� �����͸� �Է��Ѵ�.
	if (!CNetworkSession::ReadForIocp(mPacketBuffer + mRemainLength, readLength))
		return FALSE;

	// mOacjetBuffer�� ���� ��ŭ�� ������ ���̸� ������Ų��.
	mRemainLength += readLength;

	//return getPacket(protocol, packet, packetLength);
	return TRUE;
}

/// <summary>
/// TCP EventSelect �� CNetworkSession ���� �����͸� �޾ƿ��� �Լ��̴�.
/// �� �Լ��� FALSE�� �������� ���� while������ ������. ���� �� �ִ� �� �����͸� ������ �о�´ٴ� �ǹ��̴�.
/// ���� IOCP �� �Լ��� ���������� �����ϴ�.
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

// ReadPacketForIocp�� FALSE�� �������� ���� while���� ������.

/// <summary>
/// UDP IOCP �� CNetworkSession ���� �����͸� �޾ƿ��� �Լ��̴�.
/// �� �Լ��� FALSE�� �������� ���� while������ ������. ���� �� �ִ� �� �����͸� ������ �о�´ٴ� �ǹ��̴�.
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
/// UDP EventSelect �� CNetworkSession ���� �����͸� �޾ƿ��� �Լ��̴�.
/// �� �Լ��� FALSE�� �������� ���� while������ ������. ���� �� �ִ� �� �����͸� ������ �о�´ٴ� �ǹ��̴�.
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
/// TCP ��Ŷ ������ �Լ��̴�.
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
	// ��Ŷ ��ü ���� = ��Ŷ ����(4����Ʈ) + ��Ŷ ���� ��ȣ(4����Ʈ) + ��������(4����Ʈ) + ������ ����(4084 ������ ��)
	DWORD PacketLength = sizeof(DWORD)/*LENGTH*/ +
		sizeof(DWORD)/*PACKETNUMBER*/ +
		sizeof(DWORD)/*PROTOCOL*/ +
		packetLength;

	// ��Ŷ ���̰� �ִ� ���� ���̺��� ũ�� ����
	if (PacketLength >= MAX_BUFFER_LENGTH)
		return FALSE;

	// ��Ŷ ��ȣ 1�� ����
	mCurrentPacketNumber++;

	// ��Ŷ�� ����� ���� �ӽ� ����
	BYTE TempBuffer[MAX_BUFFER_LENGTH] = { 0, };

	// ��Ŷ ����� ����
	// ��Ŷ ���� �Է�
	memcpy(TempBuffer, &PacketLength, sizeof(DWORD));

	// ��Ŷ�� �ε���(��ȣ)�� �ٿ� ���� ������ �� �־�� �Ѵ�.
	memcpy(TempBuffer +
		sizeof(DWORD)/*LENGTH*/,
		&mCurrentPacketNumber, sizeof(DWORD));

	// �������� �Է�
	memcpy(TempBuffer +
		sizeof(DWORD)/*LENGTH*/ +
		sizeof(DWORD)/*PACKETNUMBER*/,
		&protocol, sizeof(DWORD));

	// ��Ŷ ������ �Է�
	memcpy(TempBuffer +
		sizeof(DWORD)/*LENGTH*/ +
		sizeof(DWORD)/*PACKETNUMBER*/ +
		sizeof(DWORD)/*PROTOCOL*/,
		packet, packetLength);

	// �Է��� ������ ��ȣȭ, ��Ŷ ���� �κ��� �����ϰ� ��ȣȭ�Ѵ�.
	CCrypt::Encrypt(TempBuffer + sizeof(DWORD), TempBuffer + sizeof(DWORD), PacketLength - sizeof(DWORD));

	// WriteQueue�� �̿��ؼ� ��Ŷ�� ���� �Ϸᰡ �Ǿ��� ������ �޸𸮸� ����д�. ���Ŀ� ������ �Ϸ�Ǹ� WriteComplete ���� Pop �ؼ� �����Ѵ�.
	// WriteQueue ���ο��� �����͸� �����ؼ� push �ϰ� �� ����� �������� �����͸� �Ѱ��ֱ⿡ �� �����ʹ� �� �Լ��� ������ ������� �ʴ´�.
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

	//  ��Ŷ�� �ε����� �ٿ� ���� ������ �� �־�� �Ѵ�.
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

	// WriteQueue�� �̿��ؼ� ��Ŷ�� ���� �Ϸᰡ �Ǿ������� �޸𸮸� ����д�.
	BYTE* WriteData = WriteQueue.Push(this, TempBuffer, PacketLength);

	return CNetworkSession::WriteTo(remoteAddress, remotePort, WriteData, PacketLength);
}

/// <summary>
/// ������ �Ϸ� �� ť���� �����͸� ���ִ� �Լ�. �� �ܿ� ���� ���� �ʴ´�.
/// </summary>
/// <param name=""></param>
/// <returns></returns>
BOOL CPacketSession::WriteComplete(VOID)
{
	CThreadSync Sync;

	// WriteQueue���� Pop�� �� �ָ� �ȴ�.
	return WriteQueue.Pop();
}

BOOL CPacketSession::ResetUdp(VOID)
{
	CThreadSync	Sync;

	mLastReadPacketInfoVectorForUdp.clear();

	return TRUE;
}