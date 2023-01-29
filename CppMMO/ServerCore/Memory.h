#pragma once
#include "Allocator.h"

// ���� ���ڸ� ���� ���ڰ� ���� ���� �ִ�.
template<typename Type, typename... Args>
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(BaseAllocator::Alloc(sizeof(Type)));

	// placement new
	new(memory) Type(std::forward<Args>(args)...);

	return memory;
}

template<typename Type>
void xdelete(Type* obj)
{
	obj->~Type();
	xrelease(obj);
}