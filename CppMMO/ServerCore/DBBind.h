#pragma once
#include "DBConnection.h"

// 해당 클래스는 DBConnection을 쉽게 쓸 수 있도록 도와주는 클래스이다.

// 재귀적으로 value 의 모든 비트가 1이된다.
// FullBits<3> 이렇게 하면 0~2번 비트가 1이 되어 있는 구조체가 나온다. 
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

// 아래와 같이 작성하면 정수값 쌍에 대하여 독립적인 클래스가 생성된다.
// DBBind<2,1>과 DBBind<1,2>가 각각 다른 클래스로 만들어지는 것이다.
template<int32 ParamCount, int32 ColumnCount>
class DBBind
{
public:
	DBBind(DBConnection& dbConnection, const WCHAR* query)
		: _dbConnection(dbConnection), _query(query)
	{
		// Flag 범위 초과 체크
		ASSERT_CRASH(ParamCount < 64);
		ASSERT_CRASH(ColumnCount < 64);

		// 내부 변수 초기화
		::memset(_paramIndexs, 0, sizeof(_paramIndexs));
		::memset(_columnIndexs, 0, sizeof(_columnIndexs));
		_paramFlag = 0;
		_columnFlag = 0;
		dbConnection.UnBind();
	}

	bool Validate()
	{
		// 아래 변수들은 디버깅 시에 변수값 확인을 위한 것들이다.
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
		// 우리 프로그래머들은 0부터 시작하는게 익숙하다. 
		// 하지만 SQL의 BindParam같은 경우는 처음 index 가 1부터 시작한다. 
		// 그래서 이 함수는 인자를 0부터 받고, 내부 BindParam에 넘겨줄 때에 +1을 한다.
		_dbConnection.BindParam(idx + 1, &value, &_paramIndexs[idx]);
		_paramFlag |= (1LL << idx); // flag는 0번째 idx부터 1씩 채워나간다.
	}

	void BindParam(int32 idx, const WCHAR* value)
	{
		_dbConnection.BindParam(idx + 1, value, &_paramIndexs[idx]);
		_paramFlag |= (1LL << idx);
	}

	// 아래에서 레퍼런스를 쓰는 이유는 코드의 안정성 때문이다. 레퍼런스는 Null 을 받을 수 없다.
	// const T(&value)[N]로 하면 맨 위의 BindParam으로 템플릿 추론되니 오타 주의하자.
	template<typename T, int32 N>
	void BindParam(int32 idx, T(&value)[N])
	{
		_dbConnection.BindParam(idx + 1, (const BYTE*)value, sizeof32(T) * N, &_paramIndexs[idx]);
		_paramFlag |= (1LL << idx);
	}

	template<typename T>
	void BindParam(int32 idx, T* value, int32 N)
	{
		_dbConnection.BindParam(idx + 1, (const BYTE*)value, sizeof32(T) * N, &_paramIndexs[idx]);
		_paramFlag |= (1LL << idx);
	}

	template<typename T>
	void BindCol(int32 idx, T& value)
	{
		_dbConnection.BindCol(idx + 1, &value, &_columnIndexs[idx]);
		_columnFlag |= (1LL << idx);
	}

	// 결과 받아올 배열에서 NULL 공간은 빼고 받아야 문자열로 인식가능하니 -1 한다.
	template<int32 N>
	void BindCol(int32 idx, WCHAR(&value)[N])
	{
		_dbConnection.BindCol(idx + 1, value, N - 1, &_columnIndexs[idx]);
		_columnFlag |= (1LL << idx);
	}

	void BindCol(int32 idx, WCHAR* value, int32 len)
	{
		_dbConnection.BindCol(idx + 1, value, len - 1, &_columnIndexs[idx]);
		_columnFlag |= (1LL << idx);
	}

	// 위의 WCHAR 버전과 다르게 문자열이 아니라서 배열의 길이 전체를 다 받아오는데에 사용한다.
	template<typename T, int32 N>
	void BindCol(int32 idx, T(&value)[N])
	{
		_dbConnection.BindCol(idx + 1, value, sizeof32(T) * N, &_columnIndexs[idx]);
		_columnFlag |= (1LL << idx);
	}

protected:
	DBConnection&	_dbConnection;
	const WCHAR*	_query;
	SQLLEN			_paramIndexs[ParamCount > 0 ? ParamCount : 1]; // 크기가 0인 배열은 선언할 수 없기에..
	SQLLEN			_columnIndexs[ColumnCount > 0 ? ColumnCount : 1];
	uint64			_paramFlag; // bit로_parmIndexs 에서 똑같은 인덱스에 바인딩 안되도록 체크하는 변수
	uint64			_columnFlag; // bit로_columnIndexs 에서 똑같은 인덱스에 바인딩 안되도록 체크하는 변수
};

