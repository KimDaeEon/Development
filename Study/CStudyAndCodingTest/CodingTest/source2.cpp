#pragma once
#include <cassert> // assert를 사용하기 위한 헤더
#include <stdio.h>
#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <regex>
#include <bitset>
using namespace std;

// 기본적 참고 사항 사항
// 뭔가 테스트 케이스가 일부만 맞고 일부만 틀린 경우가 있다면 if 문 쪽의 논리가 틀리지는 않았는지 확인하자.

// index 라는 것은 어떻게 보면 시간 순서도 될 수 있다. 풀었던 스택과 큐 문제의 내용을 생각하자.
// 어떠한 정확한 정보를 구하는 것이 아니라, 갯수, 길이만 구하는 것이면 훨씬 계산이 단순화될 수 있다.

// unordered_map 은 key value 를 넣은 순서대로 auto [k,v] 를 하면 key 가 뽑히고, map 은 key 가 정렬되어서 뽑힌다. 은근 쓸곳이 생기니 기억해두자.

// unsigned int 와 int 가 < 비교로 들어가선 안된다. 예를 들어 -1 < (unsigned int)0 을 하면, 비교 연산이 unsigned int 로 묶여서 들어가서 -1이 엄청나게 큰 수가 되어버린다.
// 비교를 할 때에는 변수 형태를 꼭 통일해서 실시하자.

// vector<vector<int>> 를 초기값을 주려 한다면, vector<vector<int>> v (5, vector<int>(5, 0)) 이런 식으로 하면 된다.

// priority_queue<int, vector<int>, greater<int>> pq; <- 이렇게 하면 작은 녀석부터 pq.top() 에서 나온다. 반대로 하려면 less<int> 넣자.
// 헷갈리는 것이 vector 를 sort 할 때에 작은 것 -> 큰 것으로 하려면 less<int> 를 넣어야 한다는 것이다. 
// sort 는 true 면 안바꾸고, heap 을 쓰는 pq 는 true 면 위치 변환을 하기 때문에 이렇게 된다.

// multiset 의 경우 minmax 를 구할 때의 용도로 활용될 수 있다.
// substr 은 처음에 변수가 offset 위치고, 그 다음은 count 이다. a.substr(1,3) 이러면 1번째 index 를 포함해서 3개의 문자를 가지는 string 을 리턴한다.

// 함수 내부에서 반복적으로 생성될 필요 없는 전역변수는 바깥으로 빼주자.

// 최대 크기나 최소 크기만 알면 된다면 max_element, min_element를 쓰자. sort 하는 것보다 연산량을 많이 줄일 수 있다.

// STL 컨테이너 간에는 상호 변환이 가능한데, 이러한 경우 생성자에서 바로 컨테이너 간에 변환이 이루어지도록 하자. ex) unordered_set<int> us(v.begin(), v.end());

// DFS 는 스택, BFS 는 queue 를 통해서 구현할 수 있다.

// vector 는 () 으로 크기를 지정하여 생성하면 기본 인자들이 전부 다 0으로 초기화된다. vector<pair<int,int>> 이렇게 하면 pair 가 각각 다 0으로 초기화된다.


// 아래는 문제ㅐ 풀 때에 반복적으로 사용하거나 기억해두면 좋을 함수들
namespace Utils
{
	// DFS 로 순열 구하기 예시 코드
	void permute(vector<int>& nums, int startLevel, int targetLevel)
	{
		if (startLevel == targetLevel)
		{
			// 순열 출력
			for (int num : nums)
			{
				cout << num << " ";
			}
			cout << endl;
		}
		else
		{
			for (int i = startLevel; i <= targetLevel; i++)
			{
				swap(nums[startLevel], nums[i]);
				permute(nums, startLevel + 1, targetLevel);
				swap(nums[startLevel], nums[i]); // 백트래킹
			}
		}
	}

	// 아래는 Stack을 통해 DFS로 순열을 구하는 코드
	struct DFSState
	{
		vector<int> nums;
		int level; // 현재 레벨
	};
	void IterativePermute(vector<int> nums)
	{
		stack<DFSState> s;
		s.push({ nums, 0 }); // 초기 상태를 스택에 푸시

		while (!s.empty())
		{
			DFSState current = s.top();
			s.pop();

			int level = current.level;
			vector<int> currentNums = current.nums;

			if (level == currentNums.size() - 1)
			{
				// 순열이 완성된 상태: 출력
				for (int num : currentNums)
				{
					cout << num << " ";
				}
				cout << endl;
			}
			else
			{
				for (int i = level; i < currentNums.size(); i++)
				{
					swap(currentNums[level], currentNums[i]);  // 요소 교환
					s.push({ currentNums, level + 1 });          // 다음 상태를 스택에 푸시
					// 다시 교환해서 원래 상태로 돌아감 (다음 루프를 위한 준비)
					swap(currentNums[level], currentNums[i]);
				}
			}
		}
	}

}


