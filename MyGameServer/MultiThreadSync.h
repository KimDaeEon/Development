#pragma once

template<class T> // ��� ���� ���⼭ ���ø��� ���� �ʾƵ� �ȴ�. ������ ���ø����� �� ������ � Ŭ������ ���ؼ� ����ȭ�� ���� ������ ���⿡ ���� Ŭ������ �ľ��� �� �ֱ� �����̴�.
class CMultiThreadSync 
{
	friend class CThreadSync; // mSync �� CThreadSync Ŭ������ ������ �� �ֵ��� �ϱ� ���ؼ� ������� ����. �׷��� �׽�Ʈ�غ��� �̰� ���ص� ������ ���� Ŭ������ �ٱ� Ŭ���� ����� ���� ������ �����ϴ�.
public:
	class CThreadSync {
	public:
		CThreadSync(VOID) {
			T::mSync.Enter(); // �ش� Ŭ���� ����ȭ ����, �� �κ��� �۵��ϱ� ���ؼ��� T Ŭ������ CMultiThreadSync Ŭ������ ��ӹ޾ƾ� �Ѵ�.
		}

		~CThreadSync(VOID) {
			T::mSync.Leave(); // �ش� Ŭ���� ����ȭ ����, �� �κ��� �۵��ϱ� ���ؼ��� T Ŭ������ CMultiThreadSync Ŭ������ ��ӹ޾ƾ� �Ѵ�.
		}
	};
private:
	static CCriticalSection mSync;  // ���� CRITICAL_SECTINON ��ü, �����Ϸ� ����ȭ�� ���ؼ� Ȥ�ó� CThreadSync Ŭ������ �̰� ��ӹ��� Ŭ�������� ����� ���ϸ� �̰� �޸𸮿� �Ҵ���� �ʴ´�.
};

template <class T>
CCriticalSection CMultiThreadSync<T>::mSync; // �̷��� �صθ� ���� ���ø����� ���ο� Ŭ������ ������ ���� static���� mSync �� ���α׷� ����ñ��� �� ���� �����ȴ�. ���� ���� �����̴� �� ��������.