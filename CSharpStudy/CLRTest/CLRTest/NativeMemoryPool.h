#pragma once

#include <stack>
#include <mutex>

class NativeMemoryPool
{
private:
    std::stack<void*> pool;  // �޸� Ǯ�� �����ϴ� ���� (����Ƽ�� Ÿ��)
	std::mutex _mutex;       // ���ؽ�
    int blockSize;           // �� ����� ũ��
    int poolCapacity;        // Ǯ���� ������ ��� ��

public:
    // ����Ƽ�� �޸� Ǯ �ʱ�ȭ
    NativeMemoryPool(int size, int capacity);

    // �Ҹ���
    ~NativeMemoryPool();

    // �޸𸮸� �Ҵ��ϰų� Ǯ���� ������ (Pop)
    void* Pop();

    // ����� �޸𸮸� Ǯ�� ��ȯ�� (Push)
    void Push(void* block);
};