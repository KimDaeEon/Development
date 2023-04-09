#pragma once
#include "Types.h"
#include "MemoryPool.h"

// MemoryPool �����ε� �޸� �Ҵ� ����ȭ �����ؼ��� ������ ����.
// ������ �̷� ��� �޸� ���� ������ �߻����� ���� � Ŭ�������� ������� �˾Ƴ��Ⱑ �����.
// �׷� �ͱ��� ����ؼ� ����ϴ� ����� ObjectPool ����̴�.

//#define _STOMP

template<typename Type>
class ObjectPool
{
public:
	 template<typename... Args>
	 static Type* Pop(Args&&... args)
	 {
// �޸� ������ ����ٸ� _STOMP�� �����ؼ� �޸� �Ҵ� ����� �����Ͽ� ���� ������ �����Ѵ�.
#ifdef _STOMP
		 MemoryHeader* ptr = reinterpret_cast<MemoryHeader*>(StompAllocator::Alloc(s_allocSize));
		 Type* memory = static_cast<Type*>(MemoryHeader::AttachHeader(ptr, s_allocSize));
#else
		 Type* memory = static_cast<Type*>(MemoryHeader::AttachHeader(s_pool.Pop(), s_allocSize));
#endif
		 
		 new(memory)Type(forward<Args>(args)...); // placement new
		 return memory;
	 }

	 static void Push(Type* obj)
	 {
		 obj->~Type();

#ifdef _STOMP
		 StompAllocator::Release(MemoryHeader::DetachHeader(obj));
#else
		 s_pool.Push(MemoryHeader::DetachHeader(obj));
#endif
		
	 }

	 // Pop�� Push�� ���� ���� ������ shared_ptr�� ���� �Ѱ��ִ� �Լ� �߰�
	 template<typename... Args>
	 static shared_ptr<Type> MakeShared(Args&&... args)
	 {
		 // shared_ptr�� ��� �Ʒ�ó�� �����ڿ��� �Ҹ��ڸ� ���� �������� �� �ִٴ� �͵� �˾Ƶ���.
		 shared_ptr<Type> ptr = { Pop(forward<Args>(args)...), Push };
		 return ptr;
	 }

private:

 	// s_�� static���� �ǹ�
	// Ŭ�������� MemoryPool�� ���� ������ �ȴ�.
	static constexpr int32		s_allocSize{ sizeof(Type) + sizeof(MemoryHeader) };
	static MemoryPool			s_pool;
};

template<typename Type>
MemoryPool ObjectPool<Type>::s_pool{ s_allocSize };