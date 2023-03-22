#pragma once
// -----------------------
//		BufferWriter
// -----------------------
class BufferWriter
{
public:
	BufferWriter();
	BufferWriter(BYTE* buffer, uint32 size, uint32 pos = 0);
	~BufferWriter();

	BYTE*			Buffer() { return _buffer; }
	uint32			Size() { return _size; }
	uint32			WriteSize() { return _pos; }
	uint32			FreeSize() { return _size - _pos; }

	// [헤더][데이터] 이런 식으로 메모리를 쓸 때에 [헤더] 부분을 비워두기 위한 함수
	template<typename T>
	T*				Reserve();

	// Write를 편하게 쓰기 위한 템플릿 버전
	template<typename T>
	bool			Write(T* src) { return Write(src, sizeof(T)); }
	bool			Write(void* src, uint32 len);

	template<typename T>
	BufferWriter&	operator<<(T&& src);

private:
	BYTE*			_buffer = nullptr;
	uint32			_size = 0;
	uint32			_pos = 0;
};

template<typename T>
inline T* BufferWriter::Reserve()
{
	if (FreeSize() < sizeof(T))
	{
		return nullptr;
	} 

	T* ret = reinterpret_cast<T*>(&_buffer[_pos]);
	_pos += sizeof(T);
	return ret;
}

template<typename T>
inline BufferWriter& BufferWriter::operator<<(T&& src)
{
	// 여기서 forward 쪽에 remove_reference_t<T>로 하면 안된다는 것 주의하자.
	*reinterpret_cast<std::remove_reference_t<T>*>(&_buffer[_pos]) = std::forward<T>(src);
	_pos += sizeof(T);
	return *this;
}