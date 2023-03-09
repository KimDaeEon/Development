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

// �����Ͱ� �ִ��� Ȯ���غ� �� �ְ�, dest�� len ��ŭ�� �����͸� �����Ѵ�.
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

	_pos += len; // �� �κ��� Peek���� ������, ���� �����͸� ���� ���̹Ƿ� _pos�� �ű��.
	return true;
}