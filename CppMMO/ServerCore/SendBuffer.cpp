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
	// 여기에는 들어오지 않는다. 계속 재활용된다.
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

	// ObjectPool에서 SendBuffer 개체를 관리하고, SendBuffer에서 Buffer를 통해 받은 데이터를 사용한다.
	// 모든 SendBuffer에서 메모리를 다 사용하여서 SendBufferChunk 참조를 해제하면(다 사용하면) 
	// SendBufferManager에서 사용하는 _sendBufferChunks로 메모리가 돌아간다.
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
	
	// 다 썼으면 새 버퍼로 교체
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

	return SendBufferChunkRef(myNew<SendBufferChunk>(), PushGlobal /*소멸자로 PushGlobal 사용*/);
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
