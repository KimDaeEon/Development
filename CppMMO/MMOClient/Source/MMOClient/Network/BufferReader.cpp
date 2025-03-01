#include "BufferReader.h"
// unreal5와 컨벤션이 똑같아서 여기에 서버처럼 "pch.h" 안해줘도 BYTE*, uint32 이런 부분에서 컴파일 에러가 안나는 것이다.

// -----------------------
//		BufferReader
// -----------------------

BufferReader::BufferReader()
{
}

BufferReader::BufferReader(BYTE* buffer, uint32 size, uint32 pos)
	: _buffer(buffer), _size(size), _pos(pos)
{
}

BufferReader::~BufferReader()
{
}

// 데이터가 있는지 확인해볼 수 있고, dest에 len 만큼의 데이터를 복사한다.
bool BufferReader::Peek(OUT void* dest, uint32 len)
{
	if (FreeSize() < len)
	{
		return false;
	}

	::memcpy(dest, &_buffer[_pos], len);

	return true;
}

bool BufferReader::Read(OUT void* dest, uint32 len)
{
	if (Peek(dest, len) == false)
	{
		return false;
	}

	_pos += len; // 이 부분이 Peek와의 차이점, 실제 데이터를 읽은 것이므로 _pos를 옮긴다.
	return true;
}
