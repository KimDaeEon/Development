#include "pch.h"
#include "CoreTLS.h"

// �׷� ���� �����ϸ� �������� LThreadId�� 16bit ������ ����� �ȵȴ�.
// WriteLock���� ThreadId�� 16bit ���� ����� �����ϱ� �����̴�.
thread_local uint32				LThreadId = 0;

thread_local std::stack<int32>	LLockStack;
thread_local SendBufferChunkRef LSendBufferChunk;