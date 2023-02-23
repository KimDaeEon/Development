#include "pch.h"
#include "IocpCore.h"
#include "IocpEvent.h"

IocpCore::IocpCore()
{
	_iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	ASSERT_CRASH(_iocpHandle != INVALID_HANDLE_VALUE);
}

IocpCore::~IocpCore()
{
	::CloseHandle(_iocpHandle);
}

bool IocpCore::Register(IocpObjectRef iocpObject)
{
	// key는 이제 사용하지 않고, IocpEvent 객체에 관리 객체를 들고 있도록 한다.
	// 이렇게 한 이유는 IocpEvent 처리되지 않은 동안에는 IocpObject 객체의 수명을 유지하기 위함이다.
	return ::CreateIoCompletionPort(iocpObject->GetHandle(), _iocpHandle, /*key*/ 0, 0);
}

bool IocpCore::Dispatch(uint32 timeoutMs)
{
	DWORD numOfBytes = 0;
	ULONG_PTR key = 0; // key는 이제 사용하지 않고, IocpEvent 객체에 관리 객체를 들고 있도록 한다.
	IocpEvent* iocpEvent = nullptr;

	if (::GetQueuedCompletionStatus(_iocpHandle, OUT & numOfBytes,
		OUT &key,
		OUT reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), timeoutMs))
	{
		IocpObjectRef iocpObject = iocpEvent->owner;
		// Iocp 일감 들어온 것 처리, iocpEvent 객체마다 처리가 다르다.
		iocpObject->Dispatch(iocpEvent, numOfBytes);
	}
	else
	{
		int32 errCode = ::WSAGetLastError();
		switch (errCode)
		{
		case WAIT_TIMEOUT:
			return false;

		default:
			// TODO: 로그 찍기
			IocpObjectRef iocpObject = iocpEvent->owner;
			iocpObject->Dispatch(iocpEvent, numOfBytes);
			break;
		}
	}

	return true;
}
