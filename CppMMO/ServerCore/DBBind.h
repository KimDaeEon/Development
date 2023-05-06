#pragma once
#include "DBConnection.h"

// ��������� value �� ��� ��Ʈ�� 1�̵ȴ�.
// FullBits<3> �̷��� �ϸ� 0~2�� ��Ʈ�� 1�� �Ǿ� �ִ� ����ü�� ���´�. 
template<int32 C>
struct FullBits
{
	enum { value = (1 << (C - 1)) | FullBits<C - 1>::value };
};

template<>
struct FullBits<1>
{
	enum { value = 1 };
};

template<>
struct FullBits<0>
{
	enum { value = 0 };
};

// �Ʒ��� ���� �ۼ��ϸ� ������ �ֿ� ���Ͽ� �������� Ŭ������ �����ȴ�.
// DBBind<2,1>�� DBBind<1,2>�� ���� �ٸ� Ŭ������ ��������� ���̴�.
template<int32 ParamCount, int32 ColumnCount>
class DBBind
{
public:
	DBBind(DBConnection& dbConnection, const WCHAR* query)
		: _dbConnection(dbConnection), _query(query)
	{
		// Flag ���� �ʰ� üũ
		ASSERT_CRASH(ParamCount < 64);
		ASSERT_CRASH(ColumnCount < 64);

		::memset(_paramIndex, 0, sizeof(_paramIndex));
		::memset(_columnIndex, 0, sizeof(_columnIndex));
		_paramFlag = 0;
		_columnFlag = 0;
		dbConnection.UnBind();
	}

	bool Validate()
	{
		// For Debugging
		int32 pCnt = ParamCount;
		int32 cCnt = ColumnCount;
		int32 validParamFlag = FullBits<ParamCount>::value;
		int32 validColumnFlag = FullBits<ColumnCount>::value;

		return _paramFlag == validParamFlag && _columnFlag == validColumnFlag;
	}

	bool Execute()
	{
		ASSERT_CRASH(Validate());
		return _dbConnection.Execute(_query);
	}

	bool Fetch()
	{
		return _dbConnection.Fetch();
	}

public:
	template<typename T>
	void BindParam(int32 idx, T& value)
	{
		// �츮 ���α׷��ӵ��� 0���� �����ϴ°� �ͼ��ϴ�. 
		// ������ SQL�� BindParam���� ���� ó�� index �� 1���� �����Ѵ�. 
		// �׷��� �� �Լ��� ���ڸ� 0���� �ް�, ���� BindParam�� �Ѱ��� ���� +1�� �Ѵ�.
		_dbConnection.BindParam(idx + 1, &value, &_paramIndex[idx]);
		_paramFlag |= (1LL << idx);
	}

	void BindParam(int32 idx, const WCHAR* value)
	{
		_dbConnection.BindParam(idx + 1, value, &_paramIndex[idx]);
		_paramFlag |= (1LL << idx);
	}

	// �Ʒ����� ���۷����� ���� ������ �ڵ��� ������ �����̴�. ���۷����� Null �� ���� �� ����.
	// const T(&value)[N]�� �ϸ� �� ���� BindParam���� ���ø� �߷еǴ� ��Ÿ ��������.
	template<typename T, int32 N>
	void BindParam(int32 idx, T(&value)[N])
	{
		_dbConnection.BindParam(idx + 1, (const BYTE*)value, sizeof32(T) * N, &_paramIndex[idx]);
		_paramFlag |= (1LL << idx);
	}

	template<typename T>
	void BindParam(int32 idx, T* value, int32 N)
	{
		_dbConnection.BindParam(idx + 1, (const BYTE*)value, sizeof32(T) * N, &_paramIndex[idx]);
		_paramFlag |= (1LL << idx);
	}

	template<typename T>
	void BindCol(int32 idx, T& value)
	{
		_dbConnection.BindCol(idx + 1, &value, &_columnIndex[idx]);
		_columnFlag |= (1LL << idx);
	}

	// TODO: NULL ������ N - 1�̳� �� �Ʒ� �ڵ�� len - 1�� �ϴµ�, �̷��� �ϴ°� �³� Ȯ�� �ʿ�
	template<int32 N>
	void BindCol(int32 idx, WCHAR(&value)[N])
	{
		_dbConnection.BindCol(idx + 1, value, N - 1, &_columnIndex[idx]);
		_columnFlag |= (1LL << idx);
	}

	void BindCol(int32 idx, WCHAR* value, int32 len)
	{
		_dbConnection.BindCol(idx + 1, value, len - 1, &_columnIndex[idx]);
		_columnFlag |= (1LL << idx);
	}

	template<typename T, int32 N>
	void BindCol(int32 idx, T(&value)[N])
	{
		_dbConnection.BindCol(idx + 1, value, sizeof32(T) * N, &_columnIndex[idx]);
		_columnFlag |= (1LL << idx);
	}

protected:
	DBConnection&	_dbConnection;
	const WCHAR*	_query;
	SQLLEN			_paramIndex[ParamCount > 0 ? ParamCount : 1]; // ũ�Ⱑ 0�� �迭�� ������ �� ���⿡..
	SQLLEN			_columnIndex[ColumnCount > 0 ? ColumnCount : 1];
	uint64			_paramFlag;
	uint64			_columnFlag;

};

