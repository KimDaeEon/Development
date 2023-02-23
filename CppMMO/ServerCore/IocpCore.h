#pragma once

// ---------------------------
//			IocpCore
// ---------------------------
class IocpCore
{
public:
	IocpCore();
	~IocpCore();

	HANDLE GetHandle() { return _iocpHandle; }

	bool Register(IocpObjectRef iocpObject);
	bool Dispatch(uint32 timeoutMs = INFINITE);
private:
	HANDLE _iocpHandle;
};


// ---------------------------
//		   IocpObject
// ---------------------------

// Iocp에서 등록해서 사용할 수 있는 object이다.
// 이 객체를 상속받아서 Iocp에서 사용한다.
class IocpObject : public enable_shared_from_this<IocpObject>
{
	// weak_ptr<IocpObject> _Wptr; // 이런 코드가 enable_shared_from_this를 하면 들어간다.
	// 이 코드를 통해 이 객체가 this 포인터에 대해서 shared_ptr을 쓸 수 있도록 해준다.
	// 이렇게 안하고 그냥 shared_ptr<IocpObject>(this) 이러면 control block이 2개 생기는 거라서 난리가 난다.
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) abstract;
};
