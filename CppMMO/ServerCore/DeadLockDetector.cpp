#include "pch.h"
#include "DeadLockDetector.h"

void DeadLockDetector::PushLock(const char* name)
{
	LockGuard guard(_lock);

	// ���̵� ã�ų� �߱��Ѵ�.
	int32 lockId = 0;

	auto findIt = _nameToId.find(name);

	// ������ ���� lock�̶�� ���� �׸� �߰�
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

	// ��� �ִ� ���� �־��ٸ� 
	if (LLockStack.empty() == false)
	{
		// ������ Ȯ�ε��� ���� ���̶�� ����� ���� �ٽ� Ȯ��
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
	_discoveredOrder = vector<int32>(lockCount, -1); // -1�� �߰ߵ��� ���� ����
	_discoveredCount = 0;
	_finished = vector<bool>(lockCount, false);
	_parent = vector<int32>(lockCount, -1);

	for (int32 lockId = 0; lockId < lockCount; lockId++)
	{
		Dfs(lockId);
	}

	// ������ �����ٸ� ����
	_discoveredOrder.clear();
	_finished.clear();
	_parent.clear();
}

void DeadLockDetector::Dfs(int32 currentNode)
{
	// �̹� �湮�� ���� ���¸� �湮���� �ʴ´�.
	if (_discoveredOrder[currentNode] != -1)
	{
		return;
	}

	_discoveredOrder[currentNode] = _discoveredCount++;

	// ���� ���� lock�� �ִ��� Ȯ�� (��� ������ ����(lock)�� ��ȸ)
	auto findIt = _lockHistory.find(currentNode);
	if (findIt == _lockHistory.end())
	{
		_finished[currentNode] = true;
		return;
	}

	// �ش� lock�� ���� ���¿��� �ٸ� lock�� ���� ��찡 �ִٸ� cycle�� �ִ��� Ȯ��
	set<int32>& adjacentNodes = findIt->second;
	for (int32 node : adjacentNodes)
	{
		// ���� �湮�� ���� ���ٸ� �湮
		if (_discoveredOrder[node] == -1)
		{
			_parent[node] = currentNode;
			Dfs(node);
			continue;
		}

		// �̹� �湮�� ���� �ִٸ�
		
		// ���� ��ġ�� ���� ��ġ���� ���� �߰ߵǾ��ٸ�, ������ Ž���̴�.
		if (_discoveredOrder[currentNode] < _discoveredOrder[node])
		{
			continue;
		}

		// ������ Ž���� �ƴϰ� Dfs(node)�� ���� ������� �ʾҴٸ�,
		// ������ Ž���� �ǰ� �ִ� ���̰� ������� �߻��� �� �ִ� ��Ȳ�� ���̴�.
		if (_finished[node] == false)
		{
			// � ������ ������ ������� ���
			printf("%s -> %s\n", _idToName[currentNode], _idToName[node]);

			// ���� ��带 Ÿ���鼭 ������� �߻��� cycle�� ���
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
