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
	map<int32, set<int32>>				_lockHistory; // 해당 정점(lock)에서 갈 수 있는 정점 정보들

	Mutex _lock;

private:
	vector<int32>	_discoveredOrder; // 노드가 발견된 순서를 기록하는 배열
	int32			_discoveredCount = 0; // 노드가 발견된 순서를 계산하기 위한 변수
	vector<bool>	_finished; // Dfs(i)가 종료되었는지 여부
	vector<int32>	_parent;  
};

