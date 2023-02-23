#pragma once
#include "IocpCore.h"
#include "NetworkAddress.h"

class AcceptEvent;
class ServerService;

// -----------------------------
//			 Listener
// -----------------------------
class Listener : public IocpObject
{
public:
	Listener() = default;
	~Listener();

	// ���� ��� �Լ�
	bool StartAccept(ServerServiceRef service);
	void CloseSocket();

	// �������̽� ����
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

protected:
	SOCKET					_listenSocket = { INVALID_SOCKET };
	myVector<AcceptEvent*>	_acceptEvents;
	ServerServiceRef		_service;

private:
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);
};

