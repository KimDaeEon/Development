#include "NativeMemoryPool.h"
#include <cstdlib>  // for malloc, free

// 네이티브 메모리 풀 초기화
NativeMemoryPool::NativeMemoryPool(int size, int capacity)
{
    blockSize = size;
    poolCapacity = capacity;

	std::lock_guard<std::mutex> lock(_mutex);  // 뮤텍스 락
    // 미리 메모리 블록을 할당해서 풀에 저장
    for (int i = 0; i < poolCapacity; i++)
    {
        void* block = malloc(blockSize);  // 블록 할당
        if (block == nullptr)
        {
            throw std::bad_alloc();
        }
        pool.push(block);  // 스택에 추가
    }
}

// 네이티브 메모리 풀 소멸자
NativeMemoryPool::~NativeMemoryPool()
{
    // 소멸자에서 풀에 남아있는 모든 메모리 해제
    while (!pool.empty())
    {
        void* block = pool.top();
        pool.pop();
        free(block);  // 블록 해제
    }
}

// 메모리를 할당하거나 풀에서 가져옴 (Pop)
void* NativeMemoryPool::Pop()
{
    std::lock_guard<std::mutex> lock(_mutex);  // 뮤텍스 락

    if (pool.empty())
    {
        return malloc(blockSize);  // 새로 메모리 할당
    }
    else
    {
        void* block = pool.top();
        pool.pop();
        return block;
    }
}

// 사용한 메모리를 풀에 반환함 (Push)
void NativeMemoryPool::Push(void* block)
{
    std::lock_guard<std::mutex> lock(_mutex);  // 뮤텍스 락
    if (block != nullptr)
    {
        pool.push(block);  // 풀에 반환
    }
}
