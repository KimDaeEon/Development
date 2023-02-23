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

// Iocp���� ����ؼ� ����� �� �ִ� object�̴�.
// �� ��ü�� ��ӹ޾Ƽ� Iocp���� ����Ѵ�.
class IocpObject : public enable_shared_from_this<IocpObject>
{
	// weak_ptr<IocpObject> _Wptr; // �̷� �ڵ尡 enable_shared_from_this�� �ϸ� ����.
	// �� �ڵ带 ���� �� ��ü�� this �����Ϳ� ���ؼ� shared_ptr�� �� �� �ֵ��� ���ش�.
	// �̷��� ���ϰ� �׳� shared_ptr<IocpObject>(this) �̷��� control block�� 2�� ����� �Ŷ� ������ ����.
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) abstract;
};