namespace BruteForce
{
	namespace FatigueLevel
	{
		int solution(int k, vector<vector<int>> dungeons)
		{
			int answer = -1;
			sort(dungeons.begin(), dungeons.end()); // 이거 안해주면 next_permutation 에서 순회가 안된다.
			// 던전 순서 permutation 돌려서 순회하면서,
			do
			{
				int totalFatigue = k;
				int dungeonCnt = 0;
				for (auto dungeon : dungeons)
				{
					int requiredFatigue = dungeon[0];
					int tobeUsedFatigue = dungeon[1];

					if (totalFatigue >= requiredFatigue)
					{
						totalFatigue -= tobeUsedFatigue;
						dungeonCnt++;
					}
				}

				answer = max(answer, dungeonCnt);
			} while (next_permutation(dungeons.begin(), dungeons.end()));
			// 각 던전에서 소모하는 피로도 계산해서 최대 k를 구한다.

			return answer;
		}


		int answer = -1;

		void Permute(vector<vector<int>>& dungeons, int startLevel, int targetLevel, int k)
		{
			if (startLevel == targetLevel)
			{
				int totalFatigue = k;
				int dungeonCnt = 0;
				for (auto dungeon : dungeons)
				{
					int requiredFatigue = dungeon[0];
					int tobeUsedFatigue = dungeon[1];

					if (totalFatigue >= requiredFatigue)
					{
						totalFatigue -= tobeUsedFatigue;
						dungeonCnt++;
					}
				}

				answer = max(answer, dungeonCnt);
			}
			else
			{
				for (int i = startLevel; i <= targetLevel; i++)
				{
					swap(dungeons[startLevel], dungeons[i]);
					Permute(dungeons, startLevel + 1, targetLevel, k);
					swap(dungeons[startLevel], dungeons[i]); // 백트래킹
				}
			}
		}

		int solutionPermute(int k, vector<vector<int>> dungeons)
		{
			Permute(dungeons, 0, dungeons.size() - 1, k);
			return answer;
		}
	}
}

namespace ProgrammersBasicTraining
{
	// 등차수열의 특정한 항만 더하기
	namespace _1
	{
		// included[i] 가 true인 경우 a[i+1] = a + n*d 를 다 더해서 리턴
		int solution(int a, int d, vector<bool> included)
		{
			int answer = 0;

			for (int i = 0; i < included.size(); i++)
			{
				if (included[i])
				{
					answer += (a + i * d);
				}
			}
			return answer;
		}
	}

	namespace _2
	{
		// 세 숫자가 모두 다르면 a + b + c
		// 2개가 같고 나머지가 다른 숫자면 (a + b + c) * (a^2 + b^2 + c^2)
		// 세 숫자가 모두 같다면 (a + b + c) * (a^2 + b^2 + c^2) * (a^3 + b^3 + c^3)

		// 추가 참고: 이거 이렇게 같은 것을 찾을 때에는 Set을 활용할 수도 있음
		int solution(int a, int b, int c)
		{
			int t1 = a + b + c;
			int t2 = t1 * (a * a + b * b + c * c);
			int t3 = t2 * (a * a * a + b * b * b + c * c * c);

			if (a != b && b != c && a != c)
			{
				return t1;
			}
			// 세 숫자가 모두 같은 경우
			else if (a = b && b == c && c == a)
			{
				return t3;
			}
			// 나머지 경우
			else
			{
				return t2;
			}
		}


	}

	namespace _3
	{
		// 팩토리얼보다 전체 합 ^ 2이 작으면 1 리턴
		int solution(vector<int> num_list)
		{
			int accumulatdFactorial = 1;
			int sum = 0;

			for (auto i : num_list)
			{
				accumulatdFactorial *= i;
				sum += i;
			}

			return accumulatdFactorial < sum * sum;
		}
	}

	namespace _4
	{
		// 호수만 순서대로 이어 붙인 수와 짝수만 순서대로 이어 붙인 수의 합 (문자열)
		int solution(vector<int> num_list)
		{
			int answer = 0;

			string oddString = "";
			string evenString = "";

			for (auto i : num_list)
			{
				if (i % 2 == 0)
				{
					evenString += to_string(i);
				}
				else
				{
					oddString += to_string(i);
				}
			}

			answer = stoi(evenString) + stoi(oddString);

			return answer;
		}
	}

