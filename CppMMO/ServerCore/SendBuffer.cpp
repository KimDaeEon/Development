#include "pch.h"
#include "SendBuffer.h"

// ----------------------------
//			SendBuffer
// ----------------------------


SendBuffer::SendBuffer(SendBufferChunkRef owner, BYTE* buffer, uint32 allocSize)
	: _owner(owner), _buffer(buffer), _allocSize(allocSize)
{
}

SendBuffer::~SendBuffer()
{
}

void SendBuffer::Close(uint32 writeSize)
{
	ASSERT_CRASH(_allocSize >= writeSize);
	_writeSize = writeSize; 
	_owner->Close(writeSize);
}


// ----------------------------------
//			SendBufferChunk
// ----------------------------------
SendBufferChunk::SendBufferChunk()
{
}

SendBufferChunk::~SendBufferChunk()
{
	// ���⿡�� ������ �ʴ´�. ��� ��Ȱ��ȴ�.
}

void SendBufferChunk::Reset()
{
	_isOpen = false;
	_usedSize = 0;
}

SendBufferRef SendBufferChunk::Open(uint32 allocSize)
{
	ASSERT_CRASH(allocSize <= SEND_BUFFER_CHUNK_SIZE);
	ASSERT_CRASH(_isOpen == false);

	if (allocSize > FreeSize())
	{
		return nullptr;
	}

	_isOpen = true;

	// ObjectPool���� SendBuffer ��ü�� �����ϰ�, SendBuffer���� Buffer�� ���� ���� �����͸� ����Ѵ�.
	// ��� SendBuffer���� �޸𸮸� �� ����Ͽ��� SendBufferChunk ������ �����ϸ�(�� ����ϸ�) 
	// SendBufferManager���� ����ϴ� _sendBufferChunks�� �޸𸮰� ���ư���.
	return ObjectPool<SendBuffer>::MakeShared(shared_from_this(), Buffer(), allocSize);
}

void SendBufferChunk::Close(uint32 writeSize)
{
	ASSERT_CRASH(_isOpen == true);
	_isOpen = false;
	_usedSize += writeSize;
}


// -----------------------------------
//			SendBufferManager
// -----------------------------------
SendBufferRef SendBufferManager::Open(uint32 size)
{
	if (LSendBufferChunk == nullptr)
	{
		LSendBufferChunk = Pop();
	}

	ASSERT_CRASH(LSendBufferChunk->IsOpen() == false);
	
	// �� ������ �� ���۷� ��ü
	if (LSendBufferChunk->FreeSize() < size)
	{
		LSendBufferChunk = Pop();
		LSendBufferChunk->Reset();
	}

	//cout << "FREE : " << LSendBufferChunk->FreeSize() << endl;

	return LSendBufferChunk->Open(size);
}

SendBufferChunkRef SendBufferManager::Pop()
{
	{
		LockGuard lg(_mutex);
		if (_sendBufferChunks.empty() == false)
		{
			SendBufferChunkRef sendBufferChunk = _sendBufferChunks.back();
			_sendBufferChunks.pop_back();
			return sendBufferChunk;
		}
	}

	return SendBufferChunkRef(myNew<SendBufferChunk>(), PushGlobal /*�Ҹ��ڷ� PushGlobal ���*/);
}

void SendBufferManager::Push(SendBufferChunkRef buffer)
{
	LockGuard lg(_mutex);
	_sendBufferChunks.push_back(buffer);
}

void SendBufferManager::PushGlobal(SendBufferChunk* buffer)
{
	GSendBufferManager->Push(SendBufferChunkRef(buffer, PushGlobal));
}
