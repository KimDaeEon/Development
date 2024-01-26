#include "pch.h"
#include <iostream>

#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "ClientSession.h"
#include "ClientSessionManager.h"
#include "ClientPacketHandler.h"
#include "BufferWriter.h"
#include "Protocol.pb.h"
#include "Job.h"
#include "Room.h"
#include "DBConnectionPool.h"
#include "DBBind.h"
#include "DBSynchronizer.h"
#include "XmlParser.h"
#include "GeneratedStoredProcedures.h"


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

void DoWorkerJob(ServerServiceRef& service)
{
	constexpr int WORKER_TICK = 64;
	while (true)
	{
		LEndTickCount = ::GetTickCount64() + WORKER_TICK;

		// 네트워크 입출력 처리, 10ms TimeOut을 두어서 Iocp작업 없으면 GlobalQueue의 작업을 처리한다.
		service->GetIocpCore()->Dispatch(10);

		// 예약 작업 분배
		ThreadManager::DistributeFutureJobs();

		// 글로벌 큐 작업 처리
		ThreadManager::DoGlobalQueueJobs();
	}
}

// Data Source=DESKTOP-9IA022F;Initial Catalog=TestDB;User ID=sa;Password=********;Connect Timeout=30;Encrypt=False;TrustServerCertificate=False;ApplicationIntent=ReadWrite;MultiSubnetFailover=False
int main()
{
	ASSERT_CRASH(GDBConnectionPool->Connect(1,
		L"Driver={ODBC Driver 17 for SQL Server};Server=(localdb)\\MSSQLLocalDB;Database=ServerDb;Trusted_Connection=Yes;"));

	// DBSync
	{
		DBConnection* dbConn = GDBConnectionPool->Pop();
		DBSynchronizer dbSync(*dbConn);
		dbSync.Synchronize(L"GameDB.xml");
		
		// insert
		{
			SP::InsertGold insertGold(*dbConn);
			insertGold.In_Gold(100);
			insertGold.In_CreateTime(TIMESTAMP_STRUCT{ 4220,06,17 });
			insertGold.Execute();
		}

		// select
		{
			SP::GetGold getGold(*dbConn);
			getGold.In_Gold(100);

			int32 id = 0;
			int32 gold = 0;
			TIMESTAMP_STRUCT createTime;

			getGold.Out_Id(id); 
			getGold.Out_Gold(gold);
			getGold.Out_CreateTime(createTime);

			getGold.Execute();

			while(getGold.Fetch())
			{
				cout << "id" << id << " gold" << gold << " createTime.year" << createTime.year << endl;
			}
		}
		
		GDBConnectionPool->Push(dbConn);
	}

	ClientPacketHandler::Init();

	ServerServiceRef service = myMakeShared<ServerService>(
		NetworkAddress(L"127.0.0.1", 7777),
		myMakeShared<IocpCore>(),
		myMakeShared<ClientSession>,
		10000
		);

	ASSERT_CRASH(service->Start());
	
	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([&service]()
			{
				while (true)
				{
					DoWorkerJob(service);
				}
			});
	}

	while (true)
	{
		DoWorkerJob(service);
		this_thread::sleep_for(1ms);
	}

	GThreadManager->Join();
	return 0;
}