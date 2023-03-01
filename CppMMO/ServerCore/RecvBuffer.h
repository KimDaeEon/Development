#pragma once

// ---------------------------
//		   RecvBuffer
// ---------------------------

// [r][][][w][][]와 같이 있으면 r <= index < w 사이의 데이터들이 아직 읽지 않은 유효한 데이터이다.
class RecvBuffer
{
	enum
	{
		BUFFER_COUNT = 10
	};

public:
	RecvBuffer(int32 bufferSize);
	~RecvBuffer();

	void			Relocate(); // _readPos, _writePos 재배치
	bool			OnRead(int32 numOfBytes);
	bool			OnWrite(int32 numOfBytes);

	BYTE*			ReadPos() { return &_buffer[_readPos]; }
	BYTE*			WritePos() { return &_buffer[_readPos]; }
	int32			DataSize() { return _writePos - _readPos; }
	int32			FreeSize() { return _capacity - _writePos; }

private:
	int32			_capacity = 0; // 실질적인 버퍼 크기, _writePos와 _readPos가 겹칠 수 있도록 여유를 두기 위한 것
	int32			_bufferSize = { 0 }; // 한 번에 쓸 수 있는 버퍼 크기
	int32			_readPos = { 0 }; // 읽은 데이터 인덱스
	int32			_writePos = { 0 }; // WSARecv로 입력된 마지막 데이터의 다음 인덱스 (새로 데이터 입력받을 수 있는 첫 인덱스)
	myVector<BYTE>	_buffer;
};

