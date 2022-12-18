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
//#include <regex>
using namespace std;

// �⺻�� ���� ���� ����
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


#pragma region vector_sort_vs_priority_queue_sort
//class my_type {
//public:
//    int a;
//    int b;
//
//};

//class my_func {
//public:
//    bool operator()(my_type& in1, my_type& in2) {
//        if (in1.a > in2.a) {
//            return true;
//        }
//        if (in1.a == in2.a) {
//            if (in1.b > in2.b) {
//                return true;
//            }
//            return false;
//        }
//
//        return false;
//    }
//};

//my_type a;
//my_type b;
//my_type c;
//
//a.a = 10;
//a.b = 20;
//
//b.a = 10;
//b.b = 30;
//
//c.a = 5;
//c.b = 50;
//
//priority_queue<my_type, vector<my_type>, my_func> mtpq;
//vector<my_type> mtv;
//mtpq.push(a);
//mtpq.push(b);
//mtpq.push(c);
//
//mtv.push_back(a);
//mtv.push_back(b);
//mtv.push_back(c);
//
//sort(mtv.begin(), mtv.end(), my_func());
//
//int size = mtpq.size();
//for (int i = 0; i < size; i++) {
//    my_type top = mtpq.top();
//    cout << top.a << top.b << endl;
//    mtpq.pop();
//}
//
//for (int i = 0; i < size; i++) {
//    cout << mtv[i].a << mtv[i].b << endl;
//}
#pragma endregion


#pragma region hash(map)


// �ؽ� > �������� ���� ����
string solution_hash_1(vector<string> participant, vector<string> completion) {
	// �Ʒ��� ��� ���
	sort(participant.begin(), participant.end());
	sort(completion.begin(), completion.end());

	for (int i = 0; i < completion.size(); i++) {
		if (participant[i] != completion[i])
			return participant[i];
	}

	return participant[participant.size() - 1];


	// �Ʒ��� ���� ó���� Ǯ���� ��
	//map<string, int> m;
	//for (auto it = completion.begin(); it != completion.end(); it++) {
	//    if (m.find(*it) == m.end())// ó�� ������ ��Ҹ� ����
	//        m.insert(pair<string, int>(*it, 1)); // �׸��� ã�� �� ����
	//    else {  // �ߺ� ����� ��� ++ �Ѵ�.
	//        ++m[*it];
	//    }
	//}
	//
	//for (auto it = participant.begin(); it != participant.end(); it++) {
	//    auto target = m.find(*it);

	//    if (target == m.end()) {
	//        return *it;
	//    }

	//    if (target->second == 0) {
	//        return *it;
	//    }
	//    else {
	//        target->second--;
	//    }
	//} 



}

// �ؽ� > ��ȭ��ȣ ���
bool solution_hash_2(vector<string> phone_book) {
	// ��� ���
	sort(phone_book.begin(), phone_book.end());

	for (int i = 0; i < phone_book.size() - 1; i++) {
		if (phone_book[i] == phone_book[i + 1].substr(0, phone_book[i].size())) {
			return false;
		}
	}

	return true;
}
// ���� �� ��
//bool is_prefix_h2(string& a, string& b) {
//    for (int i = 0; i < a.size(); i++) {
//        if (a[i] != b[i])
//            return false;
//    }
//    return true;
//}
//bool solution_h2(vector<string> phone_book) {
//    sort(phone_book.begin(), phone_book.end());
//
//    for (int i = 0; i < phone_book.size()-1; i++) {
//        if (is_prefix_h2(phone_book[i], phone_book[i + 1])) {
//            return false;
//        }
//    }
//
//    return true;
//}

// �ؽ� > ����
int solution_hash_3(vector<vector<string>> clothes) {
	// �� ���� ���
	int result = 1;
	map<string, int> m;

	for (auto i = 0; i < clothes.size(); i++) {
		m[clothes[i][1]]++; // �ƹ��͵� ��� �䷸�԰� �ȴ�. ���� ���� ���� Ǯ ���� ����.
	}
	for (auto it = m.begin(); it != m.end(); it++) {
		result *= (it->second + 1);
	}
	return result - 1;

	// �Ʒ��� ���� Ǭ ���.
	//int result = 1;
	//map<string, int> m;

	//for (int i = 0; i < clothes.size(); i++) {
	//    if (m.find(clothes[i][1]) == m.end())
	//        m.insert(pair<string, int>(clothes[i][1], 1));
	//    else {
	//        m[clothes[i][1]] += 1;
	//    }
	//}

	//for (auto it = m.begin(); it != m.end(); it++) {
	//    result *= (it->second + 1);
	//}
	//return result -1;
}

bool my_sort1(pair<int, int>& a, pair<int, int>& b) {
	if (a.second > b.second) // �÷��� ���� �� ���ٸ� �տ� �;��Ѵ�.
		return true;
	if (a.second == b.second) { // �÷��� ���� ���ٸ�
		if (a.first < b.first) { // �÷��� ���� ���ٸ� index �� �� ������ �տ� �;� �Ѵ�.
			return true;
		}
	}

	return false; // a �� �÷��� ���� �� �۰ų�, �÷��� ���� ������ �ε����� ũ�� ���� ���
}

bool my_sort2(pair<string, int>& a, pair<string, int>& b) {
	return a.second > b.second;
}

// �ؽ� > ����Ʈ�ٹ�
vector<int> solution_hash_4(vector<string> genres, vector<int> plays) {
	vector<int> answer;
	map<string, int> m1;
	map<string, vector<pair<int, int>>> m2;
	for (int i = 0; i < genres.size(); i++) {
		m1[genres[i]] += plays[i];
		m2[genres[i]].push_back(pair<int, int>(i, plays[i]));
	}

	for (auto it = m2.begin(); it != m2.end(); it++) {
		sort(it->second.begin(), it->second.end(), my_sort1);
	}

	vector<pair<string, int>> tempVec(m1.begin(), m1.end());
	sort(tempVec.begin(), tempVec.end(), my_sort2);

	for (int i = 0; i < tempVec.size(); i++) {
		for (int j = 0; j < m2[tempVec[i].first].size() && j < 2; j++) {
			answer.push_back(m2[tempVec[i].first][j].first);
		}
	}

	return answer;
}

// �ؽ� > ���ϸ�
using namespace std;

int solution_hash_5(vector<int> nums)
{
	unordered_set<int> us(nums.begin(), nums.end());
	return min(nums.size() / 2, us.size());
}
#pragma endregion


#pragma region stack_and_queue

// ����/ť > ��ɰ���
vector<int> solution_sq_1(vector<int> progresses, vector<int> speeds) {
	// ��� ���
	vector<int> answer;
	int daysAfter = 0;
	int daysAfterMax = 0; // ���� ���� �ִ밪�� �� ��쿡 �ٲ۴�.

	for (int i = 0; i < progresses.size(); i++) {
		// �� �κе� �߿��ϴ�. 100�̶�� �ϼ�ġ�� ������ �����Ƿ� while ���� �ʿ� ���� �̷��� �ݺ� Ƚ���� ���� �� �ִ�.
		daysAfter = (99 - progresses[i]) / speeds[i] + 1;

		if (daysAfter > daysAfterMax) {
			answer.push_back(1);
			daysAfterMax = daysAfter;
		}
		else {
			++answer.back(); // �̰� �߿��ϴ�. ��ǻ� �̰����� ���� vector �� stack �� queue �μ� ��� ���� �� �ִ�.
		}
	}

	return answer;
	// �Ʒ��� ���� Ǯ���� ��
	//vector<int> result;
	//int distributions = 0;
	//int daysAfter = 0;

	//while (progresses[0] + daysAfter * speeds[0] < 100) {
	//    daysAfter++;
	//}
	//distributions++;

	//for (int i = 1; i < progresses.size(); i++) {
	//    if (progresses[i] + speeds[i] * daysAfter >= 100) {
	//        distributions++;
	//        continue;
	//    }
	//    else {
	//        result.push_back(distributions);
	//        distributions = 0;
	//        while (progresses[i] + daysAfter * speeds[i] < 100) {
	//            daysAfter++;
	//        }
	//        i = i - 1;
	//    }
	//}

	//if (distributions >= 1)
	//    result.push_back(distributions);

	//return result;
}

// ����/ť > ������
int solution_sq_2(vector<int> priorities, int location) {
	int answer = 0;
	priority_queue<int, vector<int>, less<int>> pq;
	queue<pair<int, int>> entryQueue;
	int currentMax;

	for (int i = 0; i < priorities.size(); i++) {
		pq.push(priorities[i]); // �켱 ���� �Ǵ��� ���� pq �� ����
		entryQueue.push(pair<int, int>(i, priorities[i])); // �ڼ����鼭 ������ �ϱ� ���ؼ�..
	}

	while (true) { // location �� pop �� ������ ��� ������.
		currentMax = pq.top();
		pq.pop();

		while (true) { // pq �� ���� �ϳ��� ���Ұ� pop �� ������ ����.
			int checkedPriority = entryQueue.front().second;
			int checkedIdx = entryQueue.front().first;
			entryQueue.pop();

			if (checkedPriority == currentMax) {
				answer++;

				if (checkedIdx == location) {
					return answer;
				}
				break;
			}
			else { // ���� ó���� �Ұ����ϸ� �ٽ� Ǫ���Ѵ�.
				entryQueue.push(pair<int, int>(checkedIdx, checkedPriority));
			}
		}
	}

	return answer;
}

// ����/ť > �ٸ��� ������ Ʈ��
int solution_sq_3(int bridge_length, int weight, vector<int> truck_weights) {
	// �� ���� ���������� üũ�ؾ� �Ѵ�.
	// �� �տ� �ִ� ���� �� �ǳʸ�(bridge_length +1 �Ŀ�) ���� �� ���Կ��� �ǳ� �� ���Ը� ���� �� Ʈ���� �ø� �� �ִ��� ���� �Ѵ�.
	int answer = 0;
	int currentTotalWeight = 0;
	int index = 0;
	int end = truck_weights.size();
	queue<pair<int, int>> passing;

	while (true) {
		answer++; // �ð� 1�� ���

		if (!passing.empty()) {
			if ((answer - passing.front().first) >= bridge_length) { // ����� �ð� - ���� �ð� >= �ٸ� ���� �̸� �ǳ� ���̴�.
				currentTotalWeight -= passing.front().second; // ������ ���Ը� �� �� ���Կ��� ���ش�.
				passing.pop(); // �� �տ� �ִ� ���� ���� ���� �Ǿ����Ƿ� pop �Ѵ�.

				if (index == end && passing.empty()) // pop �� ���� �������̸� break
					break;
			}
		}

		if (index < end && currentTotalWeight + truck_weights[index] <= weight) {
			currentTotalWeight += truck_weights[index];
			passing.push(pair<int, int>(answer, truck_weights[index])); // �� ������ �ö�� �ð��� ���Ը� ���
			index++; // ���� Ʈ���� �� �� �ִ��� üũ.
		}
	}

	return answer;
}

// ����/ť > �ֽİ���
vector<int> solution_sq_4(vector<int> prices) {
	// ��� ���
	// �ٵ� ���� ó�� Ǭ ���� ������ �ӵ��� �� ���� ���̽��� ����. ���� ��ü�� �޸� ������� ���Ƽ�..
	vector<int> answer(prices.size());
	stack<int> s;

	for (auto i = 0; i < prices.size(); i++) {
		// stack ���� ���� ���� ���ö������� pop�� �ؾ��Ѵ�.
		while (!s.empty() && prices[s.top()] > prices[i]) {
			// ������ �� ���� �ִ� index �� ���� �ð��� ���� �����̴�. 
			// ���� index - stack top �� ����� �ð��� �Ǵ� ���̴�.
			answer[s.top()] = i - s.top();
			s.pop(); // ���� �񱳴�󺸴� �۰ų� ���� ���� ���� �������� pop �Ѵ�.
		}
		s.push(i);
	}

	while (!s.empty()) { // ��� ���ÿ� �׿� �ִ� ���� �ִٸ� ���� ���� ������ pop �ϸ� �Ҵ����ش�.
		answer[s.top()] = prices.size() - 1 - s.top();
		s.pop();
	}

	return answer;

	// ���� Ǭ ���
	//vector<int> answer;
	//int checked;
	//for (int i = 0; i < prices.size(); i++) {
	//	checked = 0;
	//	for (int j = i + 1; j < prices.size(); j++) {
	//		if (prices[i] <= prices[j])
	//			checked++;
	//		else { // �� �κ��� �ʹ� �򰥸��µ�, ������ �ٷ� �������µ� 1�ʰ� �������� ���� ������ �����Ѵ�..
	//			checked++;
	//			break;
	//		}
	//	}
	//	answer.push_back(checked);
	//}
	//return answer;
}

#pragma endregion


#pragma region heap
// �� > �� �ʰ�
int solution_heap_1(vector<int> scoville, int K) {
	// ���� Ǭ ���, pq �� �� �� ����Ʈ�ϰ� ���� �� �ִ�. ���� �� ���� �߰�����.
	int answer = 0;
	bool isAllOverK = false;
	priority_queue<int, vector<int>, greater<int>> pq(scoville.begin(), scoville.end()); // �̷��� �ʱ�ȭ�� �����ϴ�.

	while (pq.top() < K) {
		if (pq.size() == 1)
			return -1;

		int least1 = pq.top();
		pq.pop();

		int least2 = pq.top();
		pq.pop();

		pq.push(least1 + least2 * 2);
		answer++;
	}

	return answer;

}

// �� > ��ũ ��Ʈ�ѷ�
struct solution_heap_2_sort {
	bool operator()(vector<int>& a, vector<int>& b) {
		if (a[1] > b[1]) // ��û �ð��� �� ���� �༮�� �տ� ���� �Ѵ�.
			return true;
		if (a[1] == b[1]) {
			return a[0] > b[0]; // ������ �ð��� �� ����(����) �༮�� �տ� ���� �Ѵ�.
		}

		return false;
	}
};
int solution_heap_2(vector<vector<int>> jobs) {
	int answer = 0;
	int times = 0;
	vector<int> top(2);
	priority_queue<vector<int>, vector<vector<int>>, solution_heap_2_sort> pq(jobs.begin(), jobs.end());
	priority_queue<vector<int>, vector<vector<int>>, solution_heap_2_sort> pq2; // �и� �۾����� ��� �־��ִ� queue

	while (true) {
		if (!pq.empty()) {
			top = pq.top();
			pq.pop();
			if (top[0] <= times) { // ���� �帥 �ð��� ������ �ð����� ũ�� ó���Ѵ�.
				times += top[1]; // ó���ϴµ� �ɸ� �ð���ŭ times �� ���Ѵ�.
				answer += times - top[0];
				if (!pq2.empty()) {
					while (!pq2.empty()) // ó���ϰ� ���� �и� �۾����� �ٽ� ť�� �־��ش�.
					{
						pq.push(pq2.top());
						pq2.pop();
					}
				}
			}
			else {
				pq2.push(top); // ���� �ð� ������ �ȵǼ� �и� �۾����� ���Ϳ� �־��ش�.
			}
		}

		if (pq.empty() && !pq2.empty()) { // pq �� ��� queue �� ����µ� pq2 �� �Ⱥ���ٴ� ���� �ð��� �����ؼ� �۾��� �׿��ٴ� ���̴�.
			// �̷� ������ �ð��� ������Ų��.
			times++;

			while (!pq2.empty()) {
				pq.push(pq2.top());
				pq2.pop();
			}
		}

		if (pq.empty() && pq2.empty())
			break;
	}

	answer = answer / jobs.size();
	return answer;
}

// �� > ���߿켱����ť
vector<int> solution_heap_3(vector<string> operations) {
	vector<int> answer;
	int operationCount = operations.size();
	multiset<int> mset; // min max ���� ���� ������ multiset �� Ȱ���ϸ� ���ٴ� ���� ����.

	for (int i = 0; i < operationCount; i++) {
		if (operations[i][0] == 'I') { // insert �� ����̹Ƿ� mset �� �����Ѵ�.
			mset.insert(atoi(&operations[i][2]));
		}
		else {
			if (mset.size() >= 1) {
				if (operations[i][2] == '1') { // �ִ밪�� �����ϴ� ���
					mset.erase(--mset.end());
				}
				else { // �ּҰ��� �����ϴ� ���
					mset.erase(mset.begin());
				}
			}
		}
	}

	if (mset.size() == 0)
		return { 0,0 };

	answer.push_back(*(--mset.end())); // �ִ밪 ����
	answer.push_back(*mset.begin()); // �ּҰ� ����
	return answer;
}
#pragma endregion


#pragma region sort

// ���� > K��°��
vector<int> solution_sort_1(vector<int> array, vector<vector<int>> commands) {
	vector<int> answer;

	for (int i = 0; i < commands.size(); i++) {
		vector<int> temp(array.begin() + commands[i][0] - 1, array.begin() + commands[i][1]);
		sort(temp.begin(), temp.end());
		answer.push_back(temp[commands[i][2] - 1]);
	}

	return answer;
}


// ���� > ���� ū ��
bool solution_sort_2_comp(int a, int b) {
	// ��� ���
	string a_str = to_string(a);
	string b_str = to_string(b);

	return a_str + b_str > b_str + a_str; // ���.. ���� ����.

	// �Ʒ��� ������ Ǯ���� ��
	// a�� b �� �� �ڸ��� �� ũ�� �� ���� �� ū ��
	// ���� a�� b�� �ڸ� ���ڰ� �ٸ���, ª�� ���� �������� ���缭 
	//string a_str = to_string(a);
	//string b_str = to_string(b);
	//int a_str_size = a_str.size();
	//int b_str_size = b_str.size();

	//if (a_str_size > b_str_size) { // a �� �ڸ����� �� ���� ���
	//	int i = 0;
	//	for (;i < b_str_size; i++) {
	//		if (a_str[i] > b_str[i]) return true; // a �� �� ũ�� true
	//		else if (a_str[i] < b_str[i]) return false; // b �� �� ũ�� false
	//	}

	//	for (int j = i; j < a_str_size; j++) {
	//		if (a_str[j] > b_str[i-1]) return true;
	//		else if (a_str[j] < b_str[i-1]) return false;
	//	}

	//	return false;
	//}
	//else { // b �� �ڸ����� �� ���ų� ���� ���� ���
	//	int i = 0;
	//	for (; i < a_str_size; i++) {
	//		if (a_str[i] > b_str[i]) return true; // a �� �� ũ�� true
	//		else if (a_str[i] < b_str[i]) return false; // b �� �� ũ�� false
	//	}

	//	for (int j = i; j < b_str_size; j++) {
	//		if (a_str[i - 1] > b_str[j]) return true;
	//		else if (a_str[i - 1] < b_str[j]) return false;
	//	}

	//	return false;
	//}
}

string solution_sort_2(vector<int> numbers) {
	string answer;
	sort(numbers.begin(), numbers.end(), solution_sort_2_comp);

	for (int i = 0; i < numbers.size(); i++) {
		answer += to_string(numbers[i]);
	}

	if (answer[0] == '0')
		answer = "0";

	//if (atoi(answer.c_str()) == 0) // ���α׷��ӽ����� stoi �� �ȵȴ�.
	//	answer = "0";

	return answer;
}

// ���� > H-Index
int solution_sort_3(vector<int> citations) {
	int answer = 0;
	sort(citations.begin(), citations.end(), greater<int>());

	for (int i = 0; i < citations.size(); i++) {
		answer++;
		// 6,5,3,1,0 �� ��� answer �� �� ������ ���ڰ� ������ ������ ó�� answer �� �����ϸ� �ȴ�.
		if (answer >= citations[i])
			return answer;
	}
}

#pragma endregion


#pragma region Brute-Force
//struct my_comp_brute_force_1 {
//
//	bool operator()(pair<int, int>& a, pair<int, int>& b) {
//		return a.second > b.second;
//	}
//};

vector<vector<int>> supo_answers = {
	{1,2,3,4,5},
	{2,1,2,3,2,4,2,5},
	{3,3,1,1,2,2,4,4,5,5}
};

vector<int> solution_brute_force_1(vector<int> answers) {
	// ��� ���
	vector<int> ret;
	vector<int> supos = { 0,0,0 };

	for (int i = 0; i < answers.size(); i++) {
		if (supo_answers[0][i % supo_answers[0].size()] == answers[i]) supos[0]++;
		if (supo_answers[1][i % supo_answers[1].size()] == answers[i]) supos[1]++;
		if (supo_answers[2][i % supo_answers[2].size()] == answers[i]) supos[2]++;
	}

	auto it = max_element(supos.begin(), supos.end()); // �ִ� ũ�⸸ �˰� ������ �ȴٸ�, max_element �� ���� ���� ���Ŀ��� for������ ���ϸ� �ȴ�.
	int max = *it;

	for (int i = 0; i < supos.size(); i++) {
		if (supos[i] == max)
			ret.push_back(i + 1);
	}

	return ret;
	// �Ʒ��� ������ ���� �ߴ� ���
	//vector<int> answer;
	//vector<pair<int, int>> supo_score = { {0,0},{1,0},{2,0} };
	//vector<int> supo1_answer = { 1, 2, 3, 4, 5 };
	//vector<int> supo2_answer = { 2, 1, 2, 3, 2, 4, 2, 5 };
	//vector<int> supo3_answer = { 3, 3, 1, 1, 2, 2, 4, 4, 5, 5 };


	//for (int i = 0; i < answers.size(); i++) {
	//	// ����1
	//	if (supo1_answer[i % supo1_answer.size()] == answers[i])
	//		supo_score[0].second++;

	//	// ����2
	//	if (supo2_answer[i % supo2_answer.size()] == answers[i])
	//		supo_score[1].second++;

	//	// ����3
	//	if (supo3_answer[i % supo3_answer.size()] == answers[i])
	//		supo_score[2].second++;
	//}

	//sort(supo_score.begin(), supo_score.end(), my_comp_brute_force_1());

	//answer.push_back(supo_score[0].first + 1);

	//for (int i = 1; i < supo_score.size(); i++) {
	//	if (supo_score[0].second == supo_score[i].second) // �ִ밪�� �� ���� ��Ұ� ������ �߰�
	//		answer.push_back(supo_score[i].first + 1);
	//	else // ������ �ڸ� �� �� �ʿ䰡 �����Ƿ� break
	//		break;
	//}

	//return answer;
}
#pragma endregion


#pragma region DFS_BFS

int answer_dfs_bfs_1;
void DFS_1(vector<int>& numbers, int depth, int sum, int target) {
	if (sum == target && depth == numbers.size() - 1) // ��ǥ ã�����Ƿ� �� ++
	{
		answer_dfs_bfs_1++;
		return;
	}

	if (depth == numbers.size() - 1) // depth ���� ���������Ƿ� ++
		return;

	depth++;
	DFS_1(numbers, depth, sum + numbers[depth], target);
	DFS_1(numbers, depth, sum - numbers[depth], target);

}
int solution_dfs_bfs_1(vector<int> numbers, int target) {
	DFS_1(numbers, 0, numbers[0], target);
	DFS_1(numbers, 0, -numbers[0], target);

	return answer_dfs_bfs_1;
}


void DFS_2(vector<bool>& visited, vector<vector<int>>& computers, int start) {
	// ��͸��� stack ���� ���� Ǯ��
	stack<int> s;
	s.push(start);

	while (!s.empty()) {
		int from = s.top();
		s.pop();
		for (int i = 0; i < visited.size(); i++) {
			if ((visited[i] == false && computers[from][i])) { // ���� ���� ������ �湮�Ϸ��� �ϴ� ���� �湮 �����ϰ�, �湮�� ���� ������
				visited[i] = true; // �ش� ��Ҹ� �湮�Ѵ�.
				s.push(i); // �湮 ������ ��带 �ٽ� Ǫ���Ѵ�.
			}
		}
	}

	// �Ʒ��� ��ͷ� ���� Ǯ��
	//for (int i = 0; i < visited.size(); i++) {
	//	if ( (visited[i] == false && computers[start][i])) { // ���� ���� ������ �湮�Ϸ��� �ϴ� ���� �湮 �����ϰ�, �湮�� ���� ������
	//		visited[i] = true; // �ش� ��Ҹ� �湮�Ѵ�.
	//		DFS_2(visited, computers, i); // �湮�� ��ҿ��� �ٽ� DFS�� �����Ѵ�.
	//	}
	//}
}

// ����/�ʺ� �켱 Ž�� > ��Ʈ��ũ
int solution_dfs_bfs_2(int n, vector<vector<int>> computers) {
	int answer = 0;
	vector<bool> visited(n); // vector �� �ʱⰪ�� �� 0���� �ʱ�ȭ�ȴ�.

	// i ���� �����ؼ� ���� Ž���� �����ϸ� ������ ���� �� visted�� true�� �ٲ۴�.
	for (int i = 0; i < computers.size(); i++) {
		if (!visited[i]) {// ���� �� ��Ҹ� �湮�� ���� ������
			visited[i] = true;
			DFS_2(visited, computers, i);
			answer++; // DFS�� ������ �ϳ��� ��Ʈ��ũ�� �ϼ��ǹǷ� ��Ʈ��ũ �� �� �߰�.
		}
	}

	return answer;

	// �Ʒ��� ��ͷ� ���� Ǯ��
	//int answer = 0;
	//vector<bool> visited(n); // vector �� �ʱⰪ�� �� 0���� �ʱ�ȭ�ȴ�.

	//// i ���� �����ؼ� ���� Ž���� �����ϸ� ������ ���� �� visted�� true�� �ٲ۴�.
	//for (int i = 0; i < computers.size(); i++) {
	//	if (!visited[i]) {// ���� �� ��Ҹ� �湮�� ���� ������
	//		visited[i] = true;
	//		DFS_2(visited, computers, i);
	//		answer++; // DFS�� ������ �ϳ��� ��Ʈ��ũ�� �ϼ��ǹǷ� ��Ʈ��ũ �� �� �߰�.
	//	}
	//}

	//return answer;
}
#pragma endregion


#pragma region Greedy


// Ž��� > ū �� �����
string solution_greedy_2(string number, int k) {
	string answer = "";
	int targetLength = number.size() - k;
	char max;
	int maxIndex;
	// �տ��� k ��ŭ Ȯ���ϸ鼭, ���� �� �ִ� ���� ū������ answer�� �ְ�, k�� ���ҽ�Ų��.
	// ���������� ������+selectedNumber �� ���ڿ� ���� �ٴٸ��� ��� �� �̴´�.

	for (int i = 0; i < number.size(); i++) {
		max = 0; // max�� �ʱ�ȭ
		maxIndex = -1; // maxIndex �ʱ�ȭ, -1�̸� ã�� ���ߴٴ� ���̴�.
		for (int j = i; j <= i + k; j++) { // ���� ���� ���� �ִ� ���� ��ġ�� ã�� ��
			if (max < number[j]) {
				max = number[j];
				maxIndex = j;
			}
		}
		k -= (maxIndex - i); // ���ڸ� ���� �� ��ŭ k�� ����
		i = maxIndex; // ���� ���ڿ� �˻�� ���� answer�� �߰��� ���� �׸���� �˻��Ѵ�.
		answer += max;
		if (answer.size() == targetLength) // ��ǥ ���̿� �����ϸ� ���ڿ��� ����
			return answer;
	}

	return answer;
}
#pragma endregion


#pragma region Dynamic Programming


// ������ȹ�� > N���� ǥ��
int solution_dynamic_1(int N, int number) {
	vector<set<int>> entries(9);
	entries[1].insert(N);
	for (int i = 1; i < 9; i++) { // i ���� N �� ����ϴ� ���
		for (int j = 1; j <= i; j++) { // j + i-j �� �ɰ��� ����� ���� ��� ���ϱ� ���� ��
			for (auto it1 = entries[j].begin(); it1 != entries[j].end(); it1++) {
				for (auto it2 = entries[i - j].begin(); it2 != entries[i - j].end(); it2++) {
					// ����
					entries[i].insert(*it1 + *it2);

					// ����
					entries[i].insert(*it1 * *it2);

					// ����
					entries[i].insert(*it1 - *it2);
					entries[i].insert(*it2 - *it1);

					// ������
					if (*it2 != 0)
						entries[i].insert(*it1 / *it2);
					if (*it1 != 0)
						entries[i].insert(*it2 / *it1);

				}
			}
		}
		string temp;
		for (int t = 1; t <= i; t++) {
			temp += to_string(N);
		}
		entries[i].insert(atoi(temp.c_str()));
	}

	for (int i = 1; i < 9; i++) { // N�� �����ؼ� ������ ���
		auto it = entries[i].find(number);

		if (it != entries[i].end())
			return i;
	}

	return -1;
}

#pragma endregion


#pragma region Graph

// �׷��� > ���� �� ���
bool isPath[20001][20001];

int solution_graph_1(int n, vector<vector<int>> edge) {
	// 1�� ��忡�� ���� �ָ� ������ ����� ������ ���Ѵ�.
	// input: ������ ���� ������ ��� 2���� �迭 edge, ��� ���� n
	// 2 <= n <= 20000
	// ������ �����
	// 1 <= edge.size() <= 50000


	// 1�� ������ ����
	// 1�� ��忡�� �� �� �ִ� �� ��� �湮 üũ, �Ÿ� 1�� �Ҵ�
	// �湮�� ���� ť�� ����ְ� �ٽ� �ű⼭ �湮�� �� �ִ� ���� �湮 �Ÿ� 2�� �Ҵ�. (�̹� queue�� ������ �湮 x)
	const int impossibleDistance = 50000;
	vector<int> dist(n, 40000);
	vector<bool> checked(n);
	queue<int> readyQ;
	queue<int> processQ;

	for (int i = 0; i < edge.size(); i++) {
		isPath[edge[i][0] - 1][edge[i][1] - 1] = true;
		isPath[edge[i][1] - 1][edge[i][0] - 1] = true;
	}

	int answer = 0;

	readyQ.push(0);
	int level = 0;
	checked[0] = true;
	dist[0] = 0;


	while (!readyQ.empty()) {
		while (!readyQ.empty()) { // ready queue �� �ִ� �� process queue �� �ű��.
			processQ.push(readyQ.front());
			readyQ.pop();
		}
		level++;
		while (!processQ.empty()) {
			int front = processQ.front();
			processQ.pop();

			for (int i = 0; i < n; i++) {
				if ((isPath[front][i] || isPath[i][front]) && !checked[i]) { // �� �� �����鼭 ���� üũ �ȵ� ����� ����
					checked[i] = true;
					readyQ.push(i);
					dist[i] = min(dist[i], level);
				}
			}
		}
	}

	sort(dist.begin(), dist.end(), greater<int>());

	int biggest = dist[0];
	for (int i = 0; i < dist.size(); i++) {
		if (biggest == dist[i])
			answer++;
		else
			break;
	}

	return answer;
}
#pragma endregion


#pragma region KKO-Blind
namespace kko {

	namespace blind_2018 {

		// �������
		namespace secret_map {

			// �� Ǯ��
			int divider[] = {
				1,
				1 << 1,
				1 << 2,
				1 << 3,
				1 << 4,
				1 << 5,
				1 << 6,
				1 << 7,
				1 << 8,
				1 << 9,
				1 << 10,
				1 << 11,
				1 << 12,
				1 << 13,
				1 << 14,
				1 << 15,
				1 << 16
			};

			char one = '#';
			char zero = ' ';

			string decode(int N, int cryptedNum) {
				static unordered_map<int, string> cachedMap;

				if (cachedMap.find(cryptedNum) != cachedMap.end()) {
					return cachedMap[cryptedNum];
				}

				string decoded = string(N, zero);
				int dividend = cryptedNum;

				for (int i = N - 1; i >= 0; i--) {
					if ((dividend / divider[i]) >= 1) {
						decoded[N - 1 - i] = one;
					}
					dividend = dividend % divider[i];
				}

				cachedMap[cryptedNum] = decoded;
				return decoded;
			}

			vector<string> solution(int n, vector<int> arr1, vector<int> arr2) {
				vector<int> original(arr1.size());
				vector<string> answer;

				for (int i = 0; i < original.size(); i++) {
					original[i] = arr1[i] | arr2[i];
				}

				for (auto i : original) {
					answer.push_back(decode(n, i));
				}

				return answer;
			}

			// �ٸ� ���� ���̴� Ǯ��
			vector<string> solution_other(int n, vector<int> arr1, vector<int> arr2) {
				vector<int> original(arr1.size());
				vector<string> answer;

				for (int i = 0; i < original.size(); i++) {
					original[i] = arr1[i] | arr2[i];

					string temp = "";
					for (int j = 0; j < n; j++) {
						if (original[i] % 2 == 1) {
							temp = '#' + temp;
						}
						else {
							temp = ' ' + temp;
						}

						original[i] = original[i] / 2;
					}

					answer.push_back(temp);
				}

				return answer;
			}

		}

		// ĳ��
		namespace cache {

			//  0 <= cacheSize <= 30
			// cities.size <= 100000 
			// ���� �̸� �ִ� 20���� ������, ��ҹ��� ���� x

			constexpr int maxAge = 1000000;
			int solution(int cacheSize, vector<string> cities) {
				if (cacheSize == 0) {
					return 5 * cities.size();
				}

				int answer = 0;

				bool isCacheHit = false;
				int timeElapsed = 0;

				for (auto& cityName : cities) {
					transform(cityName.begin(), cityName.end(), cityName.begin(), tolower);
				}

				for (auto& str : cities) {
					cout << str << endl;
				}

				unordered_map<string, int> cache;

				for (auto& city : cities) {
					timeElapsed++;

					if (cache.find(city) == cache.end()) {
						// ĳ�� ���� ������ �ִٸ� ĳ��
						if (cache.size() < cacheSize) {
							cache[city] = timeElapsed;
						}
						// ĳ�� ���� ������ ���ٸ�
						else {
							int lruTime = maxAge;
							string deletedCityName = "";

							// cacheHitTime �� ���� ���� �༮�� ã�Ƽ� ����, ���ο� ������ ĳ��
							for (auto& [cityName, cacheHitTime] : cache) {
								if (lruTime >= cacheHitTime) {
									deletedCityName = cityName;
									lruTime = cacheHitTime;
								}
							}

							cache.erase(deletedCityName);

							cache[city] = timeElapsed;
						}

						isCacheHit = false;
					}
					// ĳ�� hit �Ǿ����� ���� �ð��� �Է�
					else {
						cache[city] = timeElapsed;
						isCacheHit = true;
					}

					answer += (isCacheHit) ? 1 : 5;

				}

				return answer;
			}
		}

		// ���� Ŭ�����͸�
		namespace news_clustering {
			// ���ڿ��� ������ ����, ����, Ư�� ���� ���� ���ڰ� ������ �� ���ش�.
			// �� ���� ���ڿ��� �� �ҹ��ڷ� �ٲ۴�.
			// ���ڿ��� 2���� �ɰ� ���ڿ��� �ٲٰ�, �̸� dict �� �����.
			// ������� �� dict �� min, max �� ���ؼ� AnB / AuB * 65536 �� ����

			void makeStrDict(unordered_map<string, int>& outDict, string& str) {
				for (int i = 0; i < str.length(); i++) {
					if (isalpha(str[i]) && isalpha(str[i + 1])) {
						string subStr = "";
						subStr.push_back(tolower(str[i]));
						subStr.push_back(tolower(str[i + 1]));
						cout << subStr << endl;
						if (outDict.find(subStr) == outDict.end()) {
							outDict[subStr] = 1;
						}
						else {
							outDict[subStr]++;
						}
					}
				}
			}

			int solution(string str1, string str2) {
				// ���ڿ��� 2���� �ɰ��� �������� �����.
				// ���ӵ� ���ڿ��� alpha �������� ���ڿ��� �����.

				unordered_map<string, int> strDict1;
				unordered_map<string, int> strDict2;

				makeStrDict(strDict1, str1);
				makeStrDict(strDict2, str2);

				int sizeOfInterSection = 0;
				int sizeOfUnion = 0;

				for (auto& [words1, cnt1] : strDict1) {
					// �� �ܾ� ���տ� ��� �����ϸ�
					if (strDict2.find(words1) != strDict2.end()) {
						sizeOfInterSection += min(cnt1, strDict2[words1]);
						sizeOfUnion += max(cnt1, strDict2[words1]);
					}
					// �ܾ� ���� 1���� �ִµ� 2���� ������ 1�͸� ����
					else {
						sizeOfUnion += cnt1;
					}
				}

				for (auto& [words2, cnt2] : strDict2) {
					// �տ��� ù �ܾ� ���� �������� �Ͽ����Ƿ�, �� ��° �ܾ� ���տ��� �ִ� ����̸�
					if (strDict1.find(words2) == strDict1.end()) {
						sizeOfUnion += cnt2;
					}
				}

				if (sizeOfUnion == 0) {
					return 65536;
				}

				int answer = ((double)(sizeOfInterSection) / sizeOfUnion) * 65536;
				return answer;
			}
		} // news_clustering

		// ��Ʈ ����
		namespace dart_game {
			// S, D, T �� ���� ����^1, ����^2, ����^3
			// S, D, T �� �������� �ϳ��� ����

			// * ��Ÿ�� ��÷�� �ش� ������ �ٷ� ���� ���� ������ 2��
			// # ������ ��÷ �� �ش� ������ ���̳ʽ�

			// *�� ù ��° ��ȸ������ ���� �� �ְ�, ù ��° *�� ������ 2�谡 �ȴ�.
			// * ȿ���� �ٸ� *& ȿ���� ��ø�ȴ�. �� ��� ��ø�� *�� ������ 4�谡 �ȴ�.
			// *�� ȿ���� # ȿ���� ��ø�� �� �ְ�, �� ��� ������ -2�谡 �ȴ�.

			// *, #�� �������� �� �� �ϳ��� ����, �������� ���� ���� ����


			// ���ڰ� ������ push
			// SDT üũ �� ���� �ο�
			// �ɼ� äũ �� ���� �ο� (*������ �� �ڷ� 2��, #�� �ش� ���븸 -1 ���ϱ�

			int score[4];

			int solution(string dartResult) {

				int currentIdx = 0;
				int consecutiveDigitCnt = 0;

				// dartResult ���� ���ڷ� �� �κи� �����Ѵ�.
				vector<string> numberStrings;
				vector<string> expressionStrings;

				string tempStr = "";
				for (int i = 0; i < dartResult.length(); i++) {
					if (isdigit(dartResult[i])) {
						if (tempStr != "") {
							expressionStrings.push_back(tempStr);
							tempStr = "";
						}
						consecutiveDigitCnt++;
					}
					else {
						tempStr += dartResult[i];
						if (consecutiveDigitCnt > 0) {
							numberStrings.push_back(dartResult.substr(i - consecutiveDigitCnt, consecutiveDigitCnt));
							consecutiveDigitCnt = 0;
						}
					}
				}

				if (tempStr != "") {
					expressionStrings.push_back(tempStr);
				}

				for (int i = 1; i < 4; i++) {
					score[i] = atoi(numberStrings[i - 1].c_str());
				}

				for (auto& str : expressionStrings) {
					currentIdx++;
					for (auto i = 0; i < str.length(); i++) {
						char tempChar = str[i];
						switch (tempChar)
						{
						case 'D':
							score[currentIdx] = pow(score[currentIdx], 2);
							break;
						case 'T':
							score[currentIdx] = pow(score[currentIdx], 3);
							break;
						case '#':
							score[currentIdx] = -score[currentIdx];
							break;
						case '*':
							score[currentIdx] = score[currentIdx] * 2;
							score[currentIdx - 1] = score[currentIdx - 1] * 2;
							break;
						default:
							break;
						}
					}
				}

				int answer = 0;

				for (int i = 1; i < 4; i++) {
					answer += score[i];
				}

				return answer;
			}

			int solution_other(string dartResult) {
				int digit;
				char bonus;
				char option;

				stringstream ss(dartResult);

				for (int i = 1; i < 4; i++)
				{
					ss >> digit;
					bonus = ss.get();
					option = ss.get();

					// �� ���ڰ� �ƴϸ� option �� �ƴϴ�.
					if ((option != '*') && (option != '#'))
					{
						ss.unget();
					}

					score[i] = digit;

					switch (bonus)
					{
					case 'D':
						score[i] = pow(score[i], 2);
						break;

					case 'T':
						score[i] = pow(score[i], 3);
						break;

					default:
						break;
					}

					switch (option)
					{
					case '*':
						score[i] = 2 * score[i];
						score[i - 1] = 2 * score[i - 1];
						break;

					case '#':
						score[i] = -score[i];
						break;

					default:
						break;
					}
				}

				int answer = 0;

				for (int i = 1; i < 4; i++) {
					answer += score[i];
				}

				return answer;
			}
		}

		// ������4���
		namespace friends_4blocks {
			/*
			"CCBDE"
			"AAADE"
			"AAABF"
			"CCBBF"

			"   DE"
			"   DE"
			"CCBBF"
			"CCBBF"

			"    E"
			"    E"
			"   DF"
			"   DF"

			///

			"TTTANT"
			"RRFACC"
			"RRRFCC"
			"TRRRAA"
			"TTMMMF"
			"TMMTTJ"

			"   A  "
			"   A  "
			"T TFNT"
			"TTFRAA"
			"TTMMMF"
			"TMMTTJ"

			"   A  "
			"   A  "
			"T TFNT"
			"  FRAA"
			"  MMMF"
			"TMMTTJ"


			*/

			// ����, ��, ����, �� �������� üũ
			int dr[4] = { 0, 0, 1, 1 };
			int dc[4] = { 0, 1, 1, 0 };
			char erasedChar = '@';

			void checkErased(int curR, int curC, const vector<string>& board, vector<vector<bool>>& outToBeErased, int& outEraseCount)
			{
				char curCharacter = board[curR][curC];

				// ���� ������ ���ڰ� ������ �ƹ��͵� ���� �ʴ´�.
				if (curCharacter == erasedChar)
				{
					return;
				}

				// 3 ������ üũ�ؼ� ���� ���ڿ� ������
				if (curCharacter == board[curR + dr[1]][curC + dc[1]] &&
					curCharacter == board[curR + dr[2]][curC + dc[2]] &&
					curCharacter == board[curR + dr[3]][curC + dc[3]]
					)
				{
					// 4���� ������ ������ ������ �������� ǥ��, ������ ���� ����
					for (int i = 0; i < 4; i++) {
						if (outToBeErased[curR + dr[i]][curC + dc[i]] == false) {
							outToBeErased[curR + dr[i]][curC + dc[i]] = true;
							outEraseCount++;
						}
					}
				}
			}

			bool checkAndMoveDown(int curR, int curC, vector<string>& board, int m)
			{
				int targetR = curR + dr[3];
				int toBeMovedR = curR;

				while (true)
				{
					if (targetR <= m - 1 && board[targetR][curC] == erasedChar)
					{
						toBeMovedR = targetR;
						targetR += dr[3];
					}
					else {
						break;
					}


				}

				// �̵��ϰ� �ž��ٸ� �����δ�.
				if (toBeMovedR != curR)
				{
					swap(board[curR][curC], board[toBeMovedR][curC]);
					return true;
				}
				else {
					return false;
				}
			}

			int solution(int m, int n, vector<string> board)
			{
				int answer = 0;

				while (true)
				{
					// �ȿ��� ������ �͵��� �� üũ, ������ ���� üũ�� �ȵǸ� �ű⼭ ����
					// ������ �͵��� �����. ������ ������ answer �� ���Ѵ�.
					// ���� �迭�� ����.

					int toBeErasedCnt = 0;
					vector<vector<bool>> toBeErased(m, vector<bool>(n, false));

					for (int r = 0; r < m - 1; r++)
					{
						for (int c = 0; c < n - 1; c++)
						{
							checkErased(r, c, board, toBeErased, toBeErasedCnt);
						}
					}

					if (toBeErasedCnt == 0)
					{
						break;
					}

					answer += toBeErasedCnt;

					// ���� �� �����
					for (int r = 0; r < m; r++)
					{
						for (int c = 0; c < n; c++)
						{
							if (toBeErased[r][c])
							{
								board[r][c] = erasedChar;
							}
						}
					}

					// board �� ����.
					for (int r = m - 1; r >= 0; r--)
					{
						for (int c = n - 1; c >= 0; c--)
						{
							checkAndMoveDown(r, c, board, m);
						}
					}

				}
				return answer;
			}
		}

		// ��Ʋ����
		namespace shuttle_bus {

			// ��Ʋ�� �������� ���� ������ ����� �¿��.
			// ��Ʋ�� 09:00 ���� n ȸ t �� �������� ����
			// ��Ʋ ���� Ƚ�� 1 <= n <= 10
			// ��Ʋ ���� ���� 1 <= t <= 60
			// ��Ʋ �ִ� �ο� �� 1 <= m <= 45
			// 23:59 ���� ��� ���� ���ư��⿡, ������ ��Ʋ�� Ÿ�� ���� ����. �ִ� ���� �ð��� Ÿ�� 23:59��
			// ���� ������ �ڱⰡ ������ �ð��뿡�� �� �������� ����.

			string makeFormatString(int hour, int minute) {
				char temp[6] = "";
				sprintf(temp, "%02d:%02d", hour, minute);
				return string(temp);
			}

			string oneMinuteBeforeTime(string time) {
				int hour = atoi(time.substr(0, 2).c_str());
				int minute = atoi(time.substr(3, 2).c_str());

				minute--;
				if (minute < 0) {
					hour--;
					minute = 59;
				}

				return makeFormatString(hour, minute);
			}

			string solution(int n, int t, int m, vector<string> timetable) {
				sort(timetable.begin(), timetable.end());

				int hour = 9;
				int minute = 0;

				vector<string> busArrivedTimes;
				int totalCrewCnt = timetable.size();

				busArrivedTimes.push_back(makeFormatString(hour, minute));
				for (int i = 1; i < n; i++) {
					minute += t;
					hour += (minute / 60);
					minute = minute % 60;

					busArrivedTimes.push_back(makeFormatString(hour, minute));
				}

				int lastRidingCrewIdx = -1;
				int firstWaitingCrewIdx = 0;
				int lastRidedBusIdx = -1;
				bool isLastBusAvailable = true;
				int lastBusIdx = busArrivedTimes.size() - 1;

				for (int i = 0; i < busArrivedTimes.size(); i++) {
					isLastBusAvailable = true;
					for (int j = 0; j < m; j++) {
						if (timetable[firstWaitingCrewIdx] <= busArrivedTimes[i]) {
							// ���� ���� �ð����� ������ ��� ž��
							lastRidingCrewIdx++;
							firstWaitingCrewIdx++;

							// ������ ������ ����� �� �̻� ��ź�ٰ� üũ
							if (j == m - 1) {
								isLastBusAvailable = false;
							}

							// ���������� ž���� ���� ž�� �ð� ���
							lastRidedBusIdx = i;

							// �¿�� ���߿� ��ü �ο��� �� �¿��ٸ� �ߴ��Ѵ�.
							if (lastRidingCrewIdx == totalCrewCnt - 1) {
								break;
							}
						} // if
					} // for
				} // for

				// �Ʒ� �κ� ���߿� ���� �Ǹ� �� �� �� �� ����ص���.
				// ������ ������ �� �� ��Ȳ�� �ƴ϶�� ������ ������ �� �ð��� Ÿ�� ���� ���� �ʰ� ����ϴ� �ð��� ���̴�.

				// ������ ������ �� á�ٸ�
				if (lastRidedBusIdx == lastBusIdx && !isLastBusAvailable) {
					return oneMinuteBeforeTime(timetable[lastRidingCrewIdx]);
				}
				else {
					return busArrivedTimes[lastBusIdx];
				}
			}
		}

		// �߼� Ʈ����
		namespace chuseok_traffic {
			// TODO: �����, �ٸ� ���� �� �� ������ �� �� �� Ȯ�� �ʿ�
			int toMilliSeconds(int hours, int minutes, int seconds, int milliseconds) {
				return
					hours * 3600 * 1000 +
					minutes * 60 * 1000 +
					seconds * 1000 +
					milliseconds;
			}

			int solution(vector<string> lines) {
				vector<pair<int, int>> v;
				int answer = 0;

				// �� ���κ��� �Ľ��ؼ� �и� ������� ���� start �� end �� ���Ѵ�.
				for (auto& line : lines) {
					int hours = 0;
					int minutes = 0;
					int seconds = 0;
					int milliSeconds = 0;
					int intervalSeconds = 0;
					int intervalMilliSeconds = 0;

					stringstream ss(line.substr(11));

					ss >> hours;
					ss.get();
					ss >> minutes;
					ss.get();
					ss >> seconds;
					ss.get();
					ss >> milliSeconds;

					ss >> intervalSeconds;
					if (ss.get() == '.') {
						ss >> intervalMilliSeconds;
					}

					int startMilliSeconds = toMilliSeconds(hours, minutes, seconds - intervalSeconds, milliSeconds - intervalMilliSeconds);
					int endMilliSeconds = toMilliSeconds(hours, minutes, seconds, milliSeconds);

					// �۾� �ð��� �ּ� 1�и��� �̹Ƿ�, 1�ʱ����� ������ �α� ������ ���Եȴ�.
					v.push_back({ startMilliSeconds + 1, 1 });

					// �� ������ �򰥸��°� �� ������ ��ġ�� ó�� ���� �۾����� ġ�� �ʴ´�.
					v.push_back({ endMilliSeconds + 1000, 0 });
				}

				sort(v.begin(), v.end());

				int temp = 0;
				for (auto& [time, isStart] : v) {
					if (isStart == 1) {
						temp++;
					}
					else {
						temp--;
					}

					answer = max(temp, answer);
				}

				return answer;
			}
		}
	}

	namespace blind_2019 {
		// ������
		namespace failure_rate {
			// ������ = �������� ���� and Ŭ���� ���� �÷��̾� �� / �������� ������ �÷��̾� ��
			// �������� �� N, 1 <= N <= 500
			// 1 <= stages.size() <= 200000
			// stages ���� 1�̻� N + 1 ���� �ڿ��� ��� �ִ�. N �� N ��° �������� ���� ��, N + 1�� ���� �� Ŭ������ ���� �ǹ�.
			// ���������� ������ ������ ���� ���, �ش� �������� �������� 0
			// �������� ���� ������������ ���� �������� �������� ��ȣ ��� �ִ� �迭 ����

			struct sf {
				int stage;
				double failureRate;
			};

			vector<int> solution(int N, vector<int> stages) {
				vector<int> answer;

				// �ش� ���������� ���� ���� ����� üũ
				map<int, int> m;
				int wholePlayersCnt = stages.size();

				// �������� �� ����� 0���� ����, �̰� ���صθ� stages ���� ���������� ���� Ȯ�� ó���� �����ȴ�.
				for (int i = 1; i <= N; i++) {
					m[i] = 0;
				}

				for (auto stage : stages) {
					m[stage]++;
				}

				auto myComp = [](sf& a, sf& b) {
					if (a.failureRate == b.failureRate) {
						return a.stage > b.stage;
					}

					return a.failureRate < b.failureRate;
				};

				priority_queue < sf, vector<sf>, decltype(myComp)> pq(myComp);

				for (auto [stage, playersCnt] : m) {
					if (stage != N + 1) {
						if (wholePlayersCnt != 0) {
							pq.push({ stage, ((double)playersCnt / wholePlayersCnt) });
						}
						else {
							// wholePlayersCnt �� 0�� �Ǵ� ���� Ȯ�� 0 ó��
							pq.push({ stage , 0 });
						}
						wholePlayersCnt -= playersCnt;
					}
				}

				while (!pq.empty()) {
					answer.push_back(pq.top().stage);
					pq.pop();
				}

				return answer;
			}
		}

		// ����ä�ù�
		namespace open_chatroom {
			string makeString(const string& command, const string& nickName) {
				string temp = nickName + "���� ";

				if (command == "Enter")
					temp += "���Խ��ϴ�.";
				else {
					temp += "�������ϴ�.";
				}

				return temp;
			}

			// record �� �Ľ��ؼ� ��ɾ �̾Ƽ� �����Ѵ�. �����鼭 ��ɾ �°� uid �� nickname �� �����Ѵ�.
			// uid �� nickname �� �� ���������� ������ �����ߴ� �Ľ��ߴ� ���ڿ��� ���鼭 �� ���� �����Ѵ�.
			vector<string> solution(vector<string> record) {
				vector<vector<string>> parsedStringVectors;
				unordered_map<string, string> uidNickDict;

				for (auto& input : record) {
					stringstream ss(input);
					vector<string> tv;
					string command;
					string uid;
					string nickName;

					ss >> command;
					tv.push_back(command);

					ss >> uid;
					tv.push_back(uid);

					ss >> nickName;

					if (command != "Leave") {
						uidNickDict[uid] = nickName;
					}

					parsedStringVectors.push_back(tv);
				}

				vector<string> answer;
				for (auto& v : parsedStringVectors) {
					if (v[0] != "Change") {
						answer.push_back(makeString(v[0], uidNickDict[v[1]]));
					}
				}

				return answer;
			}
		}

		// �ĺ�Ű
		namespace candidate_key {
			void getMaxCandidateKey(int depth, int& maxKey, const vector<vector<string>>& relation, vector<bool>& chosen) {
				if (depth >= chosen.size()) {
					// chosen �迭�� �� ���õǾ����Ƿ�, �ش� index �� ������ key �� ������ Ȯ���Ѵ�.
					unordered_map<string, int> um;

					for (auto& relationVector : relation) {
						string tempKey = "";
						for (int i = 0; i < chosen.size(); i++) {
							if (chosen[i]) {
								tempKey += relationVector[i];
							}
						}

						if (um.find(tempKey) == um.end()) {
							um[tempKey]++;
						}
						else {
							// ���� ������ Ű�� ���ȴٸ� �ٷ� �Լ� ����
							return;
						}
					}

					// ������� �Դٴ� ���� ������ Ű��� ���̹Ƿ�, maxKey 1 ������Ų �� ����
					maxKey++;
					return;
				}

				for (int i = 0; i < chosen.size(); i++) {
					chosen[i] = true;
					getMaxCandidateKey(i + 1, maxKey, relation, chosen);
					chosen[i] = false;
				}
			}
			// relation �� ��� �ִ� ��� Ʃ���� �����ϰ� �ĺ��� ����
			int solution(vector<vector<string>> relation) {

				int rowSize = relation.size();
				int columnSize = relation[0].size();

				vector<bool> chosen(columnSize, false);

				int answer = 0;
				return answer;
			}
		}

	}
}

#pragma endregion

#pragma region KKO_Intern
namespace kko_tech
{
	namespace intern_2021 {
		namespace one {
			string strArr[] = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
			string intArr[] = { "0", "1","2","3","4","5","6","7","8","9" };
			int solution(string s) {

				while (true) {
					bool isChanged = false;
					for (int i = 0; i < 10; i++) {
						auto pos = s.find(strArr[i]);
						if (pos != std::string::npos) {
							isChanged = true;
							s.replace(s.find(strArr[i]), strArr[i].length(), intArr[i]);
						}
					}

					if (isChanged == false) {
						break;
					}
				}
				return stoi(s);
			}
		}

		namespace two {
			const char person = 'P';
			const char emptyTable = 'O'; // �̰͵� ������ ������ ����� �ִ��� ���ƾ� �Ѵ�.
			const char partition = 'X';

			// ��, ��, ��, ��
			int dr[] = { 0,  1,  0, -1 };
			int dc[] = { 1,  0, -1,  0 };

			bool isValidPos(int r, int c) {
				return r >= 0 && r <= 4 && c >= 0 && c <= 4;
			}

			struct node {
				int r;
				int c;
				int mDistance = 0;
			};


			bool isDistance(int curR, int curC, vector<string>& place) {
				queue<node> q;
				bool visited[5][5]{ false };

				q.push({ curR, curC, 0 });

				while (!q.empty()) {
					const node& n = q.front();
					q.pop();

					// �Ÿ��� 3�̻��̸� Ž���� �ʿ䰡 ����.
					if (n.mDistance >= 3) {
						continue;
					}

					// ������ �湮�ؼ� ó���� �� ������ �н�
					if (visited[n.r][n.c]) {
						continue;
					}

					for (int i = 0; i < 4; i++) {
						int nextR = n.r + dr[i];
						int nextC = n.c + dc[i];
						int nextDistance = n.mDistance + 1;

						// �̻��� ������ ��ǥ�� �н�
						if (isValidPos(nextR, nextC) == false) {
							continue;
						}

						// ���� ��ġ�� ����� ���� ���, ���� ����� �ִٸ� false
						if (place[n.r][n.c] == person) {
							if (place[nextR][nextC] == person) {
								return false;
							}
						}

						// ���� ��ġ�� �� ���̺��� ���� ���, ���̺��� �����¿쿡 2�� �̻� ����� �ִٸ� false
						if (place[n.r][n.c] == emptyTable) {
							int adjacentPersonCnt = 0;
							for (int j = 0; j < 4; j++) {
								int nextR2 = n.r + dr[j];
								int nextC2 = n.c + dc[j];

								// �̻��� ������ ��ǥ�� �н�
								if (isValidPos(nextR2, nextC2) == false) {
									continue;
								}

								if (place[nextR2][nextC2] == person) {
									adjacentPersonCnt++;
								}
							}

							if (adjacentPersonCnt >= 2) {
								return false;
							}
						}

						if (place[nextR][nextC] == person || place[nextR][nextC] == emptyTable) {
							q.push({ nextR, nextC, nextDistance });
						}
					}
				}

				return true;
			}

			vector<int> solution(vector<vector<string>> places) {
				vector<int> answer(5, 1);
				// ���� ��ȣ
				for (int i = 0; i < places.size(); i++) {
					auto& place = places[i];
					bool isDitanceGood = true;

					for (int r = 0; r < 5 && isDitanceGood; r++) {
						for (int c = 0; c < 5; c++) {
							if (!isDistance(r, c, place)) {
								answer[i] = 0;
								isDitanceGood = false;
								break;
							}
						}
					}
				}

				return answer;
			}
		}
	}

	namespace intern_2022
	{
		namespace one
		{
			char criteria[4][2] = { { 'R', 'T' }, { 'C', 'F' }, { 'J', 'M' }, { 'A', 'N' } };
			const int offset = 4;

			string solution(vector<string> survey, vector<int> choices) {
				// survey[i] ���� �� ���ڰ� ����ְ�, 5�̻��� ������ ���ʲ�, 3���ϰ� ������ �����ʲ�, 4�� ������ ������
				// 5�̻���� ���ʰ� 1�� �ؼ� 7������ survey[i] ù��° ���ڿ� ���� �ְ� 3�� ����, 3���Ϻ��� 1������ �ؼ� 1�� ���� 3�� ����

				// survey �� choices �� ���鼭 ������ dict �� ���
				// 1�� ��ǥ���� 4�� ��ǥ���� ������� ���鼭 

				unordered_map<char, int> um;

				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 2; j++) {
						um[criteria[i][j]] = 0;
					}
				}

				for (int i = 0; i < choices.size(); i++) {
					int realScore = choices[i] - offset;
					if (realScore < 0) {
						um[survey[i][0]] += -realScore;
					}
					else if (realScore > 0) {
						um[survey[i][1]] += realScore;
					}
				}

				string answer = "";
				for (int i = 0; i < 4; i++) {
					int gap = um[criteria[i][0]] - um[criteria[i][1]];
					if (gap >= 0) {
						answer += criteria[i][0];
					}
					else {
						answer += criteria[i][1];
					}
				}

				return answer;
			}
		}

		namespace two {

			int solution(vector<int> queue1, vector<int> queue2) {
				// q1 �� q2�� ���� ���ϰ� 2�� ���� ���� m �̶��Ѵ�.
				// q1�� q2�� ������Ҹ� ���� middle �� �� �� �ִ��� Ȯ���Ѵ�.

				// ����� ��.. �߰����� �Ǵ� ���� �Ұ������� �˾Ƴ��� �Ͱ� �������� �˾Ƴ��� 
				int answer = 0;

				long long middle = 0;

				long long q1Sum = 0;
				long long q2Sum = 0;
				int size = queue1.size() + queue2.size();

				queue<int> realQueue1;
				queue<int> realQueue2;


				for (const auto& i : queue1) {
					q1Sum += i;
					realQueue1.push(i);
				}

				for (const auto& i : queue2) {
					q2Sum += i;
					realQueue2.push(i);
				}

				// Ȧ���� �ݹݾ� ���� ���� �� ����.
				if ((q1Sum + q2Sum) % 2 == 1) {
					return -1;
				}

				middle = (q1Sum + q2Sum) / 2;

				while (answer <= 2 * size) {
					if (q1Sum == middle) {
						return answer;
					}

					if (q1Sum > q2Sum) {
						realQueue2.push(realQueue1.front());
						q2Sum += realQueue1.front();
						q1Sum -= realQueue1.front();
						realQueue1.pop();
					}
					else if (q1Sum < q2Sum) {
						realQueue1.push(realQueue2.front());
						q1Sum += realQueue2.front();
						q2Sum -= realQueue2.front();
						realQueue2.pop();
					}

					answer++;
				}

				return -1;
			}
		}

		namespace three {
			// 1�ð� ���η� alp 1 ����
			// 1�ð� ���η� cop 1 ����
			// problem �� alp_req, cop_req, alp_rwd, cop_rwd, cost 5���� ������ �̷����

			const int MAX = 181;
			int cache[MAX][MAX];
			const int inf = 2000000000;
			int targetAlp = -1;
			int targetCop = -1;

			int solution(int alp, int cop, vector<vector<int>> problems) {
				// problems �� alp_req + cop_req -> alp_rwd + cop_rwd 
				int answer = 0;

				// ��ǥġ alp, cop ����

				for (const auto& problem : problems) {
					if (targetAlp < problem[0]) {
						targetAlp = problem[0];
					}

					if (targetCop < problem[1]) {
						targetCop = problem[1];
					}
				}

				targetAlp = max(alp, targetAlp);
				targetCop = max(cop, targetCop);

				for (int i = 0; i < MAX; i++) {
					for (int j = 0; j < MAX; j++) {
						cache[i][j] = inf;
					}
				}

				cache[alp][cop] = 0;

				// cache[a][c] = min( f(a-1,c) + 1, f(a,c-1) + 1, f(a-rwdA, c-rwdC) + reqA + reqC);

				for (int a = alp; a <= targetAlp; a++) {
					for (int c = cop; c <= targetCop; c++) {
						for (const auto& problem : problems) {
							int reqAlp = problem[0];
							int reqCop = problem[1];
							int rwdAlp = problem[2];
							int rwdCop = problem[3];
							int cost = problem[4];

							if (a >= reqAlp && c >= reqCop) {
								int adjustA = min(a + rwdAlp, targetAlp);
								int adjustC = min(c + rwdCop, targetCop);
								cache[adjustA][adjustC] = min(cache[adjustA][adjustC], cache[a][c] + cost);
							}

							{
								int adjustA = min(a + 1, targetAlp);
								int adjustC = min(c + 1, targetCop);
								cache[a][adjustC] = min(cache[a][adjustC], cache[a][c] + 1);
								cache[adjustA][c] = min(cache[adjustA][c], cache[a][c] + 1);
							}
						}
					}
				}

				return cache[targetAlp][targetCop];
			}
		}
	}

	namespace intern_2023 {
		namespace one {
			int solution(int x, int y, int z) {
				return 1;
			}
		}

		namespace two {
			constexpr int limit = 1e9 + 7;
			constexpr int cacheLimit = 1e5 + 1;

			// 2�� 0��, 1�� ... �� limit �� ���� ������
			long long cache[cacheLimit] = { 1, 2 };

			void calcCache(int exponentLimit) {
				for (int i = 1; i < exponentLimit; i++) {
					cache[i + 1] = (cache[i] * 2) % limit;
				}
			}

			int solution(vector<int> cost, int x) {
				long long answer = 0;
				calcCache(cost.size());

				for (int i = cost.size() - 1; i >= 0; i--) {
					if (x >= cost[i]) {
						x = x - cost[i];
						answer = (answer + cache[i]);
					}
				}
				return (int)(answer % limit);
			}
		}

		namespace three {
			int solution(vector<int> box) {
				if (box.size() == 1) {
					return box[0];
				}

				long long dept = 0;
				long long sum = 0;
				long long average = 0;
				vector<int> dif(box.size());
				vector<int> tempBox(box.begin(), box.end());
				long long tempSize = tempBox.size();

				sort(tempBox.begin(), tempBox.end());

				int medium = tempBox[tempBox.size() / 2];
				for (int i = 0; i < tempBox.size(); i++) {
					tempBox[i] = tempBox[i] - medium;
					sum += tempBox[i];
				}

				// �ø��ؼ� ����� ���Ѵ�.
				average = (sum / tempSize) + ((sum % tempSize) == 0 ? 0 : 1) + medium;

				for (int i = 0; i < dif.size(); i++) {
					dif[i] = box[i] - average;
				}

				// �Ųٷ� ��ȸ�ϸ鼭 dept �� ���
				for (int i = dif.size() - 1; i >= 0; i--) {
					dept += dif[i];
					if (dept < 0) {
						dept = 0;
					}
				}

				return average + dept;
			}
		}

		namespace four {

		}

		namespace five {

		}
	}
}
#pragma endregion


#pragma region Baekjoon
namespace BJ {
	namespace DP {
		// �Ǻ���ġ2
		namespace fibonacci2 {
			long long fib[91]{ 0, 1 };

			long long f(int n) {
				if (n == 0) {
					return fib[0];
				}

				if (n == 1) {
					return fib[1];
				}

				if (fib[n - 1] != 0) {
					return fib[n - 1];
				}
				else {
					return fib[n - 1] = f(n - 1) + f(n - 2);
				}
			}

			int main(void) {
				int n;
				cin >> n;

				cout << f(n);

				return 0;
			}
		}

		// 1�� �����
		namespace makeOne {
			const int MAX = 1e6;
			const int INF = 1e9;
			int target;
			int cache[MAX + 1]; // 

			int main(void) {
				for (int i = 0; i <= MAX; i++) {
					cache[i] = INF;
				}
				cache[0] = 0;
				cache[1] = 0;
				cache[2] = 1;
				cache[3] = 1;

				cin >> target;

				for (int i = 1; i <= MAX; i++) {
					if (3 * i <= MAX) {
						cache[3 * i] = min(cache[3 * i], cache[i] + 1);
					}

					if (2 * i <= MAX) {
						cache[2 * i] = min(cache[2 * i], cache[i] + 1);
					}

					if (i + 1 <= MAX) {
						cache[i + 1] = min(cache[i + 1], cache[i] + 1);
					}
				}

				cout << cache[target];

				return 0;
			}
		}
	}
}
#pragma endregion

template<typename c>
void show(const c& container) {
	auto it = begin(container);
	auto endIt = end(container);

	while (it != endIt) {
		cout << *it;
		it++;
	}
}
int main()
{
	//cout << kko_tech::intern_2022::one::solution({ "TR", "RT", "TR" }, { 7,1,3 }) << endl;
	//cout << kko_tech::intern_2022::two::solution({ 1,1 }, { 1,5 });
	//cout << kko_tech::intern_2022::three::solution(0, 0, { {0, 0, 2, 1, 2}, {4, 5, 3, 1, 2}, {4, 11, 4, 0, 2}, {10, 4, 0, 4, 2} });
	//BJ::DP::makeOne::main();
	//vector<int> answer = kko_tech::intern_2021::two::solution({ {"POOOP", "OXXOX", "OPXPX", "OOXOX", "POXXP"}, {"POOPX", "OXPXP", "PXXXO", "OXXXO", "OOOPP"}, {"PXOPX", "OXOXP", "OXPOX", "OXXOP", "PXPOX"}, {"OOOXX", "XOOOX", "OOOXX", "OXOOX", "OOOOO"}, {"PXPXP", "XPXPX", "PXPXP", "XPXPX", "PXPXP"} });
	//string  a = "asd";
	//a = regex_replace(a, regex("asd"), "bcd");

	//show(kko_tech::intern_2023::one::solution({1, 3, 4, 5, 8, 2, 1, 4, 5, 9, 5}, "right"));

	//cout << kko_tech::intern_2023::two_temp::solution({ 19, 78, 27, 18, 20 }, 25);

	//cout << kko_tech::intern_2023::three::solution({ 3, 8, 11, 7 });
	return 0;
}

