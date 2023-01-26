#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"

ThreadManager* GThreadManager = nullptr;

// 여기서 Manager의 생성 순서를 조절
class CoreGlobal
{
public:
	CoreGlobal()
	{
		GThreadManager = new ThreadManager();
	}

	~CoreGlobal()
	{
		delete GThreadManager;
	}
} GCoreGlobal; // 전역변수를 통해 초기화 실행
