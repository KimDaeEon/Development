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
#include <sstream>
#include <regex>
#include <bitset>
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
	cout << ProgrammersBasicTraining::_16::solution(5, 2, 6, 2) << endl;;
	return 0;
}


