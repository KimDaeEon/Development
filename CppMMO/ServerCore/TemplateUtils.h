#pragma once
#include "Types.h"
// 타입이 타입 리스트 중 하나와 일치하는지 검사하는 메타 함수
template<typename T, typename... Ts>
struct is_one_of : std::false_type {};

template<typename T, typename First, typename... Rest>
struct is_one_of<T, First, Rest...> : std::conditional_t<std::is_same_v<T, First>, std::true_type, is_one_of<T, Rest...>> {};

template<typename T, typename... Ts>
inline constexpr bool is_one_of_v = is_one_of<T, Ts...>::value;


// Helper 메타 함수: 동일한 타입이 두 번 이상 사용되는지 검사
template <typename... Ts>
struct HasDuplicates;

template <>
struct HasDuplicates<> : std::false_type {};

template <typename T, typename... Rest>
struct HasDuplicates<T, Rest...>
	: std::integral_constant<bool, (std::is_same_v<T, Rest> || ...) || HasDuplicates<Rest...>::value>
{
};