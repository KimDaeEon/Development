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
	void WriteLock(const char* name);
	void WriteUnlock(const char* name);
	void ReadLock(const char* name);
	void ReadUnlock(const char* name);

private:
	Atomic<uint32> _lockFlag = EMPTY_FLAG;
	uint16 _writeCount = 0; // WriteLock�� ��� �ִ� ���¿��� �� ���� ���� �ִ�. �װ��� üũ�ϱ� ���� ����. ���� �����忡���� ����Ǳ⿡ atomic�� ������ �ʾҴ�.
};


// -----------------------
// LockGuards
// -----------------------
class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name)
	{
		_lock.ReadLock(_name);
	}

	~ReadLockGuard()
	{
		_lock.ReadUnlock(_name);
	}

private:
	Lock&			_lock;
	const char*		_name;
};


class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name)
	{
		_lock.WriteLock(_name);
	}

	~WriteLockGuard()
	{
		_lock.WriteUnlock(_name);
	}

private:
	Lock&			_lock;
	const char*		_name;
};
