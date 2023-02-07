#pragma once
#include "Types.h"

// dynamic_cast를 사용해서 상속관계에 있는 클래스를 안전히 형변환이 가능하다.
// 하지만 이것은 성능이 느려서 static_cast를 쓰는 것이 선호되는데, static_cast같은 경우는 잘못된 형변환을 할 수 있다.
// 이러한 static_cast와 dynamic_cast의 단점을 보완하고 장점만 취해서 형변환을 하는 것이 해당 헤더에 있는 코드들의 목적이다.

#pragma region TypeList
// 이렇게 기본형을 만들지 않으면 TypeList<T, U...> 부분 특수화에서 Tail 부분이 특수화되지 못한다.
// Tuple도 이런 식으로 만들었던 것을 참고하자.
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

// TypeList<>의 경우 위에 보면 정의를 안해두어서 변수로 선언하면 에러가 나온다.
// 하지만 템플릿에 쓰이는 타입으로만 쓰이면 Length<TypeList<>>로 작성해도 에러가 발생하지 않는다는 것을 알아두자.
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

// TypeList에서 N번째 타입을 추출, 0번부터 시작
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

// 앞의 TypeAt이 index를 통해서 type을 추출하는 것이라면, 
// 아래는 type이 몇 번째 index인지를 찾는다. 못 찾는다면 index는 -1이다.
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

// From에서 To로 변환이 가능하다면 exists는 1, 아니라면 0이 할당된다.
// 강의가 예전꺼다보니 이런 것이 있는데, 지금은 std의 is_convertible_v를 쓰면 될 것 같다.
#pragma region IsConvertible
template<typename From, typename To>
class IsConvertible
{
private:
	using Small = __int8;
	using Big = __int32;

	// From이 To로 변환이 가능하다면 Test(MakeFrom())이 Small Test(const To&)로 추론이 되고,
	// sizeof를 사용했을 때에 둘다 Small을 리턴하기에 exists는 1이 된다.
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

	// for문을 돌면서 s_convert의 모든 요소를 할당하고 싶은데,
	// 템플릿을 통해 이것을 하려면 어쩔 수 없이 재귀형식을 따라야 한다.
	// 그러한 제약사항에서 아래의 코드가 작성되었다는 것을 염두하자.
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

	// 재귀형식 for문에서 i+1하고 j = 0으로 바꾸는 부분
	template<int32 i>
	static void MakeTable(Int2Type<i>, Int2Type<length>)
	{
		MakeTable(Int2Type<i + 1>(), Int2Type<0>());
	}

	// 재귀형식 for문 종료조건
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

// dynamic_cast처럼 쓰이는 TypeCast라고 보면 된다.
// dynamic_cast와 다른 점은 s_convert 테이블이 컴파일 시간에 미리 계산되어 있어서 속도가 훨씬 빠르다!
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

// 런타임에 해당 타입으로 변경될 수 있는지를 확인해준다.
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