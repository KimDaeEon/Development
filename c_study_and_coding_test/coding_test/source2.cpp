#pragma once
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
// ���� �׽�Ʈ ���̽��� �Ϻθ� �°� �Ϻθ� Ʋ�� ��찡 �ִٸ� if �� ���� ������ Ʋ������ �ʾҴ��� Ȯ������.

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

int main()
{
	vector<int> nums = { 1, 2, 3 };
	Utils::IterativePermute(nums);
	return 0;
}
