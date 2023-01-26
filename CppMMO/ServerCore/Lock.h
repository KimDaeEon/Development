#pragma once

// -----------------------
// Reader-Writer SpinLock
// -----------------------

// Reader-Writer lock ���� ���� �б⸸ �ִٸ� ���� �����尡 �����ϰ�,
// ���� �۾��� �� ������ ��Ÿ������ �����Ѵ�.
// �Ʒ�ó�� 32��Ʈ�� 16��Ʈ�� �ɰ��� ����Ѵ�.
// [WWWWWWWW][WWWWWWWW][RRRRRRRR][RRRRRRRR]
// W : WriteFlag (���� lock�� ������ ThreadId)
// R : ReadFlag (���� Read�ϰ� �ִ� ������ ����)

// TODO: �Ʒ� ���� ������ �ڵ� �����Ǵ°� ���鼭 ��Ȯ�� �ʿ�
// Lock ��å
// Write -> Write ������ lock ��� �� ����
// Write -> Read ������ lock�� ��� �� ����
// Read -> Write ������ lock�� ��� �� �Ұ���, �̷��� ���� �Ŷ�� WriteLock�� ���� ��ƾ� �Ѵ�.
class Lock
{
	enum : uint32
	{
		ACQUIRE_TIMEOUT_TICK = 10000,
		MAX_SPIN_COUNT = 5000,
		WRITE_THREAD_ID_MASK = 0xFFFF'0000,
		READ_THREAD_COUNT_MASK = 0x0000'FFFF,
		EMPTY_FLAG = 0x0000'0000
	};

public:
	void WriteLock();
	void WriteUnlock();
	void ReadLock();
	void ReadUnlock();

private:
	Atomic<uint32> _lockFlag = EMPTY_FLAG;
	uint16 _writeCount = 0; // // WriteLock�� ��� �ִ� ���¿��� �� ���� ���� �ִ�. �װ��� üũ�ϱ� ���� ����
};


// -----------------------
// LockGuards
// -----------------------
class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock) : _lock(lock)
	{
		_lock.ReadLock();
	}

	~ReadLockGuard()
	{
		_lock.ReadUnlock();
	}

private:
	Lock& _lock;
};


class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock) : _lock(lock)
	{
		_lock.WriteLock();
	}

	~WriteLockGuard()
	{
		_lock.WriteUnlock();
	}

private:
	Lock& _lock;
};
