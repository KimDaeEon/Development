#pragma once
// ���򿡴� �޸� �Ҵ�Ⱑ �� �Ǿ��־ MemoryPool�� �Ⱦ��� ��쵵 �ִٰ� �Ѵ�.

enum
{
	SLIST_ALIGNMENT = 16
};

// -----------------------
//		MemoryHeader
// -----------------------
// DECLSPEC_ALIGH(16)�� ���� struct�� 16����Ʈ�� ����� ��������� �ȴ�.
// �̷��� �ϴ� ������ CPU���� ������ 16����Ʈ ����� �ڷᱸ���� ���� ����ȭ�� �Ǿ� �ְ�,
// �̿� ���缭 Microsoft���� ���� Lock Free �˰��� �۵��ϱ� �����̴�.
DECLSPEC_ALIGN(SLIST_ALIGNMENT)
struct MemoryHeader : public SLIST_ENTRY
{
	// [MemoryHeader][Data] �������� �޸𸮰� �Ҵ��� �ȴ�.
	MemoryHeader(int32 size) : allocSize(size) {}
	
	// �Ҵ��� �޸𸮿� ����� �޾��ְ�, ����ϴ� �������� �޸� �ּ� ����
	static void* AttachHeader(MemoryHeader* header, int32 size)
	{
		new(header)MemoryHeader(size);
		return reinterpret_cast<void*>(++header); // header*��ŭ �ǳʶپ ������ �ּҸ� ����
	}

	static MemoryHeader* DetachHeader(void* ptr)
	{
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}

	int32 allocSize;
	// TODO: ���Ŀ� �ʿ��� ���� �߰��� �� ����
};

// --------------------
//		MemoryPool
// --------------------
DECLSPEC_ALIGN(SLIST_ALIGNMENT)
class MemoryPool
{
public:
	MemoryPool(int32 allocSize);
	~MemoryPool();

	void			Push(MemoryHeader* ptr);
	MemoryHeader*	Pop();

private:
	//USE_LOCK;
	SLIST_HEADER			_header; // Lock-Free Stack�� �����ϴ� ����ü
	int32					_allocSize = 0;
	atomic<int32>			_usedCount = 0; // �޸�Ǯ�� �Ҵ�� �޸� ��� �� ���Ǵ� ����
	atomic<int32>			_reservedCount = 0; // �޸�Ǯ�� �Ҵ�� �޸� ��� �� ������ ���� ����
};