	namespace _5
	{
		// 마지막 원소가 그 전 원소보다 크면 (마지막 원소 - 그 전 원소) 추가해서 리턴
		// 마지막 원소 <= 그 전 원소라면 (마지막 원소 * 2) 추가해서 리턴
		vector<int> solution(vector<int> num_list)
		{
			int lastIdx = num_list.size() - 1;

			int last = num_list[lastIdx];
			int preLast = num_list[lastIdx - 1];

			if (last > preLast)
			{
				num_list.push_back(last - preLast);
			}
			else
			{
				num_list.push_back(2 * last);
			}

			return num_list;
		}
	}

	namespace _6
	{
		// 수 조작하기 1
		// control 문자열 "w": n = n+1, "s": n = n-1;, "d": n = n+10, "a": n = n-10

		int solution(int n, string control)
		{
			for (auto ch : control)
			{
				switch (ch)
				{
				case 'w':
					n += 1;
					break;

				case 's':
					n -= 1;
					break;

				case'd':
					n += 10;
					break;

				case'a':
					n -= 10;
					break;
				}
			}

			return n;
		}
	}

	namespace _7
	{
		// 수 조작하기 2
		// "w": +1
		// "s": -1
		// "d": +10
		// "s": -10
		unordered_map<int, char> um{ {1,'w'}, {-1, 's'}, {10, 'd'}, {-10, 'a'} };

		string solution(vector<int> numLog)
		{
			string answer = "";

			int before = numLog[0];
			for (int i = 1; i < numLog.size(); i++)
			{
				int current = numLog[i];
				int cmdNum = current - before;
				answer.push_back(um[cmdNum]);
				before = current;
			}

			return answer;
		}
	}

	// 수열과 구간 쿼리 3
	namespace _8
	{
		// query에 있는 인덱스 끼리 스왑
		vector<int> solution(vector<int> arr, vector<vector<int>> queries)
		{
			for (const auto& vec : queries)
			{
				int idx1 = vec[0];
				int idx2 = vec[1];

				swap(arr[idx1], arr[idx2]);
			}

			return arr;
		}
	}

	// 수열과 구간 쿼리 2
	namespace _9
	{
		// 쿼리 범위에서 가장 작은 arr[i] 리턴, 없으면 -1
		vector<int> solution(vector<int> arr, vector<vector<int>> queries)
		{
			vector<int> answer;

			const int initValue = 1000000000;
			for (const auto& query : queries)
			{
				int idx1 = query[0];
				int idx2 = query[1];
				int minStandard = query[2];

				int temp = initValue;

				for (int i = idx1; i <= idx2; i++)
				{
					if (arr[i] > minStandard)
					{
						temp = min(arr[i], temp);
					}
				}

				if (temp == initValue)
				{
					answer.push_back(-1);
				}
				else
				{
					answer.push_back(temp);
				}
			}

			return answer;
		}
	}

	// 수열과 구간 쿼리 4
	namespace _10
	{
		// 쿼리 idx 0, 1 범위의 수들에 대해서, i가 K의 배수이면 arr[i]에 1을 더한다.
		vector<int> solution(vector<int> arr, vector<vector<int>> queries)
		{
			for (const auto& query : queries)
			{
				auto qr1 = query[0];
				auto qr2 = query[1];
				auto qr3 = query[2];

				if (qr3 == 0)
				{
					continue;
				}

				for (int i = qr1; i <= qr2; i++)
				{
					if (i % qr3 == 0)
					{
						arr[i] += 1;
					}
				}
			}
			return arr;
		}
	}

	// 배열 만들기2
	namespace _11
	{
		bool IsConsistedOfOnly5or0(const string& in)
		{
			for (auto ch : in)
			{
				if (!(ch == '0' || ch == '5'))
				{
					return false;
				}
			}

			return true;
		}

		// l 이상 r 이하 정수 중, 숫자 "0"과 "5"로만 이루어진 모든 정수를 오름차순으로 정렬한 배열 return, 없다면 -1 담아 리턴
		//vector<int> solution(int l, int r)
		//{
		//	vector<int> answer;
		//	
		//	for (int i = l; i <= r; i++)
		//	{
		//		string temp = to_string(i);

		//		if (IsConsistedOfOnly5or0(temp))
		//		{
		//			answer.push_back(i);
		//		}
		//	}


		//	if (answer.empty())
		//	{
		//		answer.push_back(-1);
		//	}

		//	return answer;
		//}


		// 아래는 더 빠른 풀이

