#pragma once
#include <stack>
#include <map>
#include <vector>

// -------------------------
//		DeadLockProfiler
// -------------------------
class DeadLockDetector
{
public:
	void PushLock(const char* name);
	void PopLock(const char* name);
	void CheckCycle();

private:
	void Dfs(int32 currentNode);

private:
	unordered_map<const char*, int32>	_nameToId;
	unordered_map<int32, const char*>	_idToName;
	map<int32, set<int32>>				_lockHistory; // �ش� ����(lock)���� �� �� �ִ� ���� ������

	Mutex _lock;

private:
	vector<int32>	_discoveredOrder; // ��尡 �߰ߵ� ������ ����ϴ� �迭
	int32			_discoveredCount = 0; // ��尡 �߰ߵ� ������ ����ϱ� ���� ����
	vector<bool>	_finished; // Dfs(i)�� ����Ǿ����� ����
	vector<int32>	_parent;  
};

