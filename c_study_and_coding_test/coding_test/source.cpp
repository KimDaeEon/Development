#pragma once
#include <stdio.h>
#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;

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
        if (phone_book[i] == phone_book[i+1].substr(0,phone_book[i].size())) {
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
    return result -1;

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


int main()
{
    // �Ʒ��� ���� ��¿�
    /*vector<string> tv = { "119", "976112", "1195524" };
    sort(tv.begin(), tv.end());

    for (auto it = tv.begin(); it != tv.end(); it++) {
        cout << *it << endl;
    }*/
    
    return 0;
}
