﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// 놀랍게도 C# 에는 PriorityQueue 가 없는 것 같다..
public class PriorityQueue<T> where T : IComparable<T>
{
    List<T> _heap = new List<T>();

    public int Count { get { return _heap.Count; } }

    // O(logN)
    public void Push(T data)
    {
        // 힙의 맨 끝에 새로운 데이터를 삽입
        _heap.Add(data);

        int now = _heap.Count - 1;
        // sort 시작
        while (now > 0)
        {
            int next = (now - 1) / 2;
            // 현재의 위치에서 바뀔 필요가 없음.
            if (_heap[now].CompareTo(_heap[next]) < 0)
            {
                break;
            }

            // 두 값을 교체
            T temp = _heap[now];
            _heap[now] = _heap[next];
            _heap[next] = temp;

            // 검사 시작지점 변경
            now = next;
        }
    }

    // O(logN)
    public T Pop()
    {
        // 반환할 데이터를 따로 저장
        T ret = _heap[0];

        // 마지막 데이터를 루트로 이동한다
        int lastIndex = _heap.Count - 1;
        _heap[0] = _heap[lastIndex];
        _heap.RemoveAt(lastIndex);
        lastIndex--;

        // 루트부터 자식으로 내려가면서 재정렬 시작
        int now = 0;
        while (true)
        {
            int left = 2 * now + 1;
            int right = 2 * now + 2;

            int next = now;
            // 왼쪽값이 현재값보다 크면, 왼쪽으로 이동
            if (left <= lastIndex && _heap[next].CompareTo(_heap[left]) < 0)
            {
                next = left;
            }
            // 오른쪽값이 현재값(왼쪽 이동 포함)보다 크면, 오른쪽으로 이동
            if (right <= lastIndex && _heap[next].CompareTo(_heap[right]) < 0)
            {
                next = right;
            }

            // 왼쪽/오른쪽 모두 현재값보다 작으면 종료
            if (next == now)
            {
                break;
            }

            // 두 값을 교체
            T temp = _heap[now];
            _heap[now] = _heap[next];
            _heap[next] = temp;

            // 검사 시작지점 변경
            now = next;
        }

        return ret;
    }

    public T Peek()
    {
        if (_heap.Count == 0)
        {
            // value type 인 경우 0, reference type 인 경우 null 을 리턴한다.
            return default(T);
        }
        return _heap[0];
    }
}
