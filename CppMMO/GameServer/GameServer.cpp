#include "pch.h"
#include "ThreadManager.h"
#include "Memory.h"
#include "Allocator.h"
#include "Memory.h"

#include <iostream>
#include <thread>
#include <mutex>
#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")


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

void HandleError(const char* cause)
{
	int32 errCode = ::WSAGetLastError();
	cout << cause << "Socket ErrorCode : " << errCode << endl;
}

#pragma region UDP_TEST
//{
	//	WSAData wsaData;

	//	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	//	{
	//		return 0;
	//	}

	//	SOCKET serverSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
	//	if (serverSocket == INVALID_SOCKET)
	//	{
	//		HandleError("Socket");
	//		return 0;
	//	}

	//	SOCKADDR_IN serverAddr;
	//	::memset(&serverAddr, 0, sizeof(serverAddr));
	//	serverAddr.sin_family = AF_INET;
	//	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	//	//serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	//	serverAddr.sin_port = ::htons(7777);

	//	if (::bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	//	{
	//		HandleError("Bind");
	//		return 0;
	//	}

	//	while (true)
	//	{
	//		this_thread::sleep_for(1s);
	//		char recvBuffer[1000] = "";

	//		SOCKADDR_IN clientAddr;
	//		::memset(&clientAddr, 0, sizeof(clientAddr));
	//		int32 addrLen = sizeof(clientAddr);

	//		int32 recvLen = ::recvfrom(serverSocket, recvBuffer, sizeof(recvBuffer), 0,
	//			(SOCKADDR*)&clientAddr, &addrLen);

	//		if (recvLen <= 0)
	//		{
	//			HandleError("RecvFrom");
	//			return 0;
	//		}

	//		cout << "[Recv] Data len : " << recvLen << " Data : " << recvBuffer << endl;

	//		int32 errorCode = ::sendto(serverSocket, recvBuffer, recvLen, 0,
	//			(SOCKADDR*)&clientAddr, sizeof(clientAddr));

	//		if (errorCode == SOCKET_ERROR)
	//		{
	//			HandleError("SendTo");
	//			return 0;
	//		}

	//		cout << "[Send] Data len : " << recvLen << " Data : " << recvBuffer << endl;
	//	}

	//	::WSACleanup();
	//}
#pragma endregion

#pragma region TCP_TEST
//{
	//	WSAData wsaData;

	//	// 윈속 DLL 초기화
	//	// 관련 정보가 wsaData에 채워진다.
	//	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	//	{
	//		cout << "WSAStartup Error!" << endl;
	//		return 0;
	//	}

	//	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	//	if (listenSocket == INVALID_SOCKET)
	//	{
	//		int32 errCode = ::WSAGetLastError();
	//		cout << "Socket ErrorCode : " << errCode << endl;
	//		return 0;
	//	}

	//	SOCKADDR_IN serverAddr; // IPv4 주소
	//	::memset(&serverAddr, 0, sizeof(serverAddr));
	//	serverAddr.sin_family = AF_INET;
	//	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY); // 주소 알아서 설정
	//	serverAddr.sin_port = ::htons(7777); // Host to Netork Short, Little-Endian -> Big-Endian 으로 변환

	//	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	//	{
	//		int32 errCode = ::WSAGetLastError();
	//		cout << "Bind ErrorCode : " << errCode << endl;
	//		return 0;
	//	}

	//	if (::listen(listenSocket, 10/*backLog 대기열 수*/) == SOCKET_ERROR)
	//	{
	//		int32 errCode = ::WSAGetLastError();
	//		cout << "Listen ErrorCode : " << errCode << endl;
	//		return 0;
	//	}

	//	while (true)
	//	{
	//		// client IP 주소 받기 용도, 벤 때리거나 할 때에 쓰일 수 있다.
	//		SOCKADDR_IN clientAddr;
	//		::memset(&clientAddr, 0, sizeof(clientAddr));
	//		int32 addrLen = sizeof(clientAddr);

	//		// 클라이언트와 통신을 하게 되는 소켓이 accept에서 생성
	//		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
	//		if (clientSocket == INVALID_SOCKET)
	//		{
	//			int32 errCode = ::WSAGetLastError();
	//			cout << "Accept ErrorCode : " << errCode << endl;
	//			return 0;
	//		}

	//		char ipAddress[16];
	//		::inet_ntop(AF_INET, &clientAddr.sin_addr, ipAddress, sizeof(ipAddress));
	//		cout << "Client Connected IP = " << ipAddress << endl;

	//		while (true)
	//		{
	//			char recvBuffer[10000] = "";

	//			this_thread::sleep_for(1s);

	//			int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
	//			if (recvLen <= 0)
	//			{
	//				int32 errCode = ::WSAGetLastError();
	//				cout << "Recv ErrorCode : " << errCode << endl;
	//				return 0;
	//			}

	//			cout << "[Recv] " << recvLen << "bytes: " << recvBuffer << endl;

	//			//int32 resultCode = ::send(clientSocket, recvBuffer, recvLen, 0);
	//			//if (resultCode == SOCKET_ERROR)
	//			//{
	//			//	int32 errCode = ::WSAGetLastError();
	//			//	cout << "Send ErrorCode : " << errCode << endl;
	//			//	return 0;
	//			//}
	//			//cout << "[Send] " << recvBuffer << endl;
	//		}

	//		::WSACleanup();
	//}
	//
	//}
