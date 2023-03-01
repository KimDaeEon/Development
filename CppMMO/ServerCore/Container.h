#pragma once
#include "Types.h"
#include "Allocator.h"
#include <vector>
#include <array>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
using namespace std;

template<typename Type, uint32 Size>
using myArray = array<Type, Size>;

template<typename Type>
using myVector = vector<Type, StlAllocator<Type>>;

template<typename Type>
using myList = list<Type, StlAllocator<Type>>;

template<typename Key, typename Value, typename Pred = less<Key>>
using myMap = map<Key, Value, Pred, StlAllocator<pair<const Key, Value>>>;

template<typename Key, typename Value, typename Hasher = hash<Key>, typename KeyEqual = equal_to<Key>>
using myUnorderedMap = unordered_map<Key, Value, Hasher, KeyEqual, StlAllocator<pair<const Key, Value>>>;

template<typename Key, typename Pred = less<Key>>
using mySet = set<Key, Pred, StlAllocator<Key>>;

template<typename Key, typename Hasher = hash<Key>, typename KeyEqual = equal_to<Key>>
using myUnorderedSet = unordered_set<Key, Hasher, KeyEqual, StlAllocator<Key>>;

template<typename Type>
using myDeque = deque<Type, StlAllocator<Type>>;

template<typename Type, typename Container = myDeque<Type>>
using myQueue = queue<Type, Container>;

template<typename Type, typename Container = myDeque<Type>>
using myStack = stack<Type, Container>;

template<typename Type, typename Container = myVector<Type>, typename Pred = less<typename Container::value_type>>
using myPriorityQueue = priority_queue<Type, Container, Pred>;

using myString = basic_string<char, char_traits<char>, StlAllocator<char>>;
using myWString = basic_string<wchar_t, char_traits<wchar_t>, StlAllocator<wchar_t>>;