#pragma once
#include "Types.h"

// dynamic_cast�� ����ؼ� ��Ӱ��迡 �ִ� Ŭ������ ������ ����ȯ�� �����ϴ�.
// ������ �̰��� ������ ������ static_cast�� ���� ���� ��ȣ�Ǵµ�, static_cast���� ���� �߸��� ����ȯ�� �� �� �ִ�.
// �̷��� static_cast�� dynamic_cast�� ������ �����ϰ� ������ ���ؼ� ����ȯ�� �ϴ� ���� �ش� ����� �ִ� �ڵ���� �����̴�.

#pragma region TypeList
// �̷��� �⺻���� ������ ������ TypeList<T, U...> �κ� Ư��ȭ���� Tail �κ��� Ư��ȭ���� ���Ѵ�.
// Tuple�� �̷� ������ ������� ���� ��������.
template<typename... T>
struct TypeList;

template<typename T, typename U>
struct TypeList<T, U>
{
	using Head = T;
	using Tail = U;
};

template<typename T, typename... U>
struct TypeList<T, U...>
{
	using Head = T;
	using Tail = TypeList<U...>;
};
#pragma endregion

#pragma region Length
template<typename T>
struct Length;

// TypeList<>�� ��� ���� ���� ���Ǹ� ���صξ ������ �����ϸ� ������ ���´�.
// ������ ���ø��� ���̴� Ÿ�����θ� ���̸� Length<TypeList<>>�� �ۼ��ص� ������ �߻����� �ʴ´ٴ� ���� �˾Ƶ���.
template<>
struct Length<TypeList<>>
{
	enum { value = 0 };
};

template<typename T, typename... U>
struct Length<TypeList<T, U...>>
{
	enum { value = 1 + Length<TypeList<U...>>::value };
};
#pragma endregion

#pragma region TypeAt
template<typename TL/*TypeList*/, int32 index>
struct TypeAt;

// TypeList���� N��° Ÿ���� ����, 0������ ����
template<typename Head, typename... Tail>
struct TypeAt<TypeList<Head, Tail...>, 0>
{
	using Result = Head;
};

template<typename Head, typename... Tail, int32 index>
struct TypeAt<TypeList<Head, Tail...>, index>
{
	using Result = typename TypeAt<TypeList<Tail...>, index - 1>::Result;
};

#pragma endregion

// ���� TypeAt�� index�� ���ؼ� type�� �����ϴ� ���̶��, 
// �Ʒ��� type�� �� ��° index������ ã�´�. �� ã�´ٸ� index�� -1�̴�.
#pragma region IndexOf
template<typename TL, typename T>
struct IndexOf;

template<typename... Tail, typename T>
struct IndexOf<TypeList<T, Tail...>, T>
{
	enum { value = 0 };
};

template <typename T>
struct IndexOf<TypeList<>, T>
{
	enum { value = -1 };
};

template<typename Head, typename... Tail, typename T>
struct IndexOf<TypeList<Head, Tail...>, T>
{
private:
	enum { temp = IndexOf<TypeList<Tail...>, T>::value };

public:
	enum { value = (temp == -1) ? -1 : temp + 1 };
};
#pragma endregion

// From���� To�� ��ȯ�� �����ϴٸ� exists�� 1, �ƴ϶�� 0�� �Ҵ�ȴ�.
// ���ǰ� �������ٺ��� �̷� ���� �ִµ�, ������ std�� is_convertible_v�� ���� �� �� ����.
#pragma region IsConvertible
template<typename From, typename To>
class IsConvertible
{
private:
	using Small = __int8;
	using Big = __int32;

	// From�� To�� ��ȯ�� �����ϴٸ� Test(MakeFrom())�� Small Test(const To&)�� �߷��� �ǰ�,
	// sizeof�� ������� ���� �Ѵ� Small�� �����ϱ⿡ exists�� 1�� �ȴ�.
	static Small Test(const To&) { return 0; }
	static Big Test(...) { return 0; }
	static From MakeFrom() { return 0; }

public:
	enum
	{
		exists = sizeof(Test(MakeFrom())) == sizeof(Small)
	};
};
#pragma endregion

