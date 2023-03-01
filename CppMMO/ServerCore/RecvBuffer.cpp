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

	// _readPos == _writePos�� ��� �� �� ��ġ�� 0���� ����
	if (dataSize == 0)
	{
		_readPos = _writePos = 0;
	}
	else
	{
		// ���� ������ ���� 1�� ũ�� �̸��̸� �����͸� ������ ����.
		// _capacity�� ���ؼ� �� �κ��� ����� Ȯ���� ��������.
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