#pragma endregion

#pragma region SelectTest
//struct Session
//{
//	SOCKET socket = INVALID_SOCKET;
//	char recvBuffer[BUFFSIZE] = {};
//	int32 recvBytes = 0;
//	int32 sendBytes = 0;
//};
//
//int main()
//{
//	WSAData wsaData;
//
//	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//	{
//		return 0;
//	}
//
//	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
//	if (listenSocket == INVALID_SOCKET)
//	{
//		HandleError("Socket");
//		return 0;
//	}
//
//	u_long on = 1;
//	if (::ioctlsocket(listenSocket, FIONBIO, &on) == INVALID_SOCKET)
//	{
//		return 0;
//	}
//
//	SOCKADDR_IN serverAddr;
//	::memset(&serverAddr, 0, sizeof(serverAddr));
//	serverAddr.sin_family = AF_INET;
//	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
//	serverAddr.sin_port = ::htons(7777);
//
//	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
//	{
//		return 0;
//	}
//
//	if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
//	{
//		return 0;
//	}
//
//	cout << "Accept" << endl;
//
//	vector<Session> sessions;
//	sessions.reserve(100);
//
//	fd_set reads;
//	fd_set writes;
//
//	while (true)
//	{
//		// 소켓 세트 초기화 작업
//		// select를 사용하면 reads와 writes가 내부적으로 제거되기에 이 작업을 계속 반복해야 한다.
//		FD_ZERO(&reads);
//		FD_ZERO(&writes);
//
//		// ListenSocket 등록
//		FD_SET(listenSocket, &reads);
//
//		// 용도별 소켓 set에 추가
//		for (Session& s : sessions)
//		{
//			if (s.recvBytes <= s.sendBytes)
//			{
//				FD_SET(s.socket, &reads);
//			}
//			else
//			{
//				FD_SET(s.socket, &writes);
//			}
//		}
//
//		cout << "select 대기중" << endl;
//
//		// 이렇게 select를 하고나면 reads와 wirtes에는 현재 해당 작업이 가능한 소켓만 남는다.
//		int32 retVal = ::select(0, &reads, &writes, nullptr, nullptr);
//
//		cout << "select 완료" << endl;
//
//		if (retVal == SOCKET_ERROR)
//		{
//			break;
//		}
//
//		// Listener 소켓 체크
//		if (FD_ISSET(listenSocket, &reads))
//		{
//			SOCKADDR_IN clientAddr;
//			int32 addrLen = sizeof(clientAddr);
//			SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
//
//			if (clientSocket != INVALID_SOCKET)
//			{
//				cout << "Client Connected" << endl;
//				sessions.push_back(Session{ clientSocket });
//			}
//		}
//
//		// 나머지 소켓 체크
//		for (Session& s : sessions)
//		{
//			// Read 체크
//			if (FD_ISSET(s.socket, &reads))
//			{
//				int32 recvLen = ::recv(s.socket, s.recvBuffer, BUFFSIZE, 0);
//				if (recvLen <= 0)
//				{
//					// TODO: sessions 제거
//					continue;
//				}
//
//				s.recvBytes = recvLen;
//			}
//
//			// Write 체크
//			if (FD_ISSET(s.socket, &writes))
//			{
//				int32 sendLen = ::send(s.socket, &s.recvBuffer[s.sendBytes], s.recvBytes - s.sendBytes, 0);
//				if (sendLen == SOCKET_ERROR)
//				{
//					// TODO: sessions 제거
//					continue;
//				}
//
//				s.sendBytes += sendLen;
//				// 에코 서버 구조라서 보낸 데이터를 다 받았으면 다시 초기화
//				if (s.recvBytes == s.sendBytes)
//				{
//					s.recvBytes = 0;
//					s.sendBytes = 0;
//				}
//			}
//		}
//	}
//
//	::WSACleanup();
//
//	return 0;
//}
#pragma endregion

