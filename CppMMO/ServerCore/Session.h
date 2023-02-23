#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetworkAddress.h"

// --------------------------------
//			   Session
// --------------------------------
class Session : public IocpObject
{
public:
	Session();
	virtual ~Session();

	// 관련 기능 함수
	void			SetNetworkAddress(NetworkAddress address) { _netAddress = address; }
	NetworkAddress	GetAddress() { return _netAddress; }
	SOCKET			GetSocket() { return _socket; }
	char*			GetRecvBuffer() { return _recvBuffer; } // TEMP TODO: 추후 변경

	// 인터페이스 구현
	virtual HANDLE	GetHandle() override;
	virtual void	Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	SOCKET			_socket = { INVALID_SOCKET };
	NetworkAddress	_netAddress = {};
	Atomic<bool>	_connected = false;

	char _recvBuffer[1000] = {}; // TEMP TODO: 추후 변경
};

