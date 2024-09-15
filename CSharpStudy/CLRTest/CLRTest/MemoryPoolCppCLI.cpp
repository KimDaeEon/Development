#include "NativeMemoryPool.h"
#include <iostream>
#include <chrono>
#include <windows.h>  // for IntPtr

using namespace System;

public ref class MemoryPoolCppCLI
{
private:
    NativeMemoryPool* nativePool;  // ����Ƽ�� �޸� Ǯ�� �����ϴ� ������

public:
    // ���� Ŭ�������� ����Ƽ�� �޸� Ǯ �ʱ�ȭ
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
            delete nativePool;  // ����Ƽ�� �޸� Ǯ ����
            nativePool = nullptr;
        }
    }

    // �޸𸮸� �Ҵ��ϰų� Ǯ���� ������ (IntPtr�� ��ȯ)
    IntPtr Pop()
    {
        void* nativePtr = nativePool->Pop();  // ����Ƽ�� �����Ϳ��� �޸� ������
        return IntPtr(nativePtr);             // ����Ƽ�� �����͸� IntPtr�� ��ȯ
    }

    // ����� �޸𸮸� Ǯ�� ��ȯ�� (IntPtr�� ����)
    void Push(IntPtr memory)
    {
        void* nativePtr = memory.ToPointer(); // IntPtr�� ����Ƽ�� �����ͷ� ��ȯ
        nativePool->Push(nativePtr);          // Ǯ�� ����Ƽ�� ������ ��ȯ
    }

    // ���� �׽�Ʈ �޼���
    void TestPerformance(int iterations)
    {
        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < iterations; i++)
        {
            IntPtr memory = Pop();  // �޸� �Ҵ� (�Ǵ� Ǯ���� ������)
            Push(memory);           // ��� �� �޸� ��ȯ
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "C++/CLI MemoryPool: " << elapsed.count() << " seconds for " << iterations << " iterations." << std::endl;
    }
};