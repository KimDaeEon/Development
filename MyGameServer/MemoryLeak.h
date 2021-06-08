#pragma once

#ifdef _MSC_VER
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC  // 메모리 누수 탐지를 위해 선언, CRT 는 C Runtime Library 란 의미이다.
#include <crtdbg.h>

#ifndef _CONSOLE
#include <cstdlib>  // 콘솔 프로그램일 경우 따로 선언
#endif

class CMemoryLeak 
{
public:
	CMemoryLeak() {
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // 이전에 설정되어 있던 FLAG 값을 리턴, 지금 여기에 집어넣은 FLAG 를 세팅하여 디버그 힙 관리자의 할당 동작 제어
		// 이렇게 한 다음에 아래에서 _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF) 를 하면 33이 리턴되는데, 이전에 설정한 _CRTDBG_LEAK_CHECK_DF 가 32, _CRTDBG_ALLOC_MEM_DF 가 1 이라서 그렇다.


// 콘솔 프로그램일 경우
#ifdef _CONSOLE
		_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE); // 여기서 _CRTDBG_MODE_FILE 로 해서 아래에서 파일 핸들로 어떤 형식으로 출력할지 정하는 것이다.
		_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT); // 위와 마찬가지로 현재 입력하는 세팅을 입력하고 이전에 입력된 형식을 리턴, STDOUT 은 표준 출력 스트림에 관련 내용을 출력한다는 뜻, 파일에도 출력이 가능하다.
		_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
		_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
		_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
		_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);

#define DEBUG_NORMALBLOCK new ( _NORMAL_BLOCK, __FILE__, __LINE__ ) 
#ifdef new
#undef new
#endif
#define new DEBUG_NORMALBLOCK  // 이렇게 안하면 new 를 사용했을 때에 어떤 파일의 몇 번째 줄에서 Memory Leak 이 발생하는지 표시가 안된다. 아래 malloc 을 재정의하는 부분도 마찬가지이다.

#else
		_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG); // 위의 CrtSetDbgFlag 랑 비슷한데, 이전 세팅했던 값들을 리턴해주고, 여기에 입력한 보고서 모드를 세팅한다. _CRTDBG_MODE_DEBUG 는 디버그 창에 메세지를 출력한다는 의미
		_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
		_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
#endif

#ifdef malloc
#undef malloc
#endif

#define malloc(s) (_malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__))  // malloc 프로그램을 새로 정의한다. 이거 안하면 malloc 으로 할당할 때 정확히 어떤 파일의 몇 번째 줄에서 Memory Leak 이 생겼는지 표시를 안한다.
		// 위의 new 를 새로 정의하는 부분도 마찬가지. 자세한 것은 https://ldevil63.tistory.com/27 <- 링크 참조.
	}
};

// 초기화를 생성자를 통해 자동으로 해주기 위해 전역으로 선언
static CMemoryLeak MemoryLeak;

#endif
#endif