#pragma region WSAEventSelectTest
//{
//WSAData wsaData;
//
//if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//{
//	return 0;
//}
//
//SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
//if (listenSocket == INVALID_SOCKET)
//{
//	HandleError("Socket");
//	return 0;
//}
//
//u_long on = 1;
//if (::ioctlsocket(listenSocket, FIONBIO, &on) == INVALID_SOCKET)
//{
//	return 0;
//}
//
//SOCKADDR_IN serverAddr;
//::memset(&serverAddr, 0, sizeof(serverAddr));
//serverAddr.sin_family = AF_INET;
//serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
//serverAddr.sin_port = ::htons(7777);
//
//if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
//{
//	return 0;
//}
//
//if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
//{
//	return 0;
//}
//
//cout << "Accept" << endl;
//
//// WSAEventSelect
//// 소켓과 관련된 네트워크 이벤트를 '이벤트 객체'를 통해 감지
//// 소켓 개수만큼 이벤트를 만들어야 한다.
//// WSAEventSeelct 함수를 호출하면 해당 소켓은 자동으로 Non-Blocking 모드로 전환된다.
//// accept()함수가 리턴하는 소캣은 listenSocket과 동일한 속성을 갖기에, clientSocket은 FD_READ, FD_WRITE 등을 다시 등록해야 한다.
//// 이벤트 발생 시에 적절한 소켓 함수를 호출해야 다음에 다시 해당 이벤트가 통지된다. ex) FD_READ 이벤트 발생했으면 recv() 호출해야 하고, 안하면 FD_READ 이벤트 발생하지 않는다.
//
//// WSAWaitForMultipleEvents
//// 이벤트를 모두 기다릴지 하나만 기다릴지 선택 가능
//// timeout 설정 가능
//// 모든 이벤트를 기다리지 않는다면 발생한 이벤트의 index를 리턴
//
//vector<Session> sessions;
//vector<WSAEVENT> wsaEvents;
//sessions.reserve(100);
//
//WSAEVENT listenEvent = ::WSACreateEvent();
//wsaEvents.push_back(listenEvent);
//sessions.push_back(Session{ listenSocket });
//
//// 소켓과 이벤트 연결
//if (::WSAEventSelect(listenSocket, listenEvent, FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR)
//{
//	return 0;
//}
//
//while (true)
//{
//	// Select 모델에서는 여기서 전체 FD_SET들을 초기화하고 다시 추가했어야 하지만,
//	// WSAEventSelect는 이벤트 객체를 통해 이것을 훨씬 더 간단히 작업할 수 있게 해준다.
//	// 하지만 이녀석도 Select와 마찬가지로 64개까지만 Event 객체를 관리 가능하다.
//	int32 idx = ::WSAWaitForMultipleEvents(wsaEvents.size(), &wsaEvents[0], false, WSA_INFINITE, false);
//
//	if (idx == WSA_WAIT_FAILED)
//	{
//		cout << "WSAWaitForMultipleEvents Failed!" << endl;
//		continue;
//	}
//
//	idx = idx - WSA_WAIT_EVENT_0; // WSAWaitForMultipleEvents에서 이 값을 더해줘서 index를 리턴해서 뻬줘야 한다.
//
//	::WSAResetEvent(wsaEvents[idx]); // 이거 없어도 WSAEnumNetworkEvents 쓰면 내부적으로 이벤트를 Non-Signal 상태로 바꿔준다.
//	WSANETWORKEVENTS networkEvents; // 어떤 네트워크 이벤트가 발생하는지 받아오기 위한 변수
//	if (::WSAEnumNetworkEvents(sessions[idx].socket, wsaEvents[idx], &networkEvents) == SOCKET_ERROR)
//	{
//		cout << "WSAEnumNetworkEvents Failed!" << endl;
//		continue;
//	}
//
//	// Listener 소켓 체크
//	if (networkEvents.lNetworkEvents & FD_ACCEPT)
//	{
//		// Error-Check
//		if (networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
//		{
//			continue;
//		}
//
//		SOCKADDR_IN clientAddr;
//		int32 addrLen = sizeof(clientAddr);
//
//		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
//		if (clientSocket != INVALID_SOCKET)
//		{
//			cout << "Client Connected" << endl;
//
//			WSAEVENT clientEvent = ::WSACreateEvent();
//			sessions.push_back(Session{ clientSocket });
//			wsaEvents.push_back(clientEvent);
//
//			// 새로 만들어진 통신용 소켓과 이벤트를 연결
//			if (::WSAEventSelect(clientSocket, clientEvent, FD_WRITE | FD_READ) == SOCKET_ERROR)
//			{
//				return 0;
//			}
//		}
//	}
//
//	// 통신용 소켓 체크
//	if (networkEvents.lNetworkEvents & FD_READ || networkEvents.lNetworkEvents & FD_WRITE)
//	{
//		// Error-Check
//		if ((networkEvents.lNetworkEvents & FD_READ) && (networkEvents.iErrorCode[FD_READ_BIT]) != 0)
//		{
//			continue;
//		}
//
//		// Error-Check
//		if ((networkEvents.lNetworkEvents & FD_WRITE) && (networkEvents.iErrorCode[FD_WRITE_BIT]) != 0)
//		{
//			continue;
//		}
//
//		Session& s = sessions[idx];
//
//		// Read
//		if (s.recvBytes == 0)
//		{
//			int32 recvLen = ::recv(s.socket, s.recvBuffer, BUFFSIZE, 0);
//			if (recvLen == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK)
//			{
//				// TODO: Remove Session
//				continue;
//			}
//
//			s.recvBytes = recvLen;
//			cout << "Recv Data = " << recvLen << endl;
//		}
//
//		// Write
//		if (s.recvBytes > s.sendBytes)
//		{
//			int32 sendLen = ::send(s.socket, &s.recvBuffer[s.sendBytes], s.recvBytes - s.sendBytes, 0);
//			if (sendLen == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK)
//			{
//				// TODO: Remove Session
//				continue;
//			}
//
//			s.sendBytes += sendLen;
//			if (s.recvBytes == s.sendBytes)
//			{
//				s.recvBytes = 0;
//				s.sendBytes = 0;
//			}
//
//			cout << "Send Data = " << sendLen << endl;
//		}
//	}
//
//	if (networkEvents.lNetworkEvents & FD_CLOSE)
//	{
//		// TODO: Remove Socket
//	}
//}
//
//::WSACleanup();
//
//return 0;
//}
#pragma endregion