#pragma region TypeCast
template<int32 v>
struct Int2Type
{
	enum { value = v };
};

template<typename TL>
class TypeConversion
{
public:
	enum
	{
		length = Length<TL>::value
	};

	TypeConversion()
	{
		MakeTable(Int2Type<0>(), Int2Type<0>());
	}

	// for���� ���鼭 s_convert�� ��� ��Ҹ� �Ҵ��ϰ� ������,
	// ���ø��� ���� �̰��� �Ϸ��� ��¿ �� ���� ��������� ����� �Ѵ�.
	// �׷��� ������׿��� �Ʒ��� �ڵ尡 �ۼ��Ǿ��ٴ� ���� ��������.
	template<int32 i, int32 j>
	static void MakeTable(Int2Type<i>, Int2Type<j>)
	{
		using FromType = typename TypeAt<TL, i>::Result;
		using ToType = typename TypeAt<TL, j>::Result;

		if (IsConvertible<const FromType*, const ToType*>::exists)
		{
			s_convert[i][j] = true;
		}
		else
		{
			s_convert[i][j] = false;
		}

		MakeTable(Int2Type<i>(), Int2Type<j + 1>());
	}

	// ������� for������ i+1�ϰ� j = 0���� �ٲٴ� �κ�
	template<int32 i>
	static void MakeTable(Int2Type<i>, Int2Type<length>)
	{
		MakeTable(Int2Type<i + 1>(), Int2Type<0>());
	}

	// ������� for�� ��������
	template<int j>
	static void MakeTable(Int2Type<length>, Int2Type<j>)
	{
	}

	static inline bool CanConvert(int32 from, int32 to)
	{
		static TypeConversion conversion;
		return s_convert[from][to];
	}

public:
	static bool s_convert[length][length];
};

template<typename TL>
bool TypeConversion<TL>::s_convert[length][length];

// dynamic_castó�� ���̴� TypeCast��� ���� �ȴ�.
// dynamic_cast�� �ٸ� ���� s_convert ���̺��� ������ �ð��� �̸� ���Ǿ� �־ �ӵ��� �ξ� ������!
template<typename To, typename From>
To TypeCast(From* ptr)
{
	if (ptr == nullptr)
	{
		return nullptr;
	}

	using TL = typename From::TL;

	if (TypeConversion<TL>::CanConvert(ptr->_typeId, IndexOf<TL, remove_pointer_t<To>>::value))
	{
		return static_cast<To>(ptr);
	}

	return nullptr;
}

template<typename To, typename From>
shared_ptr<To> TypeCast(shared_ptr<From> ptr)
{
	if (ptr == nullptr)
	{
		return nullptr;
	}

	using TL = typename From::TL;

	if (TypeConversion<TL>::CanConvert(ptr->_typeId, IndexOf<TL, remove_pointer_t<To>>::value))
	{
		return static_pointer_cast<To>(ptr);
	}

	return nullptr;
}

// ��Ÿ�ӿ� �ش� Ÿ������ ����� �� �ִ����� Ȯ�����ش�.
template<typename To, typename From>
bool CanTypeCast(From* ptr)
{
	if (ptr == nullptr)
	{
		return false;
	}

	using TL = typename From::TL;

	return TypeConversion<TL>::CanConvert(ptr->_typeId, IndexOf<TL, remove_pointer_t<To>>::value);
}

template<typename To, typename From>
bool CanTypeCast(shared_ptr<From> ptr)
{
	if (ptr == nullptr)
	{
		return false;
	}

	using TL = typename From::TL;

	return TypeConversion<TL>::CanConvert(ptr->_typeId, IndexOf<TL, remove_pointer_t<To>>::value);
}

#pragma endregion

#define DECLARE_TL		using TL = TL; int32 _typeId;
#define INIT_TL(Type)	_typeId = IndexOf<TL, Type>::value;