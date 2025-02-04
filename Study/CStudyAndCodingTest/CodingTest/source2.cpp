#pragma once
#include <cassert> // assert�� ����ϱ� ���� ���
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

// �⺻�� ���� ���� ����
// ���� �׽�Ʈ ���̽��� �Ϻθ� �°� �Ϻθ� Ʋ�� ��찡 �ִٸ� if �� ���� ���� Ʋ������ �ʾҴ��� Ȯ������.

// index ��� ���� ��� ���� �ð� ������ �� �� �ִ�. Ǯ���� ���ð� ť ������ ������ ��������.
// ��� ��Ȯ�� ������ ���ϴ� ���� �ƴ϶�, ����, ���̸� ���ϴ� ���̸� �ξ� ����� �ܼ�ȭ�� �� �ִ�.

// unordered_map �� key value �� ���� ������� auto [k,v] �� �ϸ� key �� ������, map �� key �� ���ĵǾ ������. ���� ������ ����� ����ص���.

// unsigned int �� int �� < �񱳷� ���� �ȵȴ�. ���� ��� -1 < (unsigned int)0 �� �ϸ�, �� ������ unsigned int �� ������ ���� -1�� ��û���� ū ���� �Ǿ������.
// �񱳸� �� ������ ���� ���¸� �� �����ؼ� �ǽ�����.

// vector<vector<int>> �� �ʱⰪ�� �ַ� �Ѵٸ�, vector<vector<int>> v (5, vector<int>(5, 0)) �̷� ������ �ϸ� �ȴ�.

// priority_queue<int, vector<int>, greater<int>> pq; <- �̷��� �ϸ� ���� �༮���� pq.top() ���� ���´�. �ݴ�� �Ϸ��� less<int> ����.
// �򰥸��� ���� vector �� sort �� ���� ���� �� -> ū ������ �Ϸ��� less<int> �� �־�� �Ѵٴ� ���̴�. 
// sort �� true �� �ȹٲٰ�, heap �� ���� pq �� true �� ��ġ ��ȯ�� �ϱ� ������ �̷��� �ȴ�.

// multiset �� ��� minmax �� ���� ���� �뵵�� Ȱ��� �� �ִ�.
// substr �� ó���� ������ offset ��ġ��, �� ������ count �̴�. a.substr(1,3) �̷��� 1��° index �� �����ؼ� 3���� ���ڸ� ������ string �� �����Ѵ�.

// �Լ� ���ο��� �ݺ������� ������ �ʿ� ���� ���������� �ٱ����� ������.

// �ִ� ũ�⳪ �ּ� ũ�⸸ �˸� �ȴٸ� max_element, min_element�� ����. sort �ϴ� �ͺ��� ���귮�� ���� ���� �� �ִ�.

// STL �����̳� ������ ��ȣ ��ȯ�� �����ѵ�, �̷��� ��� �����ڿ��� �ٷ� �����̳� ���� ��ȯ�� �̷�������� ����. ex) unordered_set<int> us(v.begin(), v.end());

// DFS �� ����, BFS �� queue �� ���ؼ� ������ �� �ִ�.

// vector �� () ���� ũ�⸦ �����Ͽ� �����ϸ� �⺻ ���ڵ��� ���� �� 0���� �ʱ�ȭ�ȴ�. vector<pair<int,int>> �̷��� �ϸ� pair �� ���� �� 0���� �ʱ�ȭ�ȴ�.


