#pragma once
#include <stdio.h>
#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
using namespace std;

// �⺻�� ���� ���� ����
// cout �� ���� ��� python �̳� C# ó�� + ������ �۵����� �ʴ´�. << �� �� ���ֵ��� ����.
// index ��� ���� ��� ���� �ð� ������ �� �� �ִ�. Ǯ���� ���ð� ť ������ ������ ��������.

#pragma region hash(map)
// �ؽ� > �������� ���� ����
string solution_h1(vector<string> participant, vector<string> completion) {
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
bool solution_h2(vector<string> phone_book) {
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
int solution_h3(vector<vector<string>> clothes) {
	// �� ���� ���
	int result = 1;
	map<string, int> m;

	for (int i = 0; i < clothes.size(); i++) {
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

vector<int> solution_h4(vector<string> genres, vector<int> plays) {
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
#pragma endregion

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

#pragma region stack_and_queue

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

vector<int> solution_sq_4(vector<int> prices) {
	// ��� ���
	// �ٵ� ���� ó�� Ǭ ���� ������ �ӵ��� �� ���� ���̽��� ����. ���� ��ü�� �޸� ������� ���Ƽ�..
	vector<int> answer(prices.size());
	stack<int> s;

	for (int i = 0; i < prices.size(); i++) {
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

int main()
{
	vector<int> v = { 1,2,3,2,3 };
	vector<int> result = solution_sq_4(v);
	for (int i = 0; i < result.size(); i++) {
		cout << result[i] << endl;
	}
	return 0;
}