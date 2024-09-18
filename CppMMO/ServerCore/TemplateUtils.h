#pragma once
#include "Types.h"
// Ÿ���� Ÿ�� ����Ʈ �� �ϳ��� ��ġ�ϴ��� �˻��ϴ� ��Ÿ �Լ�
template<typename T, typename... Ts>
struct is_one_of : std::false_type {};

template<typename T, typename First, typename... Rest>
struct is_one_of<T, First, Rest...> : std::conditional_t<std::is_same_v<T, First>, std::true_type, is_one_of<T, Rest...>> {};

template<typename T, typename... Ts>
inline constexpr bool is_one_of_v = is_one_of<T, Ts...>::value;


// Helper ��Ÿ �Լ�: ������ Ÿ���� �� �� �̻� ���Ǵ��� �˻�
template <typename... Ts>
struct HasDuplicates;

template <>
struct HasDuplicates<> : std::false_type {};

template <typename T, typename... Rest>
struct HasDuplicates<T, Rest...>
	: std::integral_constant<bool, (std::is_same_v<T, Rest> || ...) || HasDuplicates<Rest...>::value>
{
};