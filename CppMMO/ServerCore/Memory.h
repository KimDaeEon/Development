#pragma once
#include "Allocator.h"

// ���� ���ڸ� ���� ���ڰ� ���� ���� �ִ�.
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