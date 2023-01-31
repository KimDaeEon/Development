#pragma once
#include "Allocator.h"

// 가변 인자를 쓰면 인자가 없을 수도 있다.
template<typename Type, typename... Args>
Type* myNew(Args&&... args)
{
	Type* memory = static_cast<Type*>(myAlloc(sizeof(Type)));

	// placement new
	new(memory) Type(std::forward<Args>(args)...);

	return memory;
}

template<typename Type>
void myDelete(Type* obj)
{
	obj->~Type();
	myRelease(obj);
}