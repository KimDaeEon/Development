#pragma once

#ifdef _MSC_VER
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC  // �޸� ���� Ž���� ���� ����, CRT �� C Runtime Library �� �ǹ��̴�.
#include <crtdbg.h>

#ifndef _CONSOLE
#include <cstdlib>  // �ܼ� ���α׷��� ��� ���� ����
#endif

class CMemoryLeak 
{
public:
	CMemoryLeak() {
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // ������ �����Ǿ� �ִ� FLAG ���� ����, ���� ���⿡ ������� FLAG �� �����Ͽ� ����� �� �������� �Ҵ� ���� ����
		// �̷��� �� ������ �Ʒ����� _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF) �� �ϸ� 33�� ���ϵǴµ�, ������ ������ _CRTDBG_LEAK_CHECK_DF �� 32, _CRTDBG_ALLOC_MEM_DF �� 1 �̶� �׷���.


// �ܼ� ���α׷��� ���
#ifdef _CONSOLE
		_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE); // ���⼭ _CRTDBG_MODE_FILE �� �ؼ� �Ʒ����� ���� �ڵ�� � �������� ������� ���ϴ� ���̴�.
		_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT); // ���� ���������� ���� �Է��ϴ� ������ �Է��ϰ� ������ �Էµ� ������ ����, STDOUT �� ǥ�� ��� ��Ʈ���� ���� ������ ����Ѵٴ� ��, ���Ͽ��� ����� �����ϴ�.
		_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
		_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
		_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
		_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);

#define DEBUG_NORMALBLOCK new ( _NORMAL_BLOCK, __FILE__, __LINE__ ) 
#ifdef new
#undef new
#endif
#define new DEBUG_NORMALBLOCK  // �̷��� ���ϸ� new �� ������� ���� � ������ �� ��° �ٿ��� Memory Leak �� �߻��ϴ��� ǥ�ð� �ȵȴ�. �Ʒ� malloc �� �������ϴ� �κе� ���������̴�.

#else
		_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG); // ���� CrtSetDbgFlag �� ����ѵ�, ���� �����ߴ� ������ �������ְ�, ���⿡ �Է��� ���� ��带 �����Ѵ�. _CRTDBG_MODE_DEBUG �� ����� â�� �޼����� ����Ѵٴ� �ǹ�
		_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
		_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
#endif

#ifdef malloc
#undef malloc
#endif

#define malloc(s) (_malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__))  // malloc ���α׷��� ���� �����Ѵ�. �̰� ���ϸ� malloc ���� �Ҵ��� �� ��Ȯ�� � ������ �� ��° �ٿ��� Memory Leak �� ������� ǥ�ø� ���Ѵ�.
		// ���� new �� ���� �����ϴ� �κе� ��������. �ڼ��� ���� https://ldevil63.tistory.com/27 <- ��ũ ����.
	}
};

// �ʱ�ȭ�� �����ڸ� ���� �ڵ����� ���ֱ� ���� �������� ����
static CMemoryLeak MemoryLeak;

#endif
#endif