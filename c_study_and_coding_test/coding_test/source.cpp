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

// 기본적 참고 사항 사항
// cout 을 쓰는 경우 python 이나 C# 처럼 + 연산이 작동되지 않는다. << 를 다 써주도록 하자.
// index 라는 것은 어떻게 보면 시간 순서도 될 수 있다. 풀었던 스택과 큐 문제의 내용을 생각하자.

// priority_queue<int, vector<int>, greater<int>> pq; <- 이렇게 하면 작은 녀석부터 pq.top() 에서 나온다. 반대로 하려면 less<int> 넣자.
// 헷갈리는 것이 vector 를 sort 할 때에 작은 것 -> 큰 것으로 하려면 less<int> 를 넣어야 한다는 것이다. 
// sort 는 true 면 안바꾸고, heap 을 쓰는 pq는 true 면 위치 변환을 하기 때문에 이렇게 된다.

// multiset 의 경우 minmax 를 구할 때의 용도로 활용될 수 있다.
// substr 은 처음에 변수가 offset 위치고, 그 다음은 count 이다. a.substr(1,3) 이러면 1번째 index 를 포함해서 3개의 문자를 가지는 string 을 리턴한다.

// 함수 내부에서 반복적으로 생성될 필요 없는 전역변수는 바깥으로 빼주자.

// 최대 크기나 최소 크기만 알면 된다면 max_element, min_element를 쓰자. sort하는 것보다 연산량을 많이 줄일 수 있다.
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
// 해시 > 완주하지 못한 선수
string solution_hash_1(vector<string> participant, vector<string> completion) {
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
bool solution_hash_2(vector<string> phone_book) {
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
int solution_hash_3(vector<vector<string>> clothes) {
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

// 해시 > 베스트앨범
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

// 스택/큐 > 기능개발
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

// 스택/큐 > 프린터
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

// 스택/큐 > 다리를 지나는 트럭
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

// 스택/큐 > 주식가격
vector<int> solution_sq_4(vector<int> prices) {
	// 모범 답안
	// 근데 내가 처음 푼 이중 포문이 속도가 더 빠른 케이스가 많다. 스택 자체가 메모리 입출력이 많아서..
	vector<int> answer(prices.size());
	stack<int> s;

	for (int i = 0; i < prices.size(); i++) {
		// stack 에서 작은 값이 나올때까지만 pop을 해야한다.
		while (!s.empty() && prices[s.top()] > prices[i]) {
			// 스택의 맨 위에 있는 index 는 도착 시간과 같은 개념이다. 
			// 현재 index - stack top 이 경과한 시간이 되는 것이다.
			answer[s.top()] = i - s.top();
			s.pop(); // 현재 비교대상보다 작거나 같은 값이 나올 떄까지만 pop 한다.
		}
		s.push(i);
	}

	while (!s.empty()) { // 계속 스택에 쌓여 있는 것이 있다면 위와 같은 식으로 pop 하며 할당해준다.
		answer[s.top()] = prices.size() - 1 - s.top();
		s.pop();
	}

	return answer;

	// 내가 푼 답안
	//vector<int> answer;
	//int checked;
	//for (int i = 0; i < prices.size(); i++) {
	//	checked = 0;
	//	for (int j = i + 1; j < prices.size(); j++) {
	//		if (prices[i] <= prices[j])
	//			checked++;
	//		else { // 이 부분이 너무 헷갈리는데, 가격이 바로 떨어졌는데 1초간 떨어지지 않은 것으로 간주한다..
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
// 힙 > 더 맵게
int solution_heap_1(vector<int> scoville, int K) {
	// 내가 푼 답안, pq 를 좀 더 스마트하게 넣을 수 있다. 내일 그 내용 추가하자.
	int answer = 0;
	bool isAllOverK = false;
	priority_queue<int, vector<int>, greater<int>> pq(scoville.begin(), scoville.end()); // 이렇게 초기화가 가능하다.

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

// 힙 > 디스크 컨트롤러
struct solution_heap_2_sort {
	bool operator()(vector<int>& a, vector<int>& b) {
		if (a[1] > b[1]) // 요청 시간이 더 작은 녀석이 앞에 오게 한다.
			return true;
		if (a[1] == b[1]) {
			return a[0] > b[0]; // 도착한 시간이 더 작은(빠른) 녀석이 앞에 오게 한다.
		}

		return false;
	}
};
int solution_heap_2(vector<vector<int>> jobs) {
	int answer = 0;
	int times = 0;
	vector<int> top(2);
	priority_queue<vector<int>, vector<vector<int>>, solution_heap_2_sort> pq(jobs.begin(), jobs.end());
	priority_queue<vector<int>, vector<vector<int>>, solution_heap_2_sort> pq2; // 밀린 작업들을 잠시 넣어주는 queue

	while (true) {
		if (!pq.empty()) {
			top = pq.top();
			pq.pop();
			if (top[0] <= times) { // 현재 흐른 시간이 도착한 시간보다 크면 처리한다.
				times += top[1]; // 처리하는데 걸린 시간만큼 times 를 더한다.
				answer += times - top[0];
				if (!pq2.empty()) {
					while (!pq2.empty()) // 처리하고 나면 밀린 작업들을 다시 큐에 넣어준다.
					{
						pq.push(pq2.top());
						pq2.pop();
					}
				}
			}
			else {
				pq2.push(top); // 아직 시간 도착이 안되서 밀린 작업들을 벡터에 넣어준다.
			}
		}

		if (pq.empty() && !pq2.empty()) { // pq 에 모든 queue 가 비었는데 pq2 가 안비었다는 것은 시간이 부족해서 작업이 쌓였다는 것이다.
			// 이럴 때에는 시간을 증가시킨다.
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

// 힙 > 이중우선순위큐
vector<int> solution_heap_3(vector<string> operations) {
	vector<int> answer;
	int operationCount = operations.size();
	multiset<int> mset; // min max 값을 구할 때에는 multiset 을 활용하면 좋다는 것을 알자.

	for (int i = 0; i < operationCount; i++) {
		if (operations[i][0] == 'I') { // insert 인 경우이므로 mset 에 삽입한다.
			mset.insert(atoi(&operations[i][2]));
		}
		else {
			if (mset.size() >= 1) {
				if (operations[i][2] == '1') { // 최대값을 삭제하는 경우
					mset.erase(--mset.end());
				}
				else { // 최소값을 삭제하는 경우
					mset.erase(mset.begin());
				}
			}
		}
	}

	if (mset.size() == 0)
		return { 0,0 };

	answer.push_back(*(--mset.end())); // 최대값 삽입
	answer.push_back(*mset.begin()); // 최소값 삽입
	return answer;
}
#pragma endregion

#pragma region sort

// 정렬 > K번째수
vector<int> solution_sort_1(vector<int> array, vector<vector<int>> commands) {
	vector<int> answer;

	for (int i = 0; i < commands.size(); i++) {
		vector<int> temp(array.begin() + commands[i][0]-1, array.begin() + commands[i][1]);
		sort(temp.begin(), temp.end());
		answer.push_back(temp[commands[i][2]-1]);
	}
	
	return answer;
}


// 정렬 > 가장 큰 수
bool solution_sort_2_comp(int a, int b) {
	// 모범 답안
	string a_str = to_string(a);
	string b_str = to_string(b);

	return a_str + b_str > b_str + a_str; // 대박.. 잊지 말자.
	
	// 아래는 이전에 풀었던 답
	// a와 b 중 앞 자리가 더 크면 그 놈이 더 큰 것
	// 만약 a와 b가 자리 숫자가 다르면, 짧은 것을 기준으로 맞춰서 
	//string a_str = to_string(a);
	//string b_str = to_string(b);
	//int a_str_size = a_str.size();
	//int b_str_size = b_str.size();

	//if (a_str_size > b_str_size) { // a 가 자릿수가 더 많은 경우
	//	int i = 0;
	//	for (;i < b_str_size; i++) {
	//		if (a_str[i] > b_str[i]) return true; // a 가 더 크면 true
	//		else if (a_str[i] < b_str[i]) return false; // b 가 더 크면 false
	//	}

	//	for (int j = i; j < a_str_size; j++) {
	//		if (a_str[j] > b_str[i-1]) return true;
	//		else if (a_str[j] < b_str[i-1]) return false;
	//	}

	//	return false;
	//}
	//else { // b 가 자릿수가 더 많거나 둘이 같은 경우
	//	int i = 0;
	//	for (; i < a_str_size; i++) {
	//		if (a_str[i] > b_str[i]) return true; // a 가 더 크면 true
	//		else if (a_str[i] < b_str[i]) return false; // b 가 더 크면 false
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

	//if (atoi(answer.c_str()) == 0) // 프로그래머스에서 stoi 가 안된다.
	//	answer = "0";

	return answer;
}

// 정렬 > H-Index
int solution_sort_3(vector<int> citations) {
	int answer = 0;
	sort(citations.begin(), citations.end(), greater<int>());

	for (int i = 0; i < citations.size(); i++) {
		answer++;
		// 6,5,3,1,0 인 경우 answer 의 값 이하의 숫자가 나오는 순간의 처음 answer 를 리턴하면 된다.
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
	// 모범 답안
	vector<int> ret;
	vector<int> supos = { 0,0,0 };

	for (int i = 0; i < answers.size(); i++) {
		if (supo_answers[0][i % supo_answers[0].size()] == answers[i]) supos[0]++;
		if (supo_answers[1][i % supo_answers[1].size()] == answers[i]) supos[1]++;
		if (supo_answers[2][i % supo_answers[2].size()] == answers[i]) supos[2]++;
	}

	auto it = max_element(supos.begin(), supos.end()); // 최대 크기만 알고 있으면 된다면, max_element 한 번만 쓰고 이후에는 for문으로 비교하면 된다.
	int max = *it;

	for (int i = 0; i < supos.size(); i++) {
		if (supos[i] == max)
			ret.push_back(i + 1);
	}

	return ret;
	// 아래가 이전에 내가 했던 답안
	//vector<int> answer;
	//vector<pair<int, int>> supo_score = { {0,0},{1,0},{2,0} };
	//vector<int> supo1_answer = { 1, 2, 3, 4, 5 };
	//vector<int> supo2_answer = { 2, 1, 2, 3, 2, 4, 2, 5 };
	//vector<int> supo3_answer = { 3, 3, 1, 1, 2, 2, 4, 4, 5, 5 };


	//for (int i = 0; i < answers.size(); i++) {
	//	// 수포1
	//	if (supo1_answer[i % supo1_answer.size()] == answers[i])
	//		supo_score[0].second++;

	//	// 수포2
	//	if (supo2_answer[i % supo2_answer.size()] == answers[i])
	//		supo_score[1].second++;

	//	// 수포3
	//	if (supo3_answer[i % supo3_answer.size()] == answers[i])
	//		supo_score[2].second++;
	//}

	//sort(supo_score.begin(), supo_score.end(), my_comp_brute_force_1());

	//answer.push_back(supo_score[0].first + 1);

	//for (int i = 1; i < supo_score.size(); i++) {
	//	if (supo_score[0].second == supo_score[i].second) // 최대값과 그 다음 요소가 같으면 추가
	//		answer.push_back(supo_score[i].first + 1);
	//	else // 없으면 뒤를 더 볼 필요가 없으므로 break
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
