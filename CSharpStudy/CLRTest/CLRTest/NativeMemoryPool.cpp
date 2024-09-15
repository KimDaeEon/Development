#include "NativeMemoryPool.h"
#include <cstdlib>  // for malloc, free

// ����Ƽ�� �޸� Ǯ �ʱ�ȭ
NativeMemoryPool::NativeMemoryPool(int size, int capacity)
{
    blockSize = size;
    poolCapacity = capacity;

	std::lock_guard<std::mutex> lock(_mutex);  // ���ؽ� ��
    // �̸� �޸� ����� �Ҵ��ؼ� Ǯ�� ����
    for (int i = 0; i < poolCapacity; i++)
    {
        void* block = malloc(blockSize);  // ��� �Ҵ�
        if (block == nullptr)
        {
            throw std::bad_alloc();
        }
        pool.push(block);  // ���ÿ� �߰�
    }
}

// ����Ƽ�� �޸� Ǯ �Ҹ���
NativeMemoryPool::~NativeMemoryPool()
{
    // �Ҹ��ڿ��� Ǯ�� �����ִ� ��� �޸� ����
    while (!pool.empty())
    {
        void* block = pool.top();
        pool.pop();
        free(block);  // ��� ����
    }
}

// �޸𸮸� �Ҵ��ϰų� Ǯ���� ������ (Pop)
void* NativeMemoryPool::Pop()
{
    std::lock_guard<std::mutex> lock(_mutex);  // ���ؽ� ��

    if (pool.empty())
    {
        return malloc(blockSize);  // ���� �޸� �Ҵ�
    }
    else
    {
        void* block = pool.top();
        pool.pop();
        return block;
    }
}

// ����� �޸𸮸� Ǯ�� ��ȯ�� (Push)
void NativeMemoryPool::Push(void* block)
{
    std::lock_guard<std::mutex> lock(_mutex);  // ���ؽ� ��
    if (block != nullptr)
    {
        pool.push(block);  // Ǯ�� ��ȯ
    }
}
