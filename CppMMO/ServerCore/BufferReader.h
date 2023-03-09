#pragma once
// -----------------------
//		BufferReader
// -----------------------
// C#의 ArraySegment와 유사하다고 보면 된다. 할당된 메모리를 잘라서 쓸 수 있게 해준다.
class BufferReader
{
public:
	BufferReader();
	BufferReader(BYTE* buffer, uint32 size, uint32 pos = 0);
	~BufferReader();

	BYTE*			Buffer() { return _buffer; }
	uint32			Size() { return _size; }
	uint32			ReadSize() { return _pos; }
	uint32			FreeSize() { return _size - _pos; }

	// Peek를 편하게 쓰기 위한 템플릿 버전
	template<typename T>
	bool			Peek(T* dest) { return Peek(dest, sizeof(T)); }
	bool			Peek(OUT void* dest, uint32 len);

	// Read를 편하게 쓰기 위한 템플릿 버전
	template<typename T>
	bool			Read(T* dest) { return Read(dest, sizeof(T)); }
	bool			Read(OUT void* dest, uint32 len); 

	template<typename T>
	BufferReader&	operator>>(OUT T& dest);

private:
	BYTE*			_buffer = nullptr;
	uint32			_size = 0;
	uint32			_pos = 0;
};

// BufferReader >> T 와 같은 형식으로 사용된다.
// 왼쪽 -> 오른쪽의 결합순서를 갖는다. ex) BufferReader >> T >> B 면, (BufferReader >> T) >> B라고 보면 된다.
// BufferReader&를 리턴하지 않으면 복사생성자가 호출되니 유의하자.
template<typename T>
inline BufferReader& BufferReader::operator>>(OUT T& dest)
{
	dest = *reinterpret_cast<T*>(&_buffer[_pos]);
	_pos += sizeof(T);
	return *this;
}

