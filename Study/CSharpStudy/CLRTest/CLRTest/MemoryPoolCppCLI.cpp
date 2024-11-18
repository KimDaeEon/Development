#include "NativeMemoryPool.h"
#include <iostream>
#include <chrono>
#include <windows.h>  // for IntPtr

using namespace System;

public ref class MemoryPoolCppCLI
{
private:
    NativeMemoryPool* nativePool;  // 네이티브 메모리 풀을 관리하는 포인터

public:
    // 관리 클래스에서 네이티브 메모리 풀 초기화
    MemoryPoolCppCLI(int size, int capacity)
    {
        nativePool = new NativeMemoryPool(size, capacity);
    }

    ~MemoryPoolCppCLI()
    {
        this->!MemoryPoolCppCLI();
    }

    !MemoryPoolCppCLI()
    {
        if (nativePool != nullptr)
        {
            delete nativePool;  // 네이티브 메모리 풀 삭제
            nativePool = nullptr;
        }
    }

    // 메모리를 할당하거나 풀에서 가져옴 (IntPtr로 반환)
    IntPtr Pop()
    {
        void* nativePtr = nativePool->Pop();  // 네이티브 포인터에서 메모리 가져옴
        return IntPtr(nativePtr);             // 네이티브 포인터를 IntPtr로 변환
    }

    // 사용한 메모리를 풀에 반환함 (IntPtr로 받음)
    void Push(IntPtr memory)
    {
        void* nativePtr = memory.ToPointer(); // IntPtr을 네이티브 포인터로 변환
        nativePool->Push(nativePtr);          // 풀에 네이티브 포인터 반환
    }

    // 성능 테스트 메서드
    void TestPerformance(int iterations)
    {
        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < iterations; i++)
        {
            IntPtr memory = Pop();  // 메모리 할당 (또는 풀에서 가져옴)
            Push(memory);           // 사용 후 메모리 반환
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "C++/CLI MemoryPool: " << elapsed.count() << " seconds for " << iterations << " iterations." << std::endl;
    }
};