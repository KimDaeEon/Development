#pragma once

#include <stack>
#include <mutex>

class NativeMemoryPool
{
private:
    std::stack<void*> pool;  // 메모리 풀을 관리하는 스택 (네이티브 타입)
	std::mutex _mutex;       // 뮤텍스
    int blockSize;           // 각 블록의 크기
    int poolCapacity;        // 풀에서 관리할 블록 수

public:
    // 네이티브 메모리 풀 초기화
    NativeMemoryPool(int size, int capacity);

    // 소멸자
    ~NativeMemoryPool();

    // 메모리를 할당하거나 풀에서 가져옴 (Pop)
    void* Pop();

    // 사용한 메모리를 풀에 반환함 (Push)
    void Push(void* block);
};