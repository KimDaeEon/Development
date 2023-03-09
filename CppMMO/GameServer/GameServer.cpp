#include "pch.h"
#include <iostream>

#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"

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
	ServerServiceRef service = myMakeShared<ServerService>(
		NetworkAddress(L"127.0.0.1", 7777),
		myMakeShared<IocpCore>(),
		myMakeShared<GameSession>,
		10000
		);

	ASSERT_CRASH(service->Start());
	
	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}

	char sendData[1000] = "Hello World";

	while (true)
	{
		SendBufferRef sendBuffer = GSendBufferManager->Open(4096);

		BufferWriter bw(sendBuffer->Buffer(), 4096);

		// 이제 헤더를 이렇게 미리 집어둘 수 있다.
		PacketHeader* header = bw.Reserve<PacketHeader>();
		bw << (uint32)100 << (uint32)10 << (uint32)1;

		bw.Write(sendData, sizeof(sendData));

		// 앞에서 헤더를 미리 집어둔 후 뒤에서 이렇게 입력이 가능하다.
		// 맨 마지막에 size를 입력해주어야 하기에 Reserve를 사용한 것이다.
		header->size = bw.WriteSize();
		header->id = 1; // Hello Msg

		sendBuffer->Close(bw.WriteSize());

		this_thread::sleep_for(250ms);
		GSessionManager.Broadcast(sendBuffer);
	}

	GThreadManager->Join();
	return 0;
}