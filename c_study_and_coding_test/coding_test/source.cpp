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

	for (auto i = 0; i < clothes.size(); i++) {
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

	for (auto i = 0; i < prices.size(); i++) {
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

		// 비밀지도
		namespace secret_map {

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

			// 다른 좋아 보이는 풀이
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

		// 캐시
		namespace cache {

			//  0 <= cacheSize <= 30
			// cities.size <= 100000 
			// 도시 이름 최대 20자인 영문자, 대소문자 구분 x

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
						// 캐시 여유 공간이 있다면 캐싱
						if (cache.size() < cacheSize) {
							cache[city] = timeElapsed;
						}
						// 캐시 여유 공간이 없다면
						else {
							int lruTime = maxAge;
							string deletedCityName = "";

							// cacheHitTime 이 가장 작은 녀석을 찾아서 삭제, 새로운 내용을 캐싱
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
					// 캐시 hit 되었으면 현재 시간을 입력
					else {
						cache[city] = timeElapsed;
						isCacheHit = true;
					}

					answer += (isCacheHit) ? 1 : 5;

				}

				return answer;
			}
		}

		// 뉴스 클러스터링
		namespace news_clustering {
			// 문자열이 들어오면 공백, 숫자, 특수 문자 외의 숫자가 들어오면 다 없앤다.
			// 다 없앤 문자열을 다 소문자로 바꾼다.
			// 문자열을 2개씩 쪼갠 문자열로 바꾸고, 이를 dict 로 만든다.
			// 만들어진 두 dict 로 min, max 다 구해서 AnB / AuB * 65536 을 리턴

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
				// 문자열을 2개씩 쪼개서 집합으로 만든다.
				// 연속된 문자열이 alpha 여야지만 문자열로 만든다.

				unordered_map<string, int> strDict1;
				unordered_map<string, int> strDict2;

				makeStrDict(strDict1, str1);
				makeStrDict(strDict2, str2);

				int sizeOfInterSection = 0;
				int sizeOfUnion = 0;

				for (auto& [words1, cnt1] : strDict1) {
					// 두 단어 집합에 모두 존재하면
					if (strDict2.find(words1) != strDict2.end()) {
						sizeOfInterSection += min(cnt1, strDict2[words1]);
						sizeOfUnion += max(cnt1, strDict2[words1]);
					}
					// 단어 집합 1에는 있는데 2에는 없으면 1것만 더함
					else {
						sizeOfUnion += cnt1;
					}
				}

				for (auto& [words2, cnt2] : strDict2) {
					// 앞에서 첫 단어 집합 기준으로 하였으므로, 두 번째 단어 집합에만 있는 경우이면
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

		// 다트 게임
		namespace dart_game {
			// S, D, T 는 각각 점수^1, 점수^2, 점수^3
			// S, D, T 는 점수마다 하나씩 존재

			// * 스타상 당첨시 해당 점수와 바로 전에 얻은 점수를 2배
			// # 아차상 당첨 시 해당 점수는 마이너스

			// *는 첫 번째 기회에서도 나올 수 있고, 첫 번째 *의 점수가 2배가 된다.
			// * 효과는 다른 *& 효과와 중첩된다. 이 경우 중첩된 *는 점수가 4배가 된다.
			// *의 효과는 # 효과와 중첩될 수 있고, 이 경우 점수는 -2배가 된다.

			// *, #은 점수마다 둘 중 하나만 존재, 존재하지 않을 수도 있음


			// 숫자가 나오면 push
			// SDT 체크 및 점수 부여
			// 옵션 채크 및 점수 부여 (*나오면 앞 뒤로 2배, #는 해당 내용만 -1 곱하기

			int score[4];

			int solution(string dartResult) {

				int currentIdx = 0;
				int consecutiveDigitCnt = 0;

				// dartResult 에서 숫자로 된 부분만 추출한다.
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

					// 이 문자가 아니면 option 이 아니다.
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

		// 프렌즈4블록
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

			// 기존, 동, 동남, 남 방향으로 체크
			int dr[4] = { 0, 0, 1, 1 };
			int dc[4] = { 0, 1, 1, 0 };
			char erasedChar = '@';

			void checkErased(int curR, int curC, const vector<string>& board, vector<vector<bool>>& outToBeErased, int& outEraseCount)
			{
				char curCharacter = board[curR][curC];

				// 만약 지워진 문자가 들어오면 아무것도 하지 않는다.
				if (curCharacter == erasedChar)
				{
					return;
				}

				// 3 방향을 체크해서 현재 문자와 같으면
				if (curCharacter == board[curR + dr[1]][curC + dc[1]] &&
					curCharacter == board[curR + dr[2]][curC + dc[2]] &&
					curCharacter == board[curR + dr[3]][curC + dc[3]]
					)
				{
					// 4개의 영역을 앞으로 지워질 영역으로 표시, 지워질 갯수 증가
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

				// 이동하게 돼었다면 움직인다.
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
					// 안에서 지워질 것들을 다 체크, 지워질 것이 체크가 안되면 거기서 종료
					// 지워질 것들을 지운다. 지워진 갯수를 answer 에 더한다.
					// 문자 배열을 당긴다.

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

					// 지울 거 지우고
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

					// board 를 당긴다.
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

		// 셔틀버스
		namespace shuttle_bus {

			// 셔틀이 도착했을 때에 도착한 사람도 태운다.
			// 셔틀은 09:00 부터 n 회 t 분 간격으로 운행
			// 셔틀 운행 횟수 1 <= n <= 10
			// 셔틀 운행 간격 1 <= t <= 60
			// 셔틀 최대 인원 수 1 <= m <= 45
			// 23:59 에는 모두 집에 돌아가기에, 다음날 셔틀을 타는 일은 없음. 최대 늦은 시간에 타도 23:59임
			// 콘은 무조건 자기가 도착한 시간대에서 맨 마지막에 선다.

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
							// 버스 도착 시간보다 적으면 계속 탑승
							lastRidingCrewIdx++;
							firstWaitingCrewIdx++;

							// 도착한 버스에 사람이 더 이상 못탄다고 체크
							if (j == m - 1) {
								isLastBusAvailable = false;
							}

							// 마지막으로 탑승한 버스 탑승 시간 기록
							lastRidedBusIdx = i;

							// 태우던 도중에 전체 인원들 다 태웠다면 중단한다.
							if (lastRidingCrewIdx == totalCrewCnt - 1) {
								break;
							}
						} // if
					} // for
				} // for

				// 아래 부분 나중에 보게 되면 한 번 더 잘 기억해두자.
				// 마지막 버스가 꽉 찬 상황이 아니라면 마지막 버스가 온 시간에 타는 것이 가장 늦게 출근하는 시간인 것이다.

				// 마지막 버스가 꽉 찼다면
				if (lastRidedBusIdx == lastBusIdx && !isLastBusAvailable) {
					return oneMinuteBeforeTime(timetable[lastRidingCrewIdx]);
				}
				else {
					return busArrivedTimes[lastBusIdx];
				}
			}
		}

		// 추석 트래픽
		namespace chuseok_traffic {
			// TODO: 어려움, 다른 코테 볼 일 있으면 한 번 더 확인 필요
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

				// 각 라인별로 파싱해서 밀리 세컨드로 만들어서 start 와 end 를 구한다.
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

					// 작업 시간이 최소 1밀리초 이므로, 1초까지는 무조건 로그 구간에 포함된다.
					v.push_back({ startMilliSeconds + 1, 1 });

					// 이 문제가 헷갈리는게 딱 끝점에 걸치면 처리 중인 작업으로 치지 않는다.
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
		// 실패율
		namespace failure_rate {
			// 실패율 = 스테이지 도달 and 클리어 못한 플레이어 수 / 스테이지 도달한 플레이어 수
			// 스테이지 수 N, 1 <= N <= 500
			// 1 <= stages.size() <= 200000
			// stages 에는 1이상 N + 1 이하 자연수 들어 있다. N 은 N 번째 스테이지 도전 중, N + 1은 전부 다 클리어한 것을 의미.
			// 스테이지에 도달한 유저가 없는 경우, 해당 스테이지 실패율은 0
			// 실패율이 높은 스테이지부터 내림 차순으로 스테이지 번호 담겨 있는 배열 리턴

			struct sf {
				int stage;
				double failureRate;
			};

			vector<int> solution(int N, vector<int> stages) {
				vector<int> answer;

				// 해당 스테이지에 도전 중인 명수를 체크
				map<int, int> m;
				int wholePlayersCnt = stages.size();

				// 스테이지 별 대기자 0으로 세팅, 이거 안해두면 stages 없는 스테이지에 대한 확률 처리가 누락된다.
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
							// wholePlayersCnt 가 0이 되는 경우는 확률 0 처리
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

		// 오픈채팅방
		namespace open_chatroom {
			string makeString(const string& command, const string& nickName) {
				string temp = nickName + "님이 ";

				if (command == "Enter")
					temp += "들어왔습니다.";
				else {
					temp += "나갔습니다.";
				}

				return temp;
			}

			// record 를 파싱해서 명령어를 뽑아서 저장한다. 뽑으면서 명령어에 맞게 uid 별 nickname 을 저장한다.
			// uid 별 nickname 이 다 정해졌으면 위에서 저장했던 파싱했던 문자열들 돌면서 답 만들어서 리턴한다.
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

		// 후보키
		namespace candidate_key {
			void getMaxCandidateKey(int depth, int& maxKey, const vector<vector<string>>& relation, vector<bool>& chosen) {
				if (depth >= chosen.size()) {
					// chosen 배열이 다 선택되었으므로, 해당 index 가 유일한 key 를 갖는지 확인한다.
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
							// 만약 유일한 키가 못된다면 바로 함수 종료
							return;
						}
					}

					// 여기까지 왔다는 것은 유일한 키라는 뜻이므로, maxKey 1 증가시킨 후 리턴
					maxKey++;
					return;
				}

				for (int i = 0; i < chosen.size(); i++) {
					chosen[i] = true;
					getMaxCandidateKey(i + 1, maxKey, relation, chosen);
					chosen[i] = false;
				}
			}
			// relation 에 들어 있는 모든 튜플은 유일하게 식별이 가능
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


int main()
{
	return 0;
}

