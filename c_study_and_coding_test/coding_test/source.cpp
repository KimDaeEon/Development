#pragma once
#include <stdio.h>
#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;

// 기본적 주의 사항
// cout 을 쓰는 경우 python 이나 C# 처럼 + 연산이 작동되지 않는다. << 를 다 써주도록 하자.

#pragma region hash(map)
// 해시 > 완주하지 못한 선수
string solution_h1(vector<string> participant, vector<string> completion) {
	// 아래는 모범 답안
	sort(participant.begin(), participant.end());
	sort(completion.begin(), completion.end());

	for (int i = 0; i < completion.size(); i++) {
		if (participant[i] != completion[i])
			return participant[i];
	}

	return participant[participant.size() - 1];


	// 아래는 내가 처음에 풀었던 것
	//map<string, int> m;
	//for (auto it = completion.begin(); it != completion.end(); it++) {
	//    if (m.find(*it) == m.end())// 처음 나오는 요소면 삽입
	//        m.insert(pair<string, int>(*it, 1)); // 항목을 찾을 맵 생성
	//    else {  // 중복 요소인 경우 ++ 한다.
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

// 해시 > 전화번호 목록
bool solution_h2(vector<string> phone_book) {
	// 모범 답안
	sort(phone_book.begin(), phone_book.end());

	for (int i = 0; i < phone_book.size() - 1; i++) {
		if (phone_book[i] == phone_book[i + 1].substr(0, phone_book[i].size())) {
			return false;
		}
	}

	return true;
}
// 내가 한 것
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

// 해시 > 위장
int solution_h3(vector<vector<string>> clothes) {
	// 더 나은 답안
	int result = 1;
	map<string, int> m;

	for (int i = 0; i < clothes.size(); i++) {
		m[clothes[i][1]]++; // 아무것도 없어도 요렇게가 된다. 다음 코테 문제 풀 때에 참고.
	}
	for (auto it = m.begin(); it != m.end(); it++) {
		result *= (it->second + 1);
	}
	return result - 1;

	// 아래는 내가 푼 답안.
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
	if (a.second > b.second) // 플레이 수가 더 많다면 앞에 와야한다.
		return true;
	if (a.second == b.second) { // 플레이 수가 같다면
		if (a.first < b.first) { // 플레이 수가 같다면 index 가 더 작으면 앞에 와야 한다.
			return true;
		}
	}

	return false; // a 의 플레이 수가 더 작거나, 플레이 수가 같은데 인덱스가 크지 않은 경우
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
	// 모범 답안
	vector<int> answer;
	int daysAfter = 0;
	int daysAfterMax = 0; // 지난 날이 최대값이 된 경우에 바꾼다.

	for (int i = 0; i < progresses.size(); i++) {
		// 이 부분도 중요하다. 100이라는 완성치가 정해져 있으므로 while 돌릴 필요 없이 이렇게 반복 횟수를 구할 수 있다.
		daysAfter = (99 - progresses[i]) / speeds[i] + 1;

		if (daysAfter > daysAfterMax) {
			answer.push_back(1);
			daysAfterMax = daysAfter;
		}
		else {
			++answer.back(); // 이게 중요하다. 사실상 이것으로 인해 vector 가 stack 과 queue 로서 모두 쓰일 수 있다.
		}
	}

	return answer;
	// 아래는 내가 풀었던 답
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
		pq.push(priorities[i]); // 우선 순위 판단을 위해 pq 에 삽입
		entryQueue.push(pair<int, int>(i, priorities[i])); // 뒤섞으면서 나오게 하기 위해서..
	}

	while (true) { // location 이 pop 될 때까지 계속 돌린다.
		currentMax = pq.top();
		pq.pop();

		while (true) { // pq 와 같은 하나의 원소가 pop 될 때까지 실행.
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
			else { // 아직 처리가 불가능하면 다시 푸쉬한다.
				entryQueue.push(pair<int, int>(checkedIdx, checkedPriority));
			}
		}
	}

	return answer;
}

int solution_sq_3(int bridge_length, int weight, vector<int> truck_weights) {
	// 매 순간 내리는지를 체크해야 한다.
	// 맨 앞에 있는 놈이 다 건너면(bridge_length +1 후에) 현재 총 무게에서 건넌 놈 무게를 빼고 더 트럭을 올릴 수 있는지 봐야 한다.
	int answer = 0;
	int currentTotalWeight = 0;
	int index = 0;
	int end = truck_weights.size();
	queue<pair<int, int>> passing;

	while (true) {
		answer++; // 시간 1초 경과

		if (!passing.empty()) {
			if ((answer - passing.front().first) >= bridge_length) { // 경과한 시간 - 시작 시간 >= 다리 길이 이면 건넌 것이다.
				currentTotalWeight -= passing.front().second; // 차량의 무게를 총 합 무게에서 빼준다.
				passing.pop(); // 맨 앞에 있는 차가 빠질 때가 되었으므로 pop 한다.

				if (index == end && passing.empty()) // pop 될 때에 마지막이면 break
					break;
			}
		}

		if (index < end && currentTotalWeight + truck_weights[index] <= weight) {
			currentTotalWeight += truck_weights[index];
			passing.push(pair<int, int>(answer, truck_weights[index])); // 이 차량이 올라온 시간과 무게를 기록
			index++; // 다음 트럭이 들어갈 수 있는지 체크.
		}
	}

	return answer;
}

vector<int> solution_sq_4(vector<int> prices) {
	// 후우 이 문제 설명이 너무 그렇다.. 나중에 모범 답안에서 스택으로 인덱스 다루는 것은 보되 문제에 너무 집착하지 말자.
	vector<int> answer;
	int checked;
	for (int i = 0; i < prices.size(); i++) {
		checked = 0;
		for (int j = i + 1; j < prices.size(); j++) {
			if (prices[i] <= prices[j])
				checked++;
			else { // 이 부분이 너무 헷갈리는데, 가격이 바로 떨어졌는데 1초간 떨어지지 않은 것으로 간주한다..
				checked++;
				break;
			}
		}
		answer.push_back(checked);
	}
	return answer;
}

#pragma endregion

int main()
{
	vector<int> temp = { 1,1,1 };
	int x = 10;
	int size = temp.size();
	cout << min(1 + x, 2 + (const int)temp.size()) << endl;
	return 0;
}
