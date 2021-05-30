#pragma once
#include<Windows.h>
class CCriticalSection {
	// ũ��Ƽ�� ������ ����� ��ü�� ���, Ŀ�� ��ü�� �ƴϹǷ� ������ ������. �׷��� �� ���μ��� ���� ������ ���̿����� ����ȭ�� �����ϴ�.
	// Ŀ�� ��� ����ȭ�� ����ϸ� ����ȭ ������ Ŀ���� ���ֱ� ������ ���μ����� ���μ��� �� ����ȭ�� �Ǵµ�, �̰��� �Ұ����ϴ�.
	// ��ũ ���� https://genesis8.tistory.com/154  https://3dmpengines.tistory.com/611

public:
	CCriticalSection(VOID) {
		InitializeCriticalSection(&mSync); // CRITICAL_SECTION ��ü �ʱ�ȭ, �̰� ����� �ʱ�ȭ�� �ȵǰų� �Ʒ� Delete �� ����� �ȵǸ� ����ȭ�� ����� �ȵȴٰ� ���� �ִ�.
	}

	~CCriticalSection(VOID) {
		DeleteCriticalSection(&mSync); // CRITICAL_SECTION ��ü ����
	}

	inline VOID Enter(VOID) {
		EnterCriticalSection(&mSync); // �̰��� ����Ǹ� �׶����� �ش� ���ҽ��� �����Ϸ��� ��������� �����°� �ȴ�.
	}

	inline VOID Leave(VOID) {
		LeaveCriticalSection(&mSync); // �̰��� ȣ���� �ȵǸ� �����尡 ��� ��⸦ �ؾ��ϴ� �� Critical Section �� �������� �̸� ȣ������.
	}

private:
	CRITICAL_SECTION mSync; // CRITICAL_SECTION
};