#pragma region OverlappedWithEventTest
//{
//WSAData wsaData;
//
//if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//{
//	return 0;
//}
//
//SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
//if (listenSocket == INVALID_SOCKET)
//{
//	HandleError("Socket");
//	return 0;
//}
//
//u_long on = 1;
//if (::ioctlsocket(listenSocket, FIONBIO, &on) == INVALID_SOCKET)
//{
//	return 0;
//}
//
//SOCKADDR_IN serverAddr;
//::memset(&serverAddr, 0, sizeof(serverAddr));
//serverAddr.sin_family = AF_INET;
//serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
//serverAddr.sin_port = ::htons(7777);
//
//if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
//{
//	return 0;
//}
//
//if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
//{
//	return 0;
//}
//
//cout << "Accept" << endl; // Accept까지는 일단 동기로 처리, 추후에 라이브러리 만들 때에 비동기로..
//
//// WSASend, WSARecv
//// 1) 비동기 입출력 소켓
//// 2) WSABUF 배열의 시작 주소
//// 3) WSABUF 개수(WSABUF를 여러개 전달할 수도 있다.)
//// 4) 보내고/받은 바이트 수
//// 5) 상세 옵션인데 보통 0으로 설정
//// 6) WSAOVERLAPPED 구조체 주소값 (이벤트 방식에서 이 구조체에 이벤트 Handle을 넣어줄 것이다.)
//// 7) 입출력이 완료되면 OS가 호출할 콜백 함수 (이번 테스트인 이벤트 방식에선 사용하지 않음)
//
//// Overlapped 모델 (이벤트 기반)
//// 1) 비동기 입출력 지원하는 소켓 생성
//// 2) 통지 받기 위한 이벤트 객체 생성
//// 3) 비동기 입출력 함수 호출(위에서 만든 이벤트 객체를 같이 넘겨줌)
//// 4) 비동기 작업이 바로 완료되지 않으면 WSA_IO_PENDING을 보내주고, 
////	  완료되면 이벤트 객체가 Signaled 상태로 만들어져 완료 상태를 알려준다.
//// 5) WSAWaitForMultipleEvents 함수 호출해서 이벤트 객체의 signal 판별
//// 6) WSAGetOverlappedResult 함수 호출해서 입출력 결과 확인 및 데이터 처리
//
//// WSAGetOverlappedResult
//// 1) 비동기 입출력 소켓
//// 2) 넘겨주었던 overlapped 구조체
//// 3) 전송된 바이트 수
//// 4) 비동기 입출력 작업이 끝날때까지 대기할지 여부, 이번에는 Event객체를 기다렸다가 처리하므로 이 부분 false라 보면 된다.
//// 5) 비동기 입출력 작업 관련 부가 정보, 거의 사용 안하는 변수
//
//while (true)
//{
//	SOCKADDR_IN clientAddr;
//	int32 addrLen = sizeof(clientAddr);
//
//	SOCKET clientSocket;
//
//	// 나중엔 이 부부이 AcceptEx가 사용이 될 것이다.
//	while (true)
//	{
//		clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
//		if (clientSocket != INVALID_SOCKET)
//		{
//			break;
//		}
//
//		if (::WSAGetLastError() == WSAEWOULDBLOCK)
//		{
//			continue;
//		}
//
//		// 문제 있는 상황
//		cout << "Something is wrong" << endl;
//		return 0;
//	}
//
//	Session session = Session{ clientSocket };
//	WSAEVENT wsaEvent = ::WSACreateEvent();
//	session.overlapped.hEvent = wsaEvent;
//
//	cout << "Client Connected!" << endl;
//
//	while (true)
//	{
//		WSABUF wsaBuf;
//		wsaBuf.buf = session.recvBuffer; // 절대 밖에서 건드리면 안된다. WSA함수만 건드려야 한다.
//		wsaBuf.len = BUFFSIZE;
//
//		DWORD recvLen = 0;
//		DWORD flags = 0;
//		if (::WSARecv(clientSocket, &wsaBuf, 1, &recvLen, &flags, &session.overlapped, nullptr) == SOCKET_ERROR)
//		{
//			if (::WSAGetLastError() == WSA_IO_PENDING)
//			{
//				cout << "WSAWaitForMultipleEvents 대기.." << endl;
//				// 지연 상태일 때에 여기서는 이벤트 객체가 Signal이 될 때까지 확인한다.
//				::WSAWaitForMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, FALSE);
//				cout << "WSAWaitForMultipleEvents 완료!" << endl;
//
//				WSAGetOverlappedResult(session.socket, &session.overlapped, &recvLen, FALSE, &flags);
//			}
//			else
//			{
//				// TODO: 문제 있는 상황 Socket 통신 종료한다.
//				break;
//			}
//		}
//
//		cout << "Data Recv Len = " << recvLen << endl;
//	}
//
//	::closesocket(session.socket);
//	::WSACloseEvent(wsaEvent);
//}
//
//::WSACleanup();
//
//return 0;
//}
#pragma endregion

#pragma region OverlappedWithCompletionPort
//struct Session
//{
//	SOCKET socket = { INVALID_SOCKET };
//	char recvBuffer[BUFFSIZE] = {};
//	int32 recvBytes = { 0 };
//	int32 sendBytes = { 0 };
//};
//
//enum IO_TYPE
//{
//	READ,
//	WRITE,
//	ACCEPT,
//	CONNECT
//};
//
//struct OverlappedEx
//{
//	WSAOVERLAPPED overlapped = {};
//	int32 type = 0; // READ, WRITE, ACCEPT, CONNECT
//};
//
//void WorkerThreadMain(HANDLE iocpHandle)
//{
//	while (true)
//	{
//		DWORD bytesTransferred = 0;
//		Session* session = nullptr;
//		OverlappedEx* overlappedEx = nullptr;
//
//		BOOL ret = ::GetQueuedCompletionStatus(iocpHandle, &bytesTransferred, (ULONG_PTR*)&session, (LPOVERLAPPED*)&overlappedEx, INFINITE);
//
//		if (ret == FALSE || bytesTransferred == 0)
//		{
//			// TODO: 연결 끊기
//			continue;
//		}
//
//		ASSERT_CRASH(overlappedEx->type == IO_TYPE::READ);
//
//		cout << "Recv Data IOCP = " << bytesTransferred << endl;
//
//		// 메인 스레드에서 WSARecv 걸어줬던 것 반복, 이렇게 계속 다시 걸어줘야지 IO 작업이 추가되어서 완료된 후 GetQueuedCompletionStatus로 넘겨받는다.
//		WSABUF wsaBuf;
//		wsaBuf.buf = session->recvBuffer;
//		wsaBuf.len = BUFFSIZE;
//
//		DWORD recvLen = 0;
//		DWORD flags = 0;
//
//		// 만약에 여기서 Send같은 작업을 하고 싶다면 overlappedEx의 type을 바꿔주면 된다.
//		// 주의해야할 점은 이렇게 비동기 입출력 함수에 넣어준 session, overlappedEx는 절대 메모리 해제가 되면 안된다.
//		// 로그아웃 시에 발생하는 크래시의 주된 요인이 이것이란 것을 명심해두자.
//		::WSARecv(session->socket, &wsaBuf, 1, &recvLen, &flags, &overlappedEx->overlapped, NULL);
//	}
//}
//
//int main()
//{
//	WSAData wsaData;
//
//	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//	{
//		return 0;
//	}
//
//	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
//	if (listenSocket == INVALID_SOCKET)
//	{
//		HandleError("Socket");
//		return 0;
//	}
//
//	u_long on = 1;
//	if (::ioctlsocket(listenSocket, FIONBIO, &on) == INVALID_SOCKET)
//	{
//		return 0;
//	}
//
//	SOCKADDR_IN serverAddr;
//	::memset(&serverAddr, 0, sizeof(serverAddr));
//	serverAddr.sin_family = AF_INET;
//	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
//	serverAddr.sin_port = ::htons(7777);
//
//	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
//	{
//		return 0;
//	}
//
//	if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
//	{
//		return 0;
//	}
//
//	cout << "Accept" << endl; // Accept까지는 일단 동기로 처리, 추후에 라이브러리 만들 때에 비동기로..
//
//	// WSASend, WSARecv
//	// 1) 비동기 입출력 소켓
//	// 2) WSABUF 배열의 시작 주소
//	// 3) WSABUF 개수(WSABUF를 여러개 전달할 수도 있다.)
//	// 4) 보내고/받은 바이트 수
//	// 5) 상세 옵션인데 보통 0으로 설정
//	// 6) WSAOVERLAPPED 구조체 주소값 (이벤트 방식에서 이 구조체에 이벤트 Handle을 넣어줄 것이다.)
//	// 7) 입출력이 완료되면 OS가 호출할 콜백 함수 (이번 테스트인 이벤트 방식에선 사용하지 않음)
//
//	// Overlapped 모델 (콜백 함수 기반)
//	// 1) 비동기 입출력 지원하는 소켓 생성
//	// 2) 비동기 입출력 함수 호출, 이 때에 이벤트와 다르게 콜백 함수 포인터를 넘겨준다.
//	// 3) 비동기 작업이 바로 완료되지 않으면 WSA_IO_PENDING을 보내주고, 
//	//	  완료되면 비동기 작업 실행한 쓰레드를 'Alertable Wait' 상태(콜백 함수 호출할 준비가 완료된 상태)로 만든다.
//	//  ex) WaitForSingleObjectEx, WaitForMultipleObjectsEx, SleepEx, WSAWaitForMultipleEvents
//	// 4) 'Alertable Wait' 상태가 되면 운영체제는 APC queue를 비우면서 모든 콜백 함수(완료 루틴) 호출, 자세한 내용은 아래 링크 참조
//	// https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=kimjb1216&logNo=10075828852
//	// 5) 완료 루틴 호출이 끝나면 쓰레드는 'Alertable Wait' 상태가 해제된다.
//	// 이벤트 방식처럼 이벤트를 각각 만들어줄 필요가 없다.
//
//	// Select 모델
//	// 장점) 윈도우/리눅스 공통
//	// 단점) 성능 최하 (매번 FD_SET 등록, 해제), 관리 가능 객체 64개 제한
//
//	// WSAEventSelect 모델
//	// 장점) 단순한 사용, 비교적 뛰어난 성능
//	// 다점) 관리 가능 객체 64개 제한
//
//	// Overlapped (이벤트 기반) 모델
//	// 장점) 성능이 좋다.
//	// 단점) 관리 가능 객체 64개 제한, 이벤트 객체를 만들어서 1:1 대응을 시켜야함
//
//	// Overlapped (콜백 기반) 모델
//	// 장점) 성능이 좋다.
//	// 단점) 모든 비동기 소켓 함수에서 사용가능하진 않다. (accept가 안됨, AcceptEx 함수에서 Callback 받는 부분이 없음)
//	//		빈번한 Alertable Wait로 인한 성능 저하, APC Queue는 스레드 귀속이기에 다른 스레드에 일감 분배가 안됨
//
//	// IOCP (Completion Port) 모델
//	// APC 대신에 Completion Port로 처리, Completion Port는 1개이고 스레드마다 귀속이 아님
//	// Alertable Wait 대신 GetQueueCompletionStatus 함수를 사용
//	// 헷갈리는 점들이 좀 있는데 https://chfhrqnfrhc.tistory.com/entry/IOCP 링크 참조
//
//	// CompletionPort 생성 및 생성된 CompletionPort에 소켓을 등록하는 용도로 사용
//	// 아래처럼 인자를 채워주면 Completion Port를 생성하는 용도로 사용된다.
//	HANDLE iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
//
//	// WorkerThreads 생성
//	for (int32 i = 0; i < 5; i++)
//	{
//		GThreadManager->Launch([=]()
//			{
//				WorkerThreadMain(iocpHandle);
//			});
//	}
//
//	vector<Session*> sessionManager;
//
//	while (true)
//	{
//		SOCKADDR_IN clientAddr;
//		int32 addrLen = sizeof(clientAddr);
//
//		SOCKET clientSocket;
//
//		// 나중엔 이 부부이 AcceptEx가 사용이 될 것이다.
//		while (true)
//		{
//			clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
//			if (clientSocket != INVALID_SOCKET)
//			{
//				break;
//			}
//
//			if (::WSAGetLastError() == WSAEWOULDBLOCK)
//			{
//				continue;
//			}
//
//			// 문제 있는 상황
//			cout << "Something is wrong" << endl;
//			return 0;
//		}
//
//		Session* session = new Session{ clientSocket };
//		sessionManager.push_back(session);
//
//		cout << "Client Connected!" << endl;
//
//		// 새로 생성된 소켓을 CP에 등록
//		::CreateIoCompletionPort((HANDLE)clientSocket, iocpHandle, (ULONG_PTR)session, /*0 입력시 CPU 최대 코어 갯수로 설정됨*/0);
//
//		WSABUF wsaBuf;
//		wsaBuf.buf = session->recvBuffer;
//		wsaBuf.len = BUFFSIZE;
//
//		DWORD recvLen = 0;
//		DWORD flags = 0;
//		OverlappedEx* overlappedEx = new OverlappedEx();
//		overlappedEx->type = IO_TYPE::READ;
//
//		// 최초로 WSARecv를 걸어주는 것까지가 Main Thread의 역할
//		::WSARecv(clientSocket, &wsaBuf, 1, &recvLen, &flags, &overlappedEx->overlapped, NULL);
//	}
//
//	GThreadManager->Join();
//	::WSACleanup();
//
//	return 0;
//}
#pragma endregion

int main()
{
	return 0;
}