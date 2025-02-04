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
#include <regex>
#include <bitset>
#include <math.h>
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

	// 배열 만들기 5
	namespace _20
	{
		// 문자열들을 s idx부터 시작해서 l까지 잘라내고, 그것이 k보다 큰 값들을 담아서 리턴
		vector<int> solution(vector<string> intStrs, int k, int s, int l)
		{
			vector<int> answer;

			for (const auto& str : intStrs)
			{
				string temp = str.substr(s, l);
				int intTemp = stoi(temp);

				if (intTemp > k)
				{
					answer.push_back(intTemp);
				}
			}

			return answer;
		}
	}

	// 부분 문자열 이어 붙여 문자열 만들기
	namespace _21
	{
		string solution(vector<string> my_strings, vector<vector<int>> parts)
		{
			string answer = "";

			for (int i = 0; i < my_strings.size(); i++)
			{
				answer += my_strings[i].substr(parts[i][0], parts[i][1] - parts[i][0] + 1);
			}

			return answer;
		}
	}

	// 문자열 뒤의 n글자
	namespace _22
	{
		string solution(string my_string, int n)
		{
			string answer = "";

			answer = my_string.substr(my_string.size() - n, n);

			return answer;
		}
	}

	// 접미사 배열
	namespace _23
	{
		vector<string> solution(string my_string)
		{
			vector<string> answer;
			int len = my_string.size();

			for (int i = 0; i < len; i++)
			{
				answer.push_back(my_string.substr(len - i - 1));
			}

			sort(answer.begin(), answer.end());
			return answer;
		}
	}

	// 문자열의 앞의 n글자
	namespace _24
	{
		string solution(string my_string, int n)
		{
			string answer = "";
			answer = my_string.substr(0, n);
			return answer;
		}
	}

	// 접두사인지 확인하기
	namespace _25
	{
		int solution(string my_string, string is_prefix)
		{
			int answer = 0;
			/*for (int i = 0; i < my_string.size(); i++)
			{
				if (is_prefix == my_string.substr(0, i + 1))
				{
					answer = 1;
				}
			}*/

			// 아래 방식처럼 prefix 크기를 안다면 이걸로 바로 비교해보는 게 제일 빠르다.
			if (my_string.substr(0, is_prefix.size()) == is_prefix)
			{
				answer = 1;
			}

			return answer;
		}
	}

	// 문자열 뒤집기
	namespace _26
	{
		string solution(string my_string, int s, int e)
		{
			/*while (s <= e)
			{
				swap(my_string[s], my_string[e]);
				s++;
				e--;
			}*/

			// 뒤집는 문제는 아래처럼 하면 더 편하게 할 수 있음. 주의해야할 것은 맨 끝이 도달할 수없는 좌표여야 한다는 것 참고
			reverse(my_string.begin() + s, my_string.begin() + e + 1);

			return my_string;
		}
	}

	// 세로 읽기
	namespace _27
	{
		string solution(string my_string, int m, int c)
		{
			string answer = "";
			/*vector<string> temp;

			for (int i = 0; i < my_string.size(); i += m)
			{
				temp.push_back(my_string.substr(i, m));
			}

			for (const auto& str : temp)
			{
				answer += str[c - 1];
			}*/

			for (int i = c - 1; i < my_string.size(); i += m)
			{
				answer += my_string[i];
			}

			return answer;
		}
	}

	// 제출 내역
	namespace _28
	{
		string solution(int q, int r, string code)
		{
			string answer = "";
			/*for (int i = 0; i < code.size(); i++)
			{
				if (i % q == r)
				{
					answer += code[i];
				}
			}*/

			for (int i = r; i < code.size(); i += q)
			{
				answer += code[i];
			}

			return answer;
		}
	}

	namespace _29
	{
		vector<int> solution(string my_string)
		{
			vector<int> answer(52, 0);

			char upperCaseStart = 'A';
			char lowerCaseStart = 'a';

			for (const auto ch : my_string)
			{
				if (isupper(ch))
				{
					answer[ch - upperCaseStart]++;
				}
				else
				{
					answer[ch - lowerCaseStart + 26]++;
				}
			}

			return answer;
		}
	}

	namespace _30
	{
		// 1이상 n이하 정수 중 k 배수, 오름 차순으로 정렬
		vector<int> solution(int n, int k)
		{
			vector<int> answer;

			for (int i = 1; i <= n; i++)
			{
				if (i % k == 0)
				{
					answer.push_back(i);
				}
			}

			return answer;
		}
	}

	// 글자 지우기
	namespace _31
	{
		string solution(string my_string, vector<int> indices)
		{
			string answer = "";

			/*for (int i = 0; i < my_string.size(); i++)
			{
				if (find(indices.begin(), indices.end(), i) != indices.end())
				{
					continue;
				}
				answer.push_back(my_string[i]);
			}*/

			// 이렇게 해주면 복잡도가 n^2에서 2n으로 줄어든다. 한 번만 체크하면 되는 경우에 이런 식으로 하는 것이 더 좋은 것 같다.
			for (const auto v : indices)
			{
				my_string[v] = ' ';
			}

			for (const auto c : my_string)
			{
				if (c != ' ')
				{
					answer += c;
				}
			}

			return answer;
		}
	}

	// 카운트 다운
	namespace _32
	{
		vector<int> solution(int start_num, int end_num)
		{
			vector<int> answer;
			answer.reserve(start_num - end_num);

			for (int i = start_num; i >= end_num; i--)
			{
				answer.push_back(i);
			}

			return answer;
		}
	}

	// 가까운 1 찾기
	// 이거 문제 이상하다. 지문이 잘못되었다는 얘기가 많음.
	namespace _33
	{
		int solution(vector<int> arr, int idx)
		{
			int answer = -1;

			for (int i = 0; i < arr.size(); i++)
			{
				if (i >= idx && arr[i] == 1)
				{
					answer = i;
					break;
				}
			}

			return answer;
		}
	}

	// 리스트 자르기
	namespace _34
	{
		// n == 1, num_list 0번 ~ b번 인덱스까지 슬라이싱
		// n == 2, a번 ~ 마지막 인덱스까지
		// n == 3, a번 b번 인덱스까지
		// n == 4, a 번 ~ b번 인덱스까지 c 간격으로
		vector<int> solution(int n, vector<int> slicer, vector<int> num_list)
		{
			vector<int> answer;

			int a = slicer[0];
			int b = slicer[1];
			int c = slicer[2];

			if (n == 1)
			{
				for (int i = 0; i <= b; i++)
				{
					answer.push_back(num_list[i]);
				}
			}
			else if (n == 2)
			{
				for (int i = a; i < num_list.size(); i++)
				{
					answer.push_back(num_list[i]);
				}
			}
			else if (n == 3)
			{
				for (int i = a; i <= b; i++)
				{
					answer.push_back(num_list[i]);
				}
			}
			else if (n == 4)
			{
				for (int i = a; i <= b; i += c)
				{
					answer.push_back(num_list[i]);
				}
			}

			return answer;
		}
	}

	// 첫 번째로 나오는 음수
	namespace _35
	{
		int solution(vector<int> num_list)
		{
			for (int i = 0; i < num_list.size(); i++)
			{
				if (num_list[i] < 0)
				{
					return i;
				}
			}

			return -1;
		}
	}

	// 배열 만들기3
	namespace _36
	{
		vector<int> solution(vector<int> arr, vector<vector<int>> intervals)
		{
			vector<int> answer;

			int a1 = intervals[0][0];
			int b1 = intervals[0][1];
			int a2 = intervals[1][0];
			int b2 = intervals[1][1];

			for (int i = a1; i <= b1; i++)
			{
				answer.push_back(arr[i]);
			}

			for (int i = a2; i <= b2; i++)
			{
				answer.push_back(arr[i]);
			}

			return answer;
		}
	}

	// 2의 영역
	namespace _37
	{
		vector<int> solution(vector<int> arr)
		{
			int startIdx = -1;
			int endIdx = -1;

			for (int i = 0; i < arr.size(); i++)
			{
				if (arr[i] == 2 && startIdx == -1)
				{
					startIdx = i;
					continue;
				}

				if (arr[i] == 2 && startIdx != -1)
				{
					endIdx = i;
				}
			}

			if (startIdx != -1 && endIdx == -1)
			{
				endIdx = startIdx;
			}

			if (startIdx != -1 && endIdx != -1)
			{
				return vector<int>(arr.begin() + startIdx, arr.begin() + endIdx + 1);
			}
			else
			{
				return vector<int>(1, -1);
			}
		}
	}

	// 배열 조각하기
	namespace _38
	{
		vector<int> solution(vector<int> arr, vector<int> query)
		{
			//vector<int> answer = arr;
			//for (int i = 0; i < query.size(); i++)
			//{
			//	// query[i] 인덱스 뒷 부분 잘라서 버림
			//	if (i % 2 == 0)
			//	{
			//		answer = vector<int>(answer.begin(), answer.begin() + query[i] + 1);
			//	}
			//	// query[i] 인덱스 앞 부분 잘라서 버림
			//	else
			//	{
			//		answer = vector<int>(answer.begin() + query[i], answer.end());
			//	}
			//}

			// 굳이 제거 연산을 안하고 idx만 추려내도 된다.


			int startIdx = 0;
			int lastIdx = arr.size() - 1;

			for (int i = 0; i < query.size(); i++)
			{
				// query[i] 인덱스 뒷 부분 잘라서 버림
				if (i % 2 == 0)
				{
					int deletedCount = lastIdx - startIdx - query[i];
					lastIdx = lastIdx - deletedCount;
				}
				// query[i] 인덱스 앞 부분 잘라서 버림
				else
				{
					int deletedCount = query[i];
					startIdx = startIdx + deletedCount;
				}
			}

			return vector<int>(arr.begin() + startIdx, arr.begin() + lastIdx + 1);
		}
	}

	// n번째 원소부터
	namespace _39
	{
		vector<int> solution(vector<int> num_list, int n)
		{
			int startIdx = n - 1;
			return vector<int>(num_list.begin() + startIdx, num_list.end());
		}
	}

	// 순서 바꾸기
	namespace _40
	{
		vector<int> solution(vector<int> num_list, int n)
		{
			vector<int> answer = vector<int>(num_list.begin() + n, num_list.end());

			for (int i = 0; i < n; i++)
			{
				answer.push_back(num_list[i]);
			}

			return answer;
		}
	}

	// 왼쪽 오른쪽
	namespace _41
	{
		// l 먼저 나오면 l 나온 기준으로 왼쪽에 있는 문자열 순서대로 담은 리스트, r 먼저 나오면 r 기준으로 오른쪽에 있는 문자열들을 순서대로 담은 리스트 리턴
		vector<string> solution(vector<string> str_list)
		{
			vector<string> answer;
			int startOrEndIdx = -1;
			bool isLeftFirst;

			for (int i = 0; i < str_list.size(); i++)
			{
				if (str_list[i] == "l")
				{
					startOrEndIdx = i; // 왼쪽부터
					isLeftFirst = true;
					break;
				}
				else if (str_list[i] == "r")
				{
					startOrEndIdx = i; // 오른쪽부터
					isLeftFirst = false;
					break;
				}
			}


			if (startOrEndIdx == -1)
			{
				return answer;
			}

			if (isLeftFirst)
			{
				for (int i = 0; i < startOrEndIdx; i++)
				{
					answer.push_back(str_list[i]);
				}
			}
			else
			{
				for (int i = startOrEndIdx + 1; i < str_list.size(); i++)
				{
					answer.push_back(str_list[i]);
				}
			}

			return answer;
		}
	}

	// n 번째 원소까지
	namespace _42
	{
		vector<int> solution(vector<int> num_list, int n)
		{
			return vector<int>(num_list.begin(), num_list.begin() + n);
		}
	}

	// n개 간격의 원소들
	namespace _43
	{
		vector<int> solution(vector<int> num_list, int n)
		{
			vector<int> answer;

			for (int i = 0; i < num_list.size(); i += n)
			{
				answer.push_back(num_list[i]);
			}

			return answer;
		}
	}

	// 홀수 vs 짝수
	namespace _44
	{
		int solution(vector<int> num_list)
		{
			int sumOdd = 0;
			int sumEven = 0;

			for (int i = 0; i < num_list.size(); i++)
			{
				if (i % 2 == 0)
				{
					sumEven += num_list[i];
				}
				else
				{
					sumOdd += num_list[i];
				}
			}

			return sumOdd > sumEven ? sumOdd : sumEven;
		}
	}

	// 5명 씩
	namespace _45
	{
		vector<string> solution(vector<string> names)
		{
			vector<string> answer;

			for (int i = 0; i < names.size(); i += 5)
			{
				answer.emplace_back(names[i]);
			}

			return answer;
		}
	}

	// 할 일 목록
	namespace _46
	{
		vector<string> solution(vector<string> todo_list, vector<bool> finished)
		{
			vector<string> answer;
			for (int i = 0; i < todo_list.size(); i++)
			{
				if (finished[i] == false)
				{
					answer.emplace_back(todo_list[i]);
				}
			}
			return answer;
		}
	}

	// n보다 커질 때까지 더하기
	namespace _47
	{
		int solution(vector<int> numbers, int n)
		{
			int answer = 0;
			for (int i : numbers)
			{
				answer += i;
				if (answer > n)
				{
					return answer;
				}
			}

			return answer;
		}
	}

	// 수열과 구간 쿼리 1
	namespace _48
	{
		vector<int> solution(vector<int> arr, vector<vector<int>> queries)
		{
			for (const auto& query : queries)
			{
				for (int i = query[0]; i <= query[1]; i++)
				{
					arr[i]++;
				}
			}

			return arr;
		}
	}

	// 조건에 맞게 수열 변환하기 1
	namespace _49
	{
		// 50보다 크거나 같은 짝수라면 2로 나누기
		// 50보다 작은 홀수라면 2를 곱하기
		vector<int> solution(vector<int> arr)
		{
			vector<int> answer;
			for (int i : arr)
			{
				if (i >= 50 && i % 2 == 0)
				{
					answer.push_back(i / 2);
				}
				else if (i < 50 && i % 2 != 0)
				{
					answer.push_back(i * 2);
				}
				else
				{
					answer.push_back(i);
				}
			}

			return answer;
		}
	}

	// 조건에 맞게 수열 변환하기 2
	namespace _50
	{
		// 50보다 크거나 같은 짝수라면 2로 나누기
		// 50보다 작은 홀수라면 2를 곱하고 다시 1을 더하기

		int solution(vector<int> arr)
		{
			int answer = 0;
			/*while (true)
			{
				answer++;
				vector<int> before = arr;

				for (auto& i : arr)
				{
					if (i >= 50 && i % 2 == 0)
					{
						i = i / 2;
					}
					else if (i < 50 && i % 2 != 0)
					{
						i = 2 * i + 1;
					}
				}

				bool isEqual = true;

				for (int i = 0; i < arr.size(); i++)
				{
					if (arr[i] != before[i])
					{
						isEqual = false;
					}
				}

				if (isEqual)
				{
					return --answer;
				}
			}*/

			// 아래 식이 O(n)이 나와서 더 빠르다.
			while (true)
			{
				answer++;
				vector<int> before = arr;
				bool isChanged = false;

				for (auto& i : arr)
				{
					if (i >= 50 && i % 2 == 0)
					{
						i = i / 2;
						isChanged = true;
					}
					else if (i < 50 && i % 2 != 0)
					{
						i = 2 * i + 1;
						isChanged = true;
					}
				}

				if (isChanged == false)
				{
					return --answer;
				}
			}
		}
	}

	// 1로 만들기
	namespace _51
	{
		// 짝수 반 나누기
		// 홀수 1 뺀뒤 반 나누기
		int solution(vector<int> num_list)
		{
			int answer = 0;

			for (auto& i : num_list)
			{
				while (i != 1)
				{
					if (i % 2 == 0)
					{
						i = i / 2;
					}
					else
					{
						i = (i - 1) / 2;
					}

					answer++;
				}
			}

			return answer;
		}
	}

	// 길이에 따른 연산
	namespace _52
	{
		// 리스트 길이 11 이상 모든 원소 합 리턴
		// 리스트 길이 10 이하 모든 원소 곱 리턴
		int solution(vector<int> num_list)
		{
			int answer = 0;
			if (num_list.size() >= 11)
			{
				for (const auto i : num_list)
				{
					answer += i;
				}

				return answer;
			}
			else
			{
				answer = 1;
				for (const auto i : num_list)
				{
					answer *= i;
				}

				return answer;
			}
		}
	}

	// 원하는 문자열 찾기
	namespace _53
	{
		int solution(string myString, string pat)
		{
			transform(myString.begin(), myString.end(), myString.begin(), ::tolower);
			transform(pat.begin(), pat.end(), pat.begin(), ::tolower);

			if (myString.find(pat) != string::npos)
			{
				return 1;
			}

			return 0;
		}
	}

	// 대문자로 바꾸기
	namespace _54
	{
		string solution(string myString)
		{
			transform(myString.begin(), myString.end(), myString.begin(), ::toupper);
			return myString;
		}
	}

	// 소문자로 바꾸기
	namespace _55
	{
		string solution(string myString)
		{
			transform(myString.begin(), myString.end(), myString.begin(), ::tolower);
			return myString;
		}
	}

	// 배열에서 문자열 대소문자 변환하기
	namespace _56
	{
		// 홀수번째 인덱스 문자열 모두 대문자로
		// 짝수번째 인덱스 문자열 모두 소문자로
		vector<string> solution(vector<string> strArr)
		{
			bool tolowerMode = true;

			for (auto& str : strArr)
			{
				if (tolowerMode)
				{
					transform(str.begin(), str.end(), str.begin(), ::tolower);
				}
				else
				{
					transform(str.begin(), str.end(), str.begin(), ::toupper);
				}

				tolowerMode = !tolowerMode;
			}

			return strArr;
		}
	}

	// A 강조하기
	namespace _57
	{
		// 'a' 면 모두 대문자 'A'로
		// 'A'가 아닌 모든 대문자 알파벳은 소문자로
		string solution(string myString)
		{
			for (auto& ch : myString)
			{
				if (ch == 'a' || ch == 'A')
				{
					ch = 'A';
				}
				else
				{
					if (isupper(ch))
					{
						ch = ::tolower(ch);
					}
				}
			}

			return myString;
		}
	}

	// 특정한 문자를 대문자로 바꾸기
	namespace _58
	{
		string solution(string my_string, string alp)
		{
			char ch = alp[0];

			for (auto& c : my_string)
			{
				if (c == ch)
				{
					c = ::toupper(c);
				}
			}

			return my_string;
		}
	}

	// 특정 문자열로 끝나는 가장 긴 부분 문자열 찾기
	namespace _59
	{
		string solution(string myString, string pat)
		{
			string answer = "";

			// rfind로도 찾을 수 있단 것을 알자.
			int idx = myString.rfind(pat) + pat.size();
			if (idx != string::npos)
			{
				answer = myString.substr(0, idx);
			}

			return answer;
		}
	}

	// 문자열이 몇 번 등장하는지 세기
	namespace _60
	{
		int solution(string myString, string pat)
		{
			int answer = 0;
			int findPos = 0;

			while (true)
			{
				findPos = myString.find(pat, findPos);

				if (findPos != string::npos)
				{
					answer++;
					findPos++;
				}
				else
				{
					break;
				}
			}


			//for (int i = 0; i < myString.size(); i++)
			//{
			//	int idx = myString.rfind(pat, i);

			//	if (idx != string::npos)
			//	{
			//		answer++;
			//		myString[idx] = ' ';
			//	}
			//}

			return answer;
		}
	}

	// ad 제거하기
	namespace _61
	{
		vector<string> solution(vector<string> strArr)
		{
			vector<string> answer;

			for (auto& str : strArr)
			{
				if (str.find("ad") != string::npos)
				{
					continue;
				}

				answer.push_back(str);
			}

			return answer;
		}
	}

	// 공백으로 구분하기 1
	namespace _62
	{
		vector<string> solution(string my_string)
		{
			vector<string> answer;
			int startIdx = 0;

			for (int i = 0; i < my_string.size(); i++)
			{
				if (my_string[i] == ' ')
				{
					answer.push_back(my_string.substr(startIdx, i - startIdx));
					startIdx = i + 1;
				}
			}

			answer.push_back(my_string.substr(startIdx));

			return answer;
		}
	}

	// 공백으로 구분하기 2
	namespace _63
	{
		vector<string> solution(string my_string)
		{
			vector<string> answer;
			string temp = "";

			for (int i = 0; i < my_string.size(); i++)
			{
				if (my_string[i] == ' ')
				{
					if (temp.empty() == false)
					{
						answer.push_back(temp);
						temp.clear();
					}
				}
				else
				{
					temp.push_back(my_string[i]);
				}
			}

			if (temp.empty() == false)
			{
				answer.push_back(temp);
				temp.clear();
			}

			return answer;
		}
	}

	// x 사이의 개수
	namespace _64
	{
		vector<int> solution(string myString)
		{
			vector<int> answer;
			int temp = 0;

			for (int i = 0; i < myString.size(); i++)
			{
				if (myString[i] == 'x')
				{
					answer.push_back(temp);
					temp = 0;
				}
				else
				{
					temp++;
				}
			}

			answer.push_back(temp);

			return answer;
		}
	}

	// 문자열 잘라서 정렬하기
	namespace _65
	{
		vector<string> solution(string myString)
		{
			vector<string> answer;
			string temp = "";

			for (int i = 0; i < myString.size(); i++)
			{
				if (myString[i] == 'x')
				{
					if (temp.size() > 0)
					{
						answer.push_back(temp);
						temp.clear();
					}
				}
				else
				{
					temp.push_back(myString[i]);
				}
			}

			if (temp.size() > 0)
			{
				answer.push_back(temp);
			}

			sort(answer.begin(), answer.end());

			return answer;
		}
	}

	// 간단한 식 계산하기
	namespace _66
	{
		bool IsOp(char ch)
		{
			return ch == '+' || ch == '-' || ch == '*';
		}

		int CalcByOp(char op, int a, int b)
		{
			if (op == '+')
			{
				return a + b;
			}
			else if (op == '-')
			{
				return a - b;
			}
			else if (op == '*')
			{
				return a * b;
			}
		}

		int solution(string binomial)
		{
			//int answer = 0;
			//int a = -1;
			//int b = -1;
			//string temp = "";
			//char op = ' ';

			//for (int i = 0; i < binomial.size(); i++)
			//{
			//	if (!IsOp(binomial[i]) && binomial[i] != ' ')
			//	{
			//		temp.push_back(binomial[i]);
			//	}
			//	else
			//	{
			//		if (IsOp(binomial[i]))
			//		{
			//			op = binomial[i];
			//			i++;
			//			continue;
			//		}

			//		if (a == -1)
			//		{
			//			a = stoi(temp);
			//			temp.clear();
			//			continue;
			//		}
			//	}
			//}

			//b = stoi(temp);

			//return CalcByOp(op, a, b);

			stringstream ss(binomial);

			string a;
			string op;
			string b;

			ss >> a >> op >> b;

			return CalcByOp(op[0], stoi(a), stoi(b));
			
		}
	}

	// 문자열 바꿔서 찾기
	namespace _67
	{
		int solution(string myString, string pat)
		{
			for (auto& ch : myString)
			{
				if (ch == 'A')
				{
					ch = 'B';
				}
				else if (ch == 'B')
				{
					ch = 'A';
				}
			}

			if (myString.find(pat) != string::npos)
			{
				return 1;
			}

			return 0;
		}
	}

	// my_string
	namespace _68
	{
		string solution(string rny_string)
		{
			string answer = "";
			for (const auto ch : rny_string)
			{
				if (ch == 'm')
				{
					answer += "rn";
				}
				else
				{
					answer += ch;
				}
			}

			return answer;
		}
	}

	// 세 개의 구분자
	namespace _69
	{
		vector<string> solution(string myStr)
		{
			vector<string> answer;
			string temp;

			for (int i = 0; i < myStr.size(); i++)
			{
				char ch = myStr[i];
				if (ch == 'a' || ch == 'b' || ch == 'c')
				{
					if (temp.size() > 0)
					{
						answer.push_back(temp);
						temp.clear();
					}
				}
				else
				{
					temp.push_back(ch);
				}
			}

			if (temp.size() > 0)
			{
				answer.push_back(temp);
				temp.clear();
			}

			if (answer.size() == 0)
			{
				answer.push_back("EMPTY");
			}

			return answer;
		}
	}

	// 배열의 원소만큼 추가하기
	namespace _70
	{
		vector<int> solution(vector<int> arr)
		{
			vector<int> answer;
			for (int i : arr)
			{
				for (int j = 0; j < i; j++)
				{
					answer.push_back(i);
				}
			}
			return answer;
		}
	}

	// 빈 배열에 추가, 삭제하기
	namespace _71
	{
		vector<int> solution(vector<int> arr, vector<bool> flag)
		{
			vector<int> answer;
			for (int i = 0; i < flag.size(); i++)
			{
				if (flag[i] == true)
				{
					for (int j = 0; j < arr[i] * 2; j++)
					{
						answer.push_back(arr[i]);
					}
				}
				else
				{
					for (int j = 0; j < arr[i]; j++)
					{
						answer.pop_back();
					}
				}
			}
			return answer;
		}
	}

	// 배열 만들기 6
	namespace _72
	{
		vector<int> solution(vector<int> arr)
		{
			vector<int> answer;
			for (int i = 0; i < arr.size(); i++)
			{
				if (answer.empty())
				{
					answer.push_back(arr[i]);
					continue;
				}

				if (answer.back() == arr[i])
				{
					answer.pop_back();
				}
				else
				{
					answer.push_back(arr[i]);
				}
				
			}

			if (answer.empty())
			{
				answer.push_back(-1);
			}

			return answer;
		}
	}

	namespace _73
	{
		vector<int> solution(vector<int> arr, int k)
		{
			vector<int> answer;
			return answer;
		}
	}

	// 무작위로 K개의 수 뽑기
	namespace _74
	{
		vector<int> solution(vector<int> arr, int k)
		{
			vector<int> answer;
			unordered_set<int> s;

			for (int i = 0; i < arr.size(); i++)
			{
				if(s.find(arr[i]) == s.end())
				{
					answer.push_back(arr[i]);
					s.insert(arr[i]);
				}

				if (answer.size() == k)
				{
					break;
				}
			}

			answer.resize(k, -1); // 요렇게 나머지 채우는 것도 깔끔하게 가능함. 알아두기
			//while (answer.size() < k)
			//{
			//	answer.push_back(-1);
			//}

			return answer;
		}
	}

	// 배열의 길이를 2의 거듭제곱으로 만들기
	namespace _75
	{
		bool CheckPowerOf2(int a)
		{
			while (a > 1)
			{
				int quotient = a / 2;
				int remainder = a % 2;
				a = quotient;

				if (remainder != 0)
				{
					return false;
				}
			}

			return true;
		}
		vector<int> solution(vector<int> arr)
		{
			//int size = arr.size();

			//while (true)
			//{
			//	if (CheckPowerOf2(size))
			//	{
			//		break;
			//	}
			//	else
			//	{
			//		size++;
			//	}
			//}

			//arr.resize(size, 0);


			// 아래처럼 math 관련 함수 쓰면 좀 더 편하게 할 수 있다.
			int size = arr.size();

			int target = pow(2, ceil(log2(size)));

			arr.resize(target, 0);

			return arr;
		}
	}

	// 배열 비교하기
	namespace _76
	{
		// 길이가 더 큰 배열이 큰 배열
		// 길이가 같다면, 모든 원소의 합이 큰 쪽이 크고, 합도 같다면 같은 배열
		// arr2가 크다면 -1, arr1이 크다면 1, 같다면 0 리턴
		int solution(vector<int> arr1, vector<int> arr2)
		{
			int answer = 0;
			int size1 = arr1.size();
			int size2 = arr2.size();

			if (size1 != size2)
			{
				return size1 > size2 ? 1 : -1;
			}

			int sum1 = 0;
			int sum2 = 0;

			for (int i = 0; i < size1; i++)
			{
				sum1 += arr1[i];
				sum2 += arr2[i];
			}

			if (sum1 != sum2)
			{
				return sum1 > sum2 ? 1 : -1;
			}

			return 0;
		}
	}

	// 문자열 묶기
	namespace _77
	{
		int solution(vector<string> strArr)
		{
			int answer = 0;
			unordered_map<int, int> um;

			for (const auto& str : strArr)
			{
				um[str.size()]++;
			}

			int m = 0;

			for (const auto& [k, v] : um)
			{
				if (m < v)
				{
					m = v;
				}
			}
			
			return m;
		}
	}

	// 배열의 길이에 따라 다른 연산하기
	namespace _78
	{
		vector<int> solution(vector<int> arr, int n)
		{
			int size = arr.size();

			// 짝수면 홀수 인덱스에 n 더하기
			if (size % 2 == 0)
			{
				for (int i = 1; i < size; i = i + 2)
				{
					arr[i] += n;
				}
			}
			// 홀수면 짝수 인덱스에 n 더하기
			else
			{
				for (int i = 0; i < size; i = i + 2)
				{
					arr[i] += n;
				}
			}
			return arr;
		}
	}

	// 뒤에서 5등까지
	namespace _79
	{
		vector<int> solution(vector<int> num_list)
		{
			sort(num_list.begin(), num_list.end());

			vector<int> answer(num_list.begin(), num_list.begin() + 5);

			return answer;
		}
	}

	// 뒤에서 5등 위로
	namespace _80
	{
		vector<int> solution(vector<int> num_list)
		{
			sort(num_list.begin(), num_list.end());
			vector<int> answer(num_list.begin() + 5, num_list.end());

			return answer;
		}
	}

	// 전국 대회 선발 고사
	namespace _81
	{
		// 참가 가능한 사람들 중, 
		// rank 가 가장 낮은 사람 상위 3명의 index1 * 10000 + index2 * 100 + index3 을 리턴
		struct rankInfo
		{
			int idx;
			int rank;
		};

		int solution(vector<int> rank, vector<bool> attendance)
		{
			int answer = 0;
			vector<rankInfo> temp;

			for (int i = 0; i < attendance.size(); i++)
			{
				if (attendance[i])
				{
					temp.push_back({ i, rank[i] });
				}
			}

			sort(temp.begin(), temp.end(), [](rankInfo& a, rankInfo& b)
				{
					return a.rank < b.rank;
				});

			answer = 10000 * temp[0].idx + 100 * temp[1].idx + temp[2].idx;
			return answer;
		}
	}

	// 정수 부분
	namespace _82
	{
		int solution(double flo)
		{
			return flo;
		}
	}

	// 문자열 정수의 합
	namespace _83
	{
		int solution(string num_str)
		{
			int answer = 0;
			
			for (auto ch : num_str)
			{
				answer += ch - '0';
			}

			return answer;
		}
	}
	
	// 문자열을 정수로 변환하기
	namespace _84
	{
		int solution(string n_str)
		{
			return stoi(n_str);
		}
	}

	// 0 떼기
	namespace _85
	{
		// 맨 처음 왼쪽에 있는 0들을 다 뗀다.
		string solution(string n_str)
		{
			string answer = "";
			
			for (int i = 0; i < n_str.size(); i++)
			{
				if (n_str[i] == '0')
				{
					continue;
				}
				else
				{
					answer = n_str.substr(i);
					break;
				}
			}

			return answer;
		}
	}

	// 두 수의 합
	namespace _86
	{
		string solution(string a, string b)
		{

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
	string a = "43   + 12";

	stringstream ss(a);

	string b;
	string op;
	string c;

	ss >> b;
	ss >> op;
	ss >> c;

	cout << b << endl;
	cout << op << endl;
	cout << c << endl;

	return 0;
}



