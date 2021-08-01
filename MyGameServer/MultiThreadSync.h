#pragma once

template<class T> // 사실 굳이 여기서 템플릿을 쓰지 않아도 된다. 하지만 템플릿으로 한 이유는 어떤 클래스에 대해서 동기화를 적용 중인지 여기에 적힌 클래스로 파악할 수 있기 때문이다.
class CMultiThreadSync 
{
	friend class CThreadSync; // mSync 에 CThreadSync 클래스가 접근할 수 있도록 하기 위해서 프랜드로 선언. 그런데 테스트해보니 이거 안해도 지금은 내부 클래스가 바깥 클래스 멤버에 대해 접근이 가능하다.
public:
	class CThreadSync {
	public:
		CThreadSync(VOID) {
			T::mSync.Enter(); // 해당 클래스 동기화 시작, 이 부분이 작동하기 위해서는 T 클래스는 CMultiThreadSync 클래스를 상속받아야 한다.
		}

		~CThreadSync(VOID) {
			T::mSync.Leave(); // 해당 클래스 동기화 해제, 이 부분이 작동하기 위해서는 T 클래스는 CMultiThreadSync 클래스를 상속받아야 한다.
		}
	};
private:
	static CCriticalSection mSync;  // 실제 CRITICAL_SECTINON 개체, 컴파일러 최적화에 의해서 혹시나 CThreadSync 클래스를 이걸 상속받은 클래스에서 사용을 안하면 이게 메모리에 할당되지 않는다.
};

template <class T>
CCriticalSection CMultiThreadSync<T>::mSync; // 이렇게 해두면 위의 템플릿으로 새로운 클래스가 생성될 때만 static으로 mSync 가 프로그램 종료시까지 한 번만 생성된다. 정말 멋진 구조이니 잘 이해하자.