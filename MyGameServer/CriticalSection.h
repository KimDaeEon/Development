#pragma once
#include<Windows.h>
class CCriticalSection {
	// 크리티컬 섹션은 사용자 객체를 사용, 커널 객체가 아니므로 가볍고 빠르다. 그러나 한 프로세스 내의 쓰레드 사이에서만 동기화가 가능하다.
	// 커널 모드 동기화를 사용하면 동기화 관리를 커널이 해주기 때문에 프로세스와 프로세스 간 동기화가 되는데, 이것은 불가능하다.
	// 링크 참조 https://genesis8.tistory.com/154  https://3dmpengines.tistory.com/611

public:
	CCriticalSection(VOID) {
		InitializeCriticalSection(&mSync); // CRITICAL_SECTION 개체 초기화, 이게 제대로 초기화가 안되거나 아래 Delete 가 제대로 안되면 동기화가 제대로 안된다고 쓰여 있다.
	}

	~CCriticalSection(VOID) {
		DeleteCriticalSection(&mSync); // CRITICAL_SECTION 개체 삭제
	}

	inline VOID Enter(VOID) {
		EnterCriticalSection(&mSync); // 이것이 실행되면 그때부터 해당 리소스에 접근하려는 스레드들은 대기상태가 된다.
	}

	inline VOID Leave(VOID) {
		LeaveCriticalSection(&mSync); // 이것이 호출이 안되면 스레드가 계속 대기를 해야하니 꼭 Critical Section 이 끝났으면 이를 호출하자.
	}

private:
	CRITICAL_SECTION mSync; // CRITICAL_SECTION
};