#include "pch.h"
#include "ThreadManager.h"
#include "Memory.h"
#include "Allocator.h"
#include "Memory.h"

#include <iostream>
#include <thread>
#include <mutex>


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

#pragma region TypeCastTest
//using TL = TypeList<class Animal, class Dog, class Cat>;
//class Animal
//{
//public:
//	Animal()
//	{
//		INIT_TL(Animal);
//	}
//
//	virtual ~Animal() 
//	{
//		cout << "~Animal()" << endl;
//	}
//
//	DECLARE_TL;
//};
//
//class Dog : public Animal
//{
//public:
//	Dog()
//	{
//		INIT_TL(Dog);
//	}
//
//	~Dog()
//	{
//		cout << "~Dog()" << endl;
//	}
//};
//
//class Cat : public Animal
//{
//public:
//	Cat()
//	{
//		INIT_TL(Cat);
//	}
//
//	~Cat()
//	{
//		cout << "~Cat()" << endl;
//	}
//};

//{
//	Animal* animal = new Animal();

//	bool canTypeCast = CanTypeCast<Dog*>(animal);
//	Dog* dog = TypeCast<Dog*>(animal);

//	delete animal;
//}

//{
//	// shared_ptr로도 작동하는 것을 확인
//	shared_ptr<Dog> dog = myMakeShared<Dog>();

//	shared_ptr<Animal> animal = TypeCast<Animal>(dog);
//	shared_ptr<Cat> cat = TypeCast<Cat>(animal);

//	bool canTypeCast1 = CanTypeCast<Animal>(dog);
//	bool canTypeCast2 = CanTypeCast<Cat>(animal);
//}
#pragma endregion

int main()
{
	return 0;
}