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
using namespace std;

// 기본적 참고 사항 사항
// cout 을 쓰는 경우 python 이나 C# 처럼 + 연산이 작동되지 않는다. << 를 다 써주도록 하자.
// index 라는 것은 어떻게 보면 시간 순서도 될 수 있다. 풀었던 스택과 큐 문제의 내용을 생각하자.
// 어떠한 정확한 정보를 구하는 것이 아니라, 갯수, 길이만 구하는 것이면 훨씬 계산이 단순화될 수 있다. 명심!

// priority_queue<int, vector<int>, greater<int>> pq; <- 이렇게 하면 작은 녀석부터 pq.top() 에서 나온다. 반대로 하려면 less<int> 넣자.
// 헷갈리는 것이 vector 를 sort 할 때에 작은 것 -> 큰 것으로 하려면 less<int> 를 넣어야 한다는 것이다. 
// sort 는 true 면 안바꾸고, heap 을 쓰는 pq는 true 면 위치 변환을 하기 때문에 이렇게 된다.

// multiset 의 경우 minmax 를 구할 때의 용도로 활용될 수 있다.
// substr 은 처음에 변수가 offset 위치고, 그 다음은 count 이다. a.substr(1,3) 이러면 1번째 index 를 포함해서 3개의 문자를 가지는 string 을 리턴한다.

// 함수 내부에서 반복적으로 생성될 필요 없는 전역변수는 바깥으로 빼주자.

// 최대 크기나 최소 크기만 알면 된다면 max_element, min_element를 쓰자. sort 하는 것보다 연산량을 많이 줄일 수 있다.
// STL 컨테이너 간에는 상호 변환이 가능한데, 이러한 경우 생성자에서 바로 컨테이너 간에 변환이 이루어지도록 하자. ex) unordered_set<int> us(v.begin(), v.end());

// DFS 는 스택, BFS 는 queue 를 통해서 구현할 수 있다.

// vector 는 () 으로 크기를 지정하여 생성하면 기본 인자들이 전부 다 0으로 초기화된다. vector<pair<int,int>> 이렇게 하면 pair 가 각각 다 0으로 초기화된다.

// map<int,int> m 이렇게 하면 m[10] = 10 이런 식으로 추가가 가능하고, auto it = m.begin() 으로 할당하면 pair<int, int> 형식을 가리키는 it가 된다.
// 사용할 때에는 (*it).first, (*it).second 이런 식으로 사용하면 된다.


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

// 해시 > 폰켓몬
using namespace std;

int solution_hash_5(vector<int> nums)
{
	unordered_set<int> us(nums.begin(), nums.end());
	return min(nums.size() / 2, us.size());
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
		vector<int> temp(array.begin() + commands[i][0] - 1, array.begin() + commands[i][1]);
		sort(temp.begin(), temp.end());
		answer.push_back(temp[commands[i][2] - 1]);
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


#pragma region DFS_BFS

int answer_dfs_bfs_1;
void DFS_1(vector<int>& numbers, int depth, int sum, int target) {
	if (sum == target && depth == numbers.size() - 1) // 목표 찾았으므로 답 ++
	{
		answer_dfs_bfs_1++;
		return;
	}

	if (depth == numbers.size() - 1) // depth 끝에 도달했으므로 ++
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
	// 재귀말고 stack 으로 인한 풀이
	stack<int> s;
	s.push(start);

	while (!s.empty()) {
		int from = s.top();
		s.pop();
		for (int i = 0; i < visited.size(); i++) {
			if ((visited[i] == false && computers[from][i])) { // 현재 시작 점에서 방문하려고 하는 곳이 방문 가능하고, 방문한 적이 없으면
				visited[i] = true; // 해당 장소를 방문한다.
				s.push(i); // 방문 가능한 노드를 다시 푸쉬한다.
			}
		}
	}

	// 아래는 재귀로 인한 풀이
	//for (int i = 0; i < visited.size(); i++) {
	//	if ( (visited[i] == false && computers[start][i])) { // 현재 시작 점에서 방문하려고 하는 곳이 방문 가능하고, 방문한 적이 없으면
	//		visited[i] = true; // 해당 장소를 방문한다.
	//		DFS_2(visited, computers, i); // 방문한 장소에서 다시 DFS를 실행한다.
	//	}
	//}
}

// 깊이/너비 우선 탐색 > 네트워크
int solution_dfs_bfs_2(int n, vector<vector<int>> computers) {
	int answer = 0;
	vector<bool> visited(n); // vector 는 초기값이 다 0으로 초기화된다.

	// i 부터 시작해서 깊이 탐색을 시작하며 지나간 곳은 다 visted를 true로 바꾼다.
	for (int i = 0; i < computers.size(); i++) {
		if (!visited[i]) {// 아직 이 장소를 방문한 적이 없으면
			visited[i] = true;
			DFS_2(visited, computers, i);
			answer++; // DFS가 끝나면 하나의 네트워크가 완성되므로 네트워크 한 개 추가.
		}
	}

	return answer;

	// 아래는 재귀로 인한 풀이
	//int answer = 0;
	//vector<bool> visited(n); // vector 는 초기값이 다 0으로 초기화된다.

	//// i 부터 시작해서 깊이 탐색을 시작하며 지나간 곳은 다 visted를 true로 바꾼다.
	//for (int i = 0; i < computers.size(); i++) {
	//	if (!visited[i]) {// 아직 이 장소를 방문한 적이 없으면
	//		visited[i] = true;
	//		DFS_2(visited, computers, i);
	//		answer++; // DFS가 끝나면 하나의 네트워크가 완성되므로 네트워크 한 개 추가.
	//	}
	//}

	//return answer;
}
#pragma endregion


#pragma region Greedy


// 탐욕법 > 큰 수 만들기
string solution_greedy_2(string number, int k) {
	string answer = "";
	int targetLength = number.size() - k;
	char max;
	int maxIndex;
	// 앞에서 k 만큼 확인하면서, 뽑을 수 있는 가장 큰수부터 answer에 넣고, k를 감소시킨다.
	// 시작점부터 시작점+selectedNumber 가 문자열 끝에 다다르면 모두 다 뽑는다.

	for (int i = 0; i < number.size(); i++) {
		max = 0; // max값 초기화
		maxIndex = -1; // maxIndex 초기화, -1이면 찾지 못했다는 뜻이다.
		for (int j = i; j <= i + k; j++) { // 가장 먼저 나온 최대 값의 위치를 찾는 것
			if (max < number[j]) {
				max = number[j];
				maxIndex = j;
			}
		}
		k -= (maxIndex - i); // 문자를 제거 한 만큼 k를 감소
		i = maxIndex; // 다음 문자열 검사는 현재 answer에 추가된 다음 항목부터 검사한다.
		answer += max;
		if (answer.size() == targetLength) // 목표 길이에 도달하면 문자열을 리턴
			return answer;
	}

	return answer;
}
#pragma endregion


#pragma region Dynamic Programming


// 동적계획법 > N으로 표현
int solution_dynamic_1(int N, int number) {
	vector<set<int>> entries(9);
	entries[1].insert(N);
	for (int i = 1; i < 9; i++) { // i 개의 N 을 사용하는 경우
		for (int j = 1; j <= i; j++) { // j + i-j 로 쪼개진 경우의 수를 모두 구하기 위한 것
			for (auto it1 = entries[j].begin(); it1 != entries[j].end(); it1++) {
				for (auto it2 = entries[i - j].begin(); it2 != entries[i - j].end(); it2++) {
					// 덧셈
					entries[i].insert(*it1 + *it2);

					// 곱셈
					entries[i].insert(*it1 * *it2);

					// 뺄셈
					entries[i].insert(*it1 - *it2);
					entries[i].insert(*it2 - *it1);

					// 나눗셈
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

	for (int i = 1; i < 9; i++) { // N이 연속해서 나오는 경우
		auto it = entries[i].find(number);

		if (it != entries[i].end())
			return i;
	}

	return -1;
}

#pragma endregion


#pragma region Graph

// 그래프 > 가장 먼 노드
bool isPath[20001][20001];

int solution_graph_1(int n, vector<vector<int>> edge) {
	// 1번 노드에서 가장 멀리 떨어진 노드의 갯수를 구한다.
	// input: 간선에 대한 정보가 담긴 2차원 배열 edge, 노드 개수 n
	// 2 <= n <= 20000
	// 간선은 양방향
	// 1 <= edge.size() <= 50000


	// 1번 노드부터 시작
	// 1번 노드에서 갈 수 있는 곳 모두 방문 체크, 거리 1로 할당
	// 방문한 노드들 큐에 집어넣고 다시 거기서 방문할 수 있는 노드들 방문 거리 2로 할당. (이미 queue에 들어갔으면 방문 x)
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
		while (!readyQ.empty()) { // ready queue 에 있는 것 process queue 로 옮긴다.
			processQ.push(readyQ.front());
			readyQ.pop();
		}
		level++;
		while (!processQ.empty()) {
			int front = processQ.front();
			processQ.pop();

			for (int i = 0; i < n; i++) {
				if ((isPath[front][i] || isPath[i][front]) && !checked[i]) { // 갈 수 있으면서 아직 체크 안된 경우일 때에
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
		// [1차] 비밀지도

		// 내 풀이
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

		vector<string> solution1(int n, vector<int> arr1, vector<int> arr2) {
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

		// 다른 좋아 보이는 풀이
		vector<string> solution1_other(int n, vector<int> arr1, vector<int> arr2) {
			vector<int> original(arr1.size());
			vector<string> answer;

			for (int i = 0; i < original.size(); i++) {
				original[i] = arr1[i] | arr2[i];

				string temp = "";
				for (int j = 0; j < n; j++) {
					if (original[i] % 2 == 1) {
						temp = '#' + temp ;
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

}
#pragma endregion

int main()
{
	cout << kko::blind_2018::decode(5, 9) << endl;
	return 0;
}
