#pragma once
#include "Types.h"
#include "MemoryPool.h"

// MemoryPool 만으로도 메모리 할당 최적화 관련해서는 문제가 없다.
// 하지만 이런 경우 메모리 오염 문제가 발생했을 때에 어떤 클래스에서 생겼는지 알아내기가 힘들다.
// 그런 것까지 고려해서 사용하는 방식이 ObjectPool 방식이다.

//#define _STOMP

template<typename Type>
class ObjectPool
{
public:
	 template<typename... Args>
	 static Type* Pop(Args&&... args)
	 {
// 메모리 오염이 생긴다면 _STOMP를 설정해서 메모리 할당 방식을 변경하여 문제 원인을 추적한다.
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

	 // Pop과 Push를 잊을 수도 있으니 shared_ptr로 만들어서 넘겨주는 함수 추가
	 template<typename... Args>
	 static shared_ptr<Type> MakeShared(Args&&... args)
	 {
		 // shared_ptr의 경우 아래처럼 생성자에서 소멸자를 따로 지정해줄 수 있다는 것도 알아두자.
		 shared_ptr<Type> ptr = { Pop(forward<Args>(args)...), Push };
		 return ptr;
	 }

private:

 	// s_는 static임을 의미
	// 클래스별로 MemoryPool을 따로 가지게 된다.
	static constexpr int32		s_allocSize{ sizeof(Type) + sizeof(MemoryHeader) };
	static MemoryPool			s_pool;
};

template<typename Type>
MemoryPool ObjectPool<Type>::s_pool{ s_allocSize };