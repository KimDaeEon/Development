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
using namespace std;

// �⺻�� ���� ���� ����
// cout �� ���� ��� python �̳� C# ó�� + ������ �۵����� �ʴ´�. << �� �� ���ֵ��� ����.
// index ��� ���� ��� ���� �ð� ������ �� �� �ִ�. Ǯ���� ���ð� ť ������ ������ ��������.

// priority_queue<int, vector<int>, greater<int>> pq; <- �̷��� �ϸ� ���� �༮���� pq.top() ���� ���´�. �ݴ�� �Ϸ��� less<int> ����.
// �򰥸��� ���� vector �� sort �� ���� ���� �� -> ū ������ �Ϸ��� less<int> �� �־�� �Ѵٴ� ���̴�. 
// sort �� true �� �ȹٲٰ�, heap �� ���� pq�� true �� ��ġ ��ȯ�� �ϱ� ������ �̷��� �ȴ�.

// multiset �� ��� minmax �� ���� ���� �뵵�� Ȱ��� �� �ִ�.
// substr �� ó���� ������ offset ��ġ��, �� ������ count �̴�. a.substr(1,3) �̷��� 1��° index �� �����ؼ� 3���� ���ڸ� ������ string �� �����Ѵ�.

// �Լ� ���ο��� �ݺ������� ������ �ʿ� ���� ���������� �ٱ����� ������.

// �ִ� ũ�⳪ �ּ� ũ�⸸ �˸� �ȴٸ� max_element, min_element�� ����. sort�ϴ� �ͺ��� ���귮�� ���� ���� �� �ִ�.
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
		vector<int> temp(array.begin() + commands[i][0]-1, array.begin() + commands[i][1]);
		sort(temp.begin(), temp.end());
		answer.push_back(temp[commands[i][2]-1]);
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
		if(answer >= citations[i])
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
int main()
{
	vector<int> in = { 1,2,3,4,5};
	solution_brute_force_1(in);
	return 0;
}
