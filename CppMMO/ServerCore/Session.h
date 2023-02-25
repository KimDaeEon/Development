#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetworkAddress.h"

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

public:
						Session();
	virtual				~Session();

	// ���� ���� �Լ�
	shared_ptr<Service> GetService() { return _service.lock(); }
	void				SetSerivce(shared_ptr<Service> service) { _service = service; }
	void				SetNetworkAddress(NetworkAddress address) { _netAddress = address; }
	NetworkAddress		GetAddress() { return _netAddress; }
	SOCKET				GetSocket() { return _socket; }
	BYTE*				GetRecvBuffer() { return _recvBuffer; } // TEMP TODO: ���� ����
	bool				IsConnected() { return _connected; }
	SessionRef			GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }

	// �������̽� ����	
	virtual HANDLE		GetHandle() override;
	virtual void		Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

	// �ܺ� �ۼ��� ���� �Լ�
	void				Send(BYTE* buffer, int32 len);
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
	void				RegisterSend(SendEvent* sendEvent);

	void				ProcessConnect();
	void				ProcessDisconnect();
	void				ProecessDisconnect();
	void				ProcessRecv(int32 numOfBytes);
	void				ProcessSend(SendEvent* sendEvent, int32 numOfBytes);

	void				HandleError(int32 errorCode);
	
private:
	USE_LOCK;

	weak_ptr<Service>	_service; // ������ �� �ִٸ� �̰� �׻� �����״� ���� nullptr üũ�� ���ϰ� ���
	SOCKET				_socket = { INVALID_SOCKET };
	NetworkAddress		_netAddress = {};
	Atomic<bool>		_connected = false;

	BYTE				_recvBuffer[1000] = {}; // TEMP TODO: ���� ����

	// ���� ���� ���

	// �۽� ���� ���

	// IocpEvent ������ ���� ��� ������
	ConnectEvent		_connectEvent;
	DisconnectEvent		_disconnectEvent;
	RecvEvent			_recvEvent;

};

