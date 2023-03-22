#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetworkAddress.h"
#include "RecvBuffer.h"

class Service;

// --------------------------------
//			   Session
// --------------------------------
class Session : public IocpObject
{
	// �Ʒ��� Ŭ�����鿡���� Session ���� ���� ���
	friend class Listener;
	friend class IocpCore;
	friend class Service;

	enum
	{
		BUFFER_SIZE = 0x10000, // 64kb
	};

public:
	Session();
	virtual				~Session();

	// ���� ���� �Լ�
	shared_ptr<Service> GetService() { return _service.lock(); }
	void				SetSerivce(shared_ptr<Service> service) { _service = service; }
	void				SetNetworkAddress(NetworkAddress address) { _netAddress = address; }
	NetworkAddress		GetAddress() { return _netAddress; }
	SOCKET				GetSocket() { return _socket; }
	bool				IsConnected() { return _connected; }
	SessionRef			GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }

	// �������̽� ����	
	virtual HANDLE		GetHandle() override;
	virtual void		Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

	// �ܺ� �ۼ��� ���� �Լ�
	void				Send(SendBufferRef sendBuffer);
	bool				Connect();
	void				Disconnect(const WCHAR* cause);

protected:
	// ������ �ڵ忡�� �ش� �Լ����� �������̵��ؼ� ���, � ������ �����̳Ŀ� ���� �ٸ� ������ �Ѵ�.
	virtual void		OnConnected() {}
	virtual int32		OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void		OnSend(int32 len) {}
	virtual void		OnDisconnected() {}

private:
	// ���� �ۼ��� ���� �Լ�
	bool				RegisterConnect();
	bool				RegisterDisconnect();
	void				RegisterRecv();
	void				RegisterSend();

	void				ProcessConnect();
	void				ProcessDisconnect();
	void				ProecessDisconnect();
	void				ProcessRecv(int32 numOfBytes);
	void				ProcessSend(int32 numOfBytes);

	void				HandleError(int32 errorCode);

private:
	USE_LOCK;

	weak_ptr<Service>		_service; // ������ �� �ִٸ� �̰� �׻� �����״� ���� nullptr üũ�� ���ϰ� ���
	SOCKET					_socket = { INVALID_SOCKET };
	NetworkAddress			_netAddress = {};
	Atomic<bool>			_connected = false;

	// ���� ����
	RecvBuffer				_recvBuffer; // TEMP TODO: ���� ����

	// �۽� ����
	myQueue<SendBufferRef>	_sendQueue; // SendEvent ó���� �ȵǾ����� ���⿡ ���� ��Ŷ���� ���δ�. // TODO: LockFreeQueue�� ���� �� �� �� ���� �� ����
	Atomic<bool>			_isSendRegistered; // SendEvent�� ó���� �� ��Ȳ���� üũ

	// IocpEvent ������ ���� ��� ������
	ConnectEvent			_connectEvent;
	DisconnectEvent			_disconnectEvent;
	RecvEvent				_recvEvent;
	SendEvent				_sendEvent;

};


// ---------------------------------
//			PacketSession
// ---------------------------------
struct PacketHeader
{
	uint16 size;
	uint16 id; // Protocl ID
};

class PacketSession : public Session
{
public:
	PacketSession();
	virtual ~PacketSession();

	PacketSessionRef	GetPacketSessionRef() { return static_pointer_cast<PacketSession>(shared_from_this()); }

protected:
	virtual int32		OnRecv(BYTE* buffer, int32 len) sealed; // OnRecv�� ������ �Ұ���, OnRecvPacket�� ������ ����
	virtual void		OnRecvPacket(BYTE* buffer, int32 len) abstract;

};