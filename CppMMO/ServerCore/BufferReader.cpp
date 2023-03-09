#include "pch.h"
#include "BufferReader.h"

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
