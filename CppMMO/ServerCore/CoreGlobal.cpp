#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"

ThreadManager* GThreadManager = nullptr;

// ���⼭ Manager�� ���� ������ ����
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
} GCoreGlobal; // ���������� ���� �ʱ�ȭ ����
