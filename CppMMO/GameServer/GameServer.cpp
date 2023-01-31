#include "pch.h"
#include "ThreadManager.h"
#include <iostream>
#include <thread>
#include <mutex>
#include "Memory.h"
#include "Allocator.h"

#pragma region StompAllocatorTest
//class Player
//{
//public:
//	int id;
//
//	virtual ~Player(){}
//};
//
//class Knight : public Player
//{
//public:
//	int hp;
//	int hp2;
//};

	//Knight* knight = (Knight*)myNew<Player>();
	// 부모 클래스에서 자식 쪽의 주소를 건드리는 overflow 발생
	//knight->hp = 100;

	//myDelete(knight);
#pragma endregion

int main()
{
	mySet<int> um;

	um.insert(1);
	um.insert(1);
	um.insert(2);

	for (auto i : um)
	{
		cout << i << endl;
	}
	
}