		bool CheckDividedBy10AndN(int input, int n)
		{
			int temp = input;

			// 10씩 나누면서 1의 자리 수가 모두 0 또는 n인지 확인
			while (temp > 0)
			{
				int quotient = temp / 10;
				int remainderBy10 = temp % 10;
				int remainderBy5 = remainderBy10 % n;

				if (remainderBy10 == 0 || remainderBy5 == 0)
				{
					temp = quotient;
				}
				else
				{
					return false;
				}
			}

			return true;
		}

		vector<int> solution(int l, int r)
		{
			vector<int> answer;

			for (int i = l; i <= r; i++)
			{
				if (CheckDividedBy10AndN(i, 5))
				{
					answer.push_back(i);
				}
			}


			if (answer.empty())
			{
				answer.push_back(-1);
			}

			return answer;
		}
	}

	// 카운트 업
	namespace _12
	{
		vector<int> solution(int start_num, int end_num)
		{
			auto len = end_num - start_num;
			vector<int> answer;
			answer.reserve(len);

			for (int i = start_num; i <= end_num; i++)
			{
				answer.push_back(i);
			}

			return answer;
		}
	}

	// 콜라츠 수열 만들기
	namespace _13
	{
		// n이 짝수일 때 2로 나눔
		// n이 홀 수일때 3*n + 1
		vector<int> solution(int n)
		{
			vector<int> answer;
			while (true)
			{
				answer.push_back(n);

				if (n == 1)
				{
					break;
				}

				if (n % 2 == 0)
				{
					n = n / 2;
				}
				else
				{
					n = 3 * n + 1;
				}
			}

			return answer;
		}
	}

	// 배열 만들기 4
	namespace _14
	{
		// i = 0
		// i < arr.size()면 반복 작업 수행
		// stk 빈 배열, arr[i]를 stk에 추가 i=i+1
		// stk 원소 있고, stk 마지막 원소 < arr[i], arr[i]를 stk에 추가, i=i+1
		// stk 원소 있고, stk 마지막 원소 >= arr[i], stk 마지막 원소 stk에서 제거
		vector<int> solution(vector<int> arr)
		{
			vector<int> stk;
			for (int i = 0; i < arr.size();)
			{
				if (stk.empty())
				{
					stk.push_back(arr[i]);
					i++;
				}
				else
				{
					if (stk.back() < arr[i])
					{
						stk.push_back(arr[i]);
						i++;
					}
					else
					{
						stk.pop_back();
					}
				}
			}
			return stk;
		}
	}

	// 간단한 논리 연산
	namespace _15
	{
		bool solution(bool x1, bool x2, bool x3, bool x4)
		{
			return (x1 || x2) && (x3 || x4);
		}
	}

	// 주사위 게임 3
	namespace _16
	{
		// 4 주사위 수가 모두 p로 같다면 1111*p 점을 얻음
		// 3 주사위에서 나온 숫자가 P로 같고, 나머지 다른 주사위에서 나온 숫자가 q라면 (10*p+q)^2 점을 얻음
		// 2 주사위가 2개씩 값이 같고, 각각 나온 값이 p, q 이면 (p+q) * abs(p-q) 점을 얻음
		// 2 주사위가 p이고, 나머지 두 주사위에서 나온 숫자가 각각 p가 아닌 다른 값 q,r 이라면 q*r 점을 얻음
		// 네 주사위에 적힌 숫자가 모두 다르면 가장 작은 숫자 만큼의 점수를 얻음


		// 내일 다시 map 으로 접근해서 풀어보기
		int solution(int a, int b, int c, int d)
		{
			// 주사위 눈이 같은 경우의 수를 기록
			unordered_map<int, int> um;

			um[a]++;
			um[b]++;
			um[c]++;
			um[d]++;

			int setCnt = um.size();

			// 4 주사위가 같은 경우 (size = 1)
			if (setCnt == 1)
			{
				return 1111 * a;
			}

			if (setCnt == 2)
			{
				int p = -1;
				int q;

				for (const auto& [val, cnt] : um)
				{
					// 3 주사위가 같고, 1개만 다른 경우 (size = 2)
					if (cnt == 3)
					{
						p = val;
						continue;
					}
					
					if (cnt == 1)
					{
						q = val;
						continue;
					}

					// 2 주사위씩 같은 경우 (size = 2)
					if (cnt == 2 && p == -1)
					{
						p = val;
						continue;
					}

					if (cnt == 2)
					{
						q = val;
					}
				}

				if (um[a] == 1 || um[a] == 3)
				{
					return (10 * p + q) * (10 * p + q);
				}
				else
				{
					return (p + q) * abs(p - q);
				}
			}

			// 2 주사위는 같고, 나머지 2개는 각각 다른 경우 (size = 3)
			if (setCnt == 3)
			{
				int p;
				int q = -1;
				int r;

				for (auto [val, cnt] : um)
				{
					if (cnt == 2)
					{
						p = val;
						continue;
					}
					
					if (cnt == 1 && q == -1)
					{
						q = val;
						continue;
					}

					if (cnt == 1)
					{
						r = val;
					}
				}

				return q * r;
			}
	
			// 모든 주사위가 다른 경우 (size = 4)

			int minVal = min(a, b);
			minVal = min(minVal, c);
			minVal = min(minVal, d);

			return minVal;
		}
	}

	// 글자 이어 붙여 문자열 만들기
	namespace _17
	{
		string solution(string my_string, vector<int> index_list)
		{
			string answer = "";

			for (const auto i : index_list)
			{
				answer += my_string[i];
			}

			return answer;
		}
	}

	// 9로 나눈 나머지
	namespace _18
	{
		int solution(string number)
		{
			int answer = 0;

			for (const auto ch : number)
			{
				answer += ch - '0';
			}

			answer = answer % 9;

			return answer;
		}
	}

	// 문자열 여러 번 뒤집기
	namespace _19
	{
		string solution(string my_string, vector<vector<int>> queries)
		{
			for (const auto& query : queries)
			{
				int left = query[0];
				int right = query[1];

				while (left < right)
				{
					swap(my_string[left], my_string[right]);
					left++;
					right--;
				}
			}
			return my_string;
		}
	}
}

namespace ListTest
{
	struct ListNode
	{
		int data;
		ListNode* next;

		ListNode(int value, ListNode* next = nullptr)
			: data(value), next(next)
		{
			cout << "ListNode()" << endl;
		}

		~ListNode()
		{
			cout << "~ListNode()" << endl;
		}
	};

	struct MyList
	{
		ListNode* head = nullptr;
		ListNode* tail = nullptr;
		int size = 0;

		ListNode* MoveToNthNode(int nth)
		{
			if (nth < 0 || nth >= size)
			{
				// 유효하지 않은 인덱스
				return nullptr;
			}

			ListNode* cur = head;

			for (int i = 0; i < nth; i++)
			{
				if (cur->next)
				{
					cur = cur->next;
				}
				else
				{
					// n 번째 있는 요소 찾지 못함
					return nullptr;
				}
			}

			return cur;
		}

		void Delete()
		{
			ListNode* cur = head;

			while (cur)
			{
				ListNode* deletedNode = cur;
				cur = cur->next;
				delete deletedNode;
				size--;
			}

			head = nullptr;
			tail = nullptr;
		}

		bool insertAfter(ListNode* targetNode, ListNode* addedNode)
		{
			if (targetNode == nullptr || addedNode == nullptr)
			{
				return false;
			}

			if (targetNode == tail)
			{
				tail = addedNode;
			}

			addedNode->next = targetNode->next;

			targetNode->next = addedNode;

			size++;

			return true;
		}

		void PushFront(ListNode* addedNode)
		{
			if (addedNode == nullptr)
			{
				return;
			}

			if (size == 0)
			{
				head = addedNode;
				tail = addedNode;
			}
			else
			{
				addedNode->next = head;
				head = addedNode;
			}

			size++;
		}

		void PushBack(ListNode* addedNode)
		{
			if (addedNode == nullptr)
			{
				return;
			}

			if (size == 0)
			{
				head = addedNode;
				tail = addedNode;
			}
			else
			{
				tail->next = addedNode;
				tail = addedNode;
			}

			size++;
		}
	};


	static void TestList()
	{
		MyList myLi;

		for (int i = 0; i < 5; i++)
		{
			ListNode* a = new ListNode(i, nullptr);
			myLi.PushFront(a);
		}

		for (int i = 0; i < myLi.size; i++)
		{
			auto listItem = myLi.MoveToNthNode(i);

			cout << listItem->data << endl;
		}

		auto temp = myLi.MoveToNthNode(myLi.size - 1);
		auto listItem = new ListNode(111, nullptr);
		myLi.insertAfter(temp, listItem);

		for (int i = 0; i < myLi.size; i++)
		{
			auto listItem = myLi.MoveToNthNode(i);

			cout << listItem->data << endl;
		}

		myLi.Delete();
	}
}

int main()
{
	cout << ProgrammersBasicTraining::_16::solution(5, 2, 6, 2) << endl;;
	return 0;
}


