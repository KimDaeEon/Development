#include "pch.h"
#include "RecvBuffer.h"

// ---------------------------
//		   RecvBuffer
// ---------------------------

RecvBuffer::RecvBuffer(int32 bufferSize) : _bufferSize(bufferSize)
{
	_capacity = bufferSize * BUFFER_COUNT;
	_buffer.resize(_capacity);
}

RecvBuffer::~RecvBuffer()
{
}

void RecvBuffer::Relocate()
{
	int32 dataSize = DataSize();

	// _readPos == _writePos인 경우 둘 다 위치를 0으로 변경
	if (dataSize == 0)
	{
		_readPos = _writePos = 0;
	}
	else
	{
		// 여유 공간이 버퍼 1개 크기 미만이면 데이터를 앞으로 당긴다.
		// _capacity로 인해서 이 부분이 실행될 확률이 내려간다.
		if (FreeSize() < _bufferSize)
		{
			memcpy(&_buffer[0], &_buffer[_readPos], dataSize);
			_readPos = 0;
			_writePos = dataSize;
		}
	}
}

bool RecvBuffer::OnRead(int32 numOfBytes)
{
	if (numOfBytes > DataSize())
	{
		return false;
	}

	_readPos += numOfBytes;
	return true;
}

bool RecvBuffer::OnWrite(int32 numOfBytes)
{
	if (numOfBytes > FreeSize())
	{ 
		return false;
	}

	_writePos += numOfBytes;
	return true;
}
