#include "pch.h"
#include "DeadLockDetector.h"

void DeadLockDetector::PushLock(const char* name)
{
	LockGuard guard(_lock);

	// 아이디를 찾거나 발급한다.
	int32 lockId = 0;

	auto findIt = _nameToId.find(name);

	// 기존에 없던 lock이라면 감시 항목에 추가
	if (findIt == _nameToId.end())
	{
		lockId = static_cast<int32>(_nameToId.size());
		_nameToId[name] = lockId;
		_idToName[lockId] = name;
	}
	else
	{
		lockId = findIt->second;
	}

	// 잡고 있는 락이 있었다면 
	if (LLockStack.empty() == false)
	{
		// 기존에 확인되지 않은 락이라면 데드락 여부 다시 확인
		const int32 prevId = LLockStack.top();
		if (lockId != prevId)
		{
			set<int32>& history = _lockHistory[prevId];
			if (history.find(lockId) == history.end())
			{
				history.insert(lockId);
				CheckCycle();
			}
		}
	}

	LLockStack.push(lockId);

}

void DeadLockDetector::PopLock(const char* name)
{
	LockGuard guard(_lock);

	if (LLockStack.empty())
	{
		CRASH("MULTIPLE_UNLOCK");
	}

	int32 lockId = _nameToId[name];
	if (LLockStack.top() != lockId)
	{
		CRASH("INVALID_UNLOCK");
	}

	LLockStack.pop();
}

void DeadLockDetector::CheckCycle()
{
	const int32 lockCount = static_cast<int32>(_nameToId.size());
	_discoveredOrder = vector<int32>(lockCount, -1); // -1이 발견되지 않은 정점
	_discoveredCount = 0;
	_finished = vector<bool>(lockCount, false);
	_parent = vector<int32>(lockCount, -1);

	for (int32 lockId = 0; lockId < lockCount; lockId++)
	{
		Dfs(lockId);
	}

	// 연산이 끝났다면 정리
	_discoveredOrder.clear();
	_finished.clear();
	_parent.clear();
}

void DeadLockDetector::Dfs(int32 currentNode)
{
	// 이미 방문이 끝난 상태면 방문하지 않는다.
	if (_discoveredOrder[currentNode] != -1)
	{
		return;
	}

	_discoveredOrder[currentNode] = _discoveredCount++;

	// 같이 잡은 lock이 있는지 확인 (모든 인접한 정점(lock)을 순회)
	auto findIt = _lockHistory.find(currentNode);
	if (findIt == _lockHistory.end())
	{
		_finished[currentNode] = true;
		return;
	}

	// 해당 lock을 잡은 상태에서 다른 lock을 잡은 경우가 있다면 cycle이 있는지 확인
	set<int32>& adjacentNodes = findIt->second;
	for (int32 node : adjacentNodes)
	{
		// 아직 방문한 적이 없다면 방문
		if (_discoveredOrder[node] == -1)
		{
			_parent[node] = currentNode;
			Dfs(node);
			continue;
		}

		// 이미 방문한 적이 있다면
		
		// 현재 위치가 다음 위치보다 먼저 발견되었다면, 순방향 탐색이다.
		if (_discoveredOrder[currentNode] < _discoveredOrder[node])
		{
			continue;
		}

		// 순방향 탐색이 아니고 Dfs(node)가 아직 종료되지 않았다면,
		// 역방향 탐색이 되고 있는 것이고 데드락이 발생할 수 있는 상황인 것이다.
		if (_finished[node] == false)
		{
			// 어떤 락에서 문제가 생겼는지 출력
			printf("%s -> %s\n", _idToName[currentNode], _idToName[node]);

			// 조상 노드를 타고가면서 데드락이 발생한 cycle을 출력
			int32 now = currentNode;
			while (true)
			{
				printf("%s -> %s\n", _idToName[_parent[now]], _idToName[now]);
				now = _parent[now];
				if (now == node)
				{
					break;
				}
			}

			CRASH("DEADLOCK_DETECTED")
		}
	}

	_finished[currentNode] = true;
}