// �Ʒ��� ������ Ǯ ���� �ݺ������� ����ϰų� ����صθ� ���� �Լ���
namespace Utils
{
	// DFS �� ���� ���ϱ� ���� �ڵ�
	void permute(vector<int>& nums, int startLevel, int targetLevel)
	{
		if (startLevel == targetLevel)
		{
			// ���� ���
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
				swap(nums[startLevel], nums[i]); // ��Ʈ��ŷ
			}
		}
	}

	// �Ʒ��� Stack�� ���� DFS�� ������ ���ϴ� �ڵ�
	struct DFSState
	{
		vector<int> nums;
		int level; // ���� ����
	};
	void IterativePermute(vector<int> nums)
	{
		stack<DFSState> s;
		s.push({ nums, 0 }); // �ʱ� ���¸� ���ÿ� Ǫ��

		while (!s.empty())
		{
			DFSState current = s.top();
			s.pop();

			int level = current.level;
			vector<int> currentNums = current.nums;

			if (level == currentNums.size() - 1)
			{
				// ������ �ϼ��� ����: ���
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
					swap(currentNums[level], currentNums[i]);  // ��� ��ȯ
					s.push({ currentNums, level + 1 });          // ���� ���¸� ���ÿ� Ǫ��
					// �ٽ� ��ȯ�ؼ� ���� ���·� ���ư� (���� ������ ���� �غ�)
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
			sort(dungeons.begin(), dungeons.end()); // �̰� �����ָ� next_permutation ���� ��ȸ�� �ȵȴ�.
			// ���� ���� permutation ������ ��ȸ�ϸ鼭,
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
			// �� �������� �Ҹ��ϴ� �Ƿε� ����ؼ� �ִ� k�� ���Ѵ�.

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
					swap(dungeons[startLevel], dungeons[i]); // ��Ʈ��ŷ
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
	// ���������� Ư���� �׸� ���ϱ�
	namespace _1
	{
		// included[i] �� true�� ��� a[i+1] = a + n*d �� �� ���ؼ� ����
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
		// �� ���ڰ� ��� �ٸ��� a + b + c
		// 2���� ���� �������� �ٸ� ���ڸ� (a + b + c) * (a^2 + b^2 + c^2)
		// �� ���ڰ� ��� ���ٸ� (a + b + c) * (a^2 + b^2 + c^2) * (a^3 + b^3 + c^3)

		// �߰� ����: �̰� �̷��� ���� ���� ã�� ������ Set�� Ȱ���� ���� ����
		int solution(int a, int b, int c)
		{
			int t1 = a + b + c;
			int t2 = t1 * (a * a + b * b + c * c);
			int t3 = t2 * (a * a * a + b * b * b + c * c * c);

			if (a != b && b != c && a != c)
			{
				return t1;
			}
			// �� ���ڰ� ��� ���� ���
			else if (a = b && b == c && c == a)
			{
				return t3;
			}
			// ������ ���
			else
			{
				return t2;
			}
		}


	}

	namespace _3
	{
		// ���丮�󺸴� ��ü �� ^ 2�� ������ 1 ����
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
		// ȣ���� ������� �̾� ���� ���� ¦���� ������� �̾� ���� ���� �� (���ڿ�)
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
		// ������ ���Ұ� �� �� ���Һ��� ũ�� (������ ���� - �� �� ����) �߰��ؼ� ����
		// ������ ���� <= �� �� ���Ҷ�� (������ ���� * 2) �߰��ؼ� ����
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
		// �� �����ϱ� 1
		// control ���ڿ� "w": n = n+1, "s": n = n-1;, "d": n = n+10, "a": n = n-10

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
		// �� �����ϱ� 2
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

	// ������ ���� ���� 3
	namespace _8
	{
		// query�� �ִ� �ε��� ���� ����
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

	// ������ ���� ���� 2
	namespace _9
	{
		// ���� �������� ���� ���� arr[i] ����, ������ -1
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

	// ������ ���� ���� 4
	namespace _10
	{
		// ���� idx 0, 1 ������ ���鿡 ���ؼ�, i�� K�� ����̸� arr[i]�� 1�� ���Ѵ�.
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

	// �迭 �����2
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

		// l �̻� r ���� ���� ��, ���� "0"�� "5"�θ� �̷���� ��� ������ ������������ ������ �迭 return, ���ٸ� -1 ��� ����
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


		// �Ʒ��� �� ���� Ǯ��

		bool CheckDividedBy10AndN(int input, int n)
		{
			int temp = input;

			// 10�� �����鼭 1�� �ڸ� ���� ��� 0 �Ǵ� n���� Ȯ��
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

	// ī��Ʈ ��
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

	// �ݶ��� ���� �����
	namespace _13
	{
		// n�� ¦���� �� 2�� ����
		// n�� Ȧ ���϶� 3*n + 1
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

	// �迭 ����� 4
	namespace _14
	{
		// i = 0
		// i < arr.size()�� �ݺ� �۾� ����
		// stk �� �迭, arr[i]�� stk�� �߰� i=i+1
		// stk ���� �ְ�, stk ������ ���� < arr[i], arr[i]�� stk�� �߰�, i=i+1
		// stk ���� �ְ�, stk ������ ���� >= arr[i], stk ������ ���� stk���� ����
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

	// ������ �� ����
	namespace _15
	{
		bool solution(bool x1, bool x2, bool x3, bool x4)
		{
			return (x1 || x2) && (x3 || x4);
		}
	}

	// �ֻ��� ���� 3
	namespace _16
	{
		// 4 �ֻ��� ���� ��� p�� ���ٸ� 1111*p ���� ����
		// 3 �ֻ������� ���� ���ڰ� P�� ����, ������ �ٸ� �ֻ������� ���� ���ڰ� q��� (10*p+q)^2 ���� ����
		// 2 �ֻ����� 2���� ���� ����, ���� ���� ���� p, q �̸� (p+q) * abs(p-q) ���� ����
		// 2 �ֻ����� p�̰�, ������ �� �ֻ������� ���� ���ڰ� ���� p�� �ƴ� �ٸ� �� q,r �̶�� q*r ���� ����
		// �� �ֻ����� ���� ���ڰ� ��� �ٸ��� ���� ���� ���� ��ŭ�� ������ ����


		// ���� �ٽ� map ���� �����ؼ� Ǯ���
		int solution(int a, int b, int c, int d)
		{
			// �ֻ��� ���� ���� ����� ���� ���
			unordered_map<int, int> um;

			um[a]++;
			um[b]++;
			um[c]++;
			um[d]++;

			int setCnt = um.size();

			// 4 �ֻ����� ���� ��� (size = 1)
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
					// 3 �ֻ����� ����, 1���� �ٸ� ��� (size = 2)
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

					// 2 �ֻ����� ���� ��� (size = 2)
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

			// 2 �ֻ����� ����, ������ 2���� ���� �ٸ� ��� (size = 3)
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

			// ��� �ֻ����� �ٸ� ��� (size = 4)

			int minVal = min(a, b);
			minVal = min(minVal, c);
			minVal = min(minVal, d);

			return minVal;
		}
	}

	// ���� �̾� �ٿ� ���ڿ� �����
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

	// 9�� ���� ������
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

	// ���ڿ� ���� �� ������
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

	// �迭 ����� 5
	namespace _20
	{
		// ���ڿ����� s idx���� �����ؼ� l���� �߶󳻰�, �װ��� k���� ū ������ ��Ƽ� ����
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

	// �κ� ���ڿ� �̾� �ٿ� ���ڿ� �����
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

	// ���ڿ� ���� n����
	namespace _22
	{
		string solution(string my_string, int n)
		{
			string answer = "";

			answer = my_string.substr(my_string.size() - n, n);

			return answer;
		}
	}

	// ���̻� �迭
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

	// ���ڿ��� ���� n����
	namespace _24
	{
		string solution(string my_string, int n)
		{
			string answer = "";
			answer = my_string.substr(0, n);
			return answer;
		}
	}

	// ���λ����� Ȯ���ϱ�
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

			// �Ʒ� ���ó�� prefix ũ�⸦ �ȴٸ� �̰ɷ� �ٷ� ���غ��� �� ���� ������.
			if (my_string.substr(0, is_prefix.size()) == is_prefix)
			{
				answer = 1;
			}

			return answer;
		}
	}

	// ���ڿ� ������
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

			// ������ ������ �Ʒ�ó�� �ϸ� �� ���ϰ� �� �� ����. �����ؾ��� ���� �� ���� ������ ������ ��ǥ���� �Ѵٴ� �� ����
			reverse(my_string.begin() + s, my_string.begin() + e + 1);

			return my_string;
		}
	}

	// ���� �б�
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

	// ���� ����
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
		// 1�̻� n���� ���� �� k ���, ���� �������� ����
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

	// ���� �����
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

			// �̷��� ���ָ� ���⵵�� n^2���� 2n���� �پ���. �� ���� üũ�ϸ� �Ǵ� ��쿡 �̷� ������ �ϴ� ���� �� ���� �� ����.
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

	// ī��Ʈ �ٿ�
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

	// ����� 1 ã��
	// �̰� ���� �̻��ϴ�. ������ �߸��Ǿ��ٴ� ��Ⱑ ����.
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

	// ����Ʈ �ڸ���
	namespace _34
	{
		// n == 1, num_list 0�� ~ b�� �ε������� �����̽�
		// n == 2, a�� ~ ������ �ε�������
		// n == 3, a�� b�� �ε�������
		// n == 4, a �� ~ b�� �ε������� c ��������
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

	// ù ��°�� ������ ����
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

	// �迭 �����3
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

	// 2�� ����
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

	// �迭 �����ϱ�
	namespace _38
	{
		vector<int> solution(vector<int> arr, vector<int> query)
		{
			//vector<int> answer = arr;
			//for (int i = 0; i < query.size(); i++)
			//{
			//	// query[i] �ε��� �� �κ� �߶� ����
			//	if (i % 2 == 0)
			//	{
			//		answer = vector<int>(answer.begin(), answer.begin() + query[i] + 1);
			//	}
			//	// query[i] �ε��� �� �κ� �߶� ����
			//	else
			//	{
			//		answer = vector<int>(answer.begin() + query[i], answer.end());
			//	}
			//}

			// ���� ���� ������ ���ϰ� idx�� �߷����� �ȴ�.


			int startIdx = 0;
			int lastIdx = arr.size() - 1;

			for (int i = 0; i < query.size(); i++)
			{
				// query[i] �ε��� �� �κ� �߶� ����
				if (i % 2 == 0)
				{
					int deletedCount = lastIdx - startIdx - query[i];
					lastIdx = lastIdx - deletedCount;
				}
				// query[i] �ε��� �� �κ� �߶� ����
				else
				{
					int deletedCount = query[i];
					startIdx = startIdx + deletedCount;
				}
			}

			return vector<int>(arr.begin() + startIdx, arr.begin() + lastIdx + 1);
		}
	}

	// n��° ���Һ���
	namespace _39
	{
		vector<int> solution(vector<int> num_list, int n)
		{
			int startIdx = n - 1;
			return vector<int>(num_list.begin() + startIdx, num_list.end());
		}
	}

	// ���� �ٲٱ�
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

	// ���� ������
	namespace _41
	{
		// l ���� ������ l ���� �������� ���ʿ� �ִ� ���ڿ� ������� ���� ����Ʈ, r ���� ������ r �������� �����ʿ� �ִ� ���ڿ����� ������� ���� ����Ʈ ����
		vector<string> solution(vector<string> str_list)
		{
			vector<string> answer;
			int startOrEndIdx = -1;
			bool isLeftFirst;

			for (int i = 0; i < str_list.size(); i++)
			{
				if (str_list[i] == "l")
				{
					startOrEndIdx = i; // ���ʺ���
					isLeftFirst = true;
					break;
				}
				else if (str_list[i] == "r")
				{
					startOrEndIdx = i; // �����ʺ���
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

	// n ��° ���ұ���
	namespace _42
	{
		vector<int> solution(vector<int> num_list, int n)
		{
			return vector<int>(num_list.begin(), num_list.begin() + n);
		}
	}

	// n�� ������ ���ҵ�
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

	// Ȧ�� vs ¦��
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

	// 5�� ��
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

	// �� �� ���
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

	// n���� Ŀ�� ������ ���ϱ�
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

	// ������ ���� ���� 1
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

	// ���ǿ� �°� ���� ��ȯ�ϱ� 1
	namespace _49
	{
		// 50���� ũ�ų� ���� ¦����� 2�� ������
		// 50���� ���� Ȧ����� 2�� ���ϱ�
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

	// ���ǿ� �°� ���� ��ȯ�ϱ� 2
	namespace _50
	{
		// 50���� ũ�ų� ���� ¦����� 2�� ������
		// 50���� ���� Ȧ����� 2�� ���ϰ� �ٽ� 1�� ���ϱ�

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

			// �Ʒ� ���� O(n)�� ���ͼ� �� ������.
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

	// 1�� �����
	namespace _51
	{
		// ¦�� �� ������
		// Ȧ�� 1 ���� �� ������
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

	// ���̿� ���� ����
	namespace _52
	{
		// ����Ʈ ���� 11 �̻� ��� ���� �� ����
		// ����Ʈ ���� 10 ���� ��� ���� �� ����
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

	// ���ϴ� ���ڿ� ã��
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

	// �빮�ڷ� �ٲٱ�
	namespace _54
	{
		string solution(string myString)
		{
			transform(myString.begin(), myString.end(), myString.begin(), ::toupper);
			return myString;
		}
	}

	// �ҹ��ڷ� �ٲٱ�
	namespace _55
	{
		string solution(string myString)
		{
			transform(myString.begin(), myString.end(), myString.begin(), ::tolower);
			return myString;
		}
	}

	// �迭���� ���ڿ� ��ҹ��� ��ȯ�ϱ�
	namespace _56
	{
		// Ȧ����° �ε��� ���ڿ� ��� �빮�ڷ�
		// ¦����° �ε��� ���ڿ� ��� �ҹ��ڷ�
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

	// A �����ϱ�
	namespace _57
	{
		// 'a' �� ��� �빮�� 'A'��
		// 'A'�� �ƴ� ��� �빮�� ���ĺ��� �ҹ��ڷ�
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

	// Ư���� ���ڸ� �빮�ڷ� �ٲٱ�
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

	// Ư�� ���ڿ��� ������ ���� �� �κ� ���ڿ� ã��
	namespace _59
	{
		string solution(string myString, string pat)
		{
			string answer = "";

			// rfind�ε� ã�� �� �ִ� ���� ����.
			int idx = myString.rfind(pat) + pat.size();
			if (idx != string::npos)
			{
				answer = myString.substr(0, idx);
			}

			return answer;
		}
	}

	// ���ڿ��� �� �� �����ϴ��� ����
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

	// ad �����ϱ�
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

	// �������� �����ϱ� 1
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

	// �������� �����ϱ� 2
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

	// x ������ ����
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

	// ���ڿ� �߶� �����ϱ�
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

	// ������ �� ����ϱ�
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

	// ���ڿ� �ٲ㼭 ã��
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

	// �� ���� ������
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

	// �迭�� ���Ҹ�ŭ �߰��ϱ�
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

	// �� �迭�� �߰�, �����ϱ�
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

	// �迭 ����� 6
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

	// �������� K���� �� �̱�
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

			answer.resize(k, -1); // �䷸�� ������ ä��� �͵� ����ϰ� ������. �˾Ƶα�
			//while (answer.size() < k)
			//{
			//	answer.push_back(-1);
			//}

			return answer;
		}
	}

	// �迭�� ���̸� 2�� �ŵ��������� �����
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


			// �Ʒ�ó�� math ���� �Լ� ���� �� �� ���ϰ� �� �� �ִ�.
			int size = arr.size();

			int target = pow(2, ceil(log2(size)));

			arr.resize(target, 0);

			return arr;
		}
	}

	// �迭 ���ϱ�
	namespace _76
	{
		// ���̰� �� ū �迭�� ū �迭
		// ���̰� ���ٸ�, ��� ������ ���� ū ���� ũ��, �յ� ���ٸ� ���� �迭
		// arr2�� ũ�ٸ� -1, arr1�� ũ�ٸ� 1, ���ٸ� 0 ����
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

	// ���ڿ� ����
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

	// �迭�� ���̿� ���� �ٸ� �����ϱ�
	namespace _78
	{
		vector<int> solution(vector<int> arr, int n)
		{
			int size = arr.size();

			// ¦���� Ȧ�� �ε����� n ���ϱ�
			if (size % 2 == 0)
			{
				for (int i = 1; i < size; i = i + 2)
				{
					arr[i] += n;
				}
			}
			// Ȧ���� ¦�� �ε����� n ���ϱ�
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

	// �ڿ��� 5�����
	namespace _79
	{
		vector<int> solution(vector<int> num_list)
		{
			sort(num_list.begin(), num_list.end());

			vector<int> answer(num_list.begin(), num_list.begin() + 5);

			return answer;
		}
	}

	// �ڿ��� 5�� ����
	namespace _80
	{
		vector<int> solution(vector<int> num_list)
		{
			sort(num_list.begin(), num_list.end());
			vector<int> answer(num_list.begin() + 5, num_list.end());

			return answer;
		}
	}

	// ���� ��ȸ ���� ���
	namespace _81
	{
		// ���� ������ ����� ��, 
		// rank �� ���� ���� ��� ���� 3���� index1 * 10000 + index2 * 100 + index3 �� ����
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

	// ���� �κ�
	namespace _82
	{
		int solution(double flo)
		{
			return flo;
		}
	}

	// ���ڿ� ������ ��
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
	
	// ���ڿ��� ������ ��ȯ�ϱ�
	namespace _84
	{
		int solution(string n_str)
		{
			return stoi(n_str);
		}
	}

	// 0 ����
	namespace _85
	{
		// �� ó�� ���ʿ� �ִ� 0���� �� ����.
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

	// �� ���� ��
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
				// ��ȿ���� ���� �ε���
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
					// n ��° �ִ� ��� ã�� ����
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



