#pragma once
#include <sql.h>
#include <sqlext.h>

// --------------------------------
//			DBConnection
// --------------------------------
enum
{
	INVALID_SQL_ROW_COUNT = -1,
	WVARCHAR_MAX = 4000, // SQL Server 4000, MySQL인 경우 65535
	BINARY_MAX = 8000,
};

class DBConnection
{
public:
	bool		Connect(SQLHENV hEnv, const WCHAR* connectionString);
	void		Clear();

	bool		Execute(const WCHAR* query);
	bool		Fetch();
	int32		GetRowCount();
	void		UnBind();

public:
	bool		BindParam(int32 paramIndex, bool* value, SQLLEN* index);
	bool		BindParam(int32 paramIndex, float* value, SQLLEN* index);
	bool		BindParam(int32 paramIndex, double* value, SQLLEN* index);
	bool		BindParam(int32 paramIndex, int8* value, SQLLEN* index);
	bool		BindParam(int32 paramIndex, int16* value, SQLLEN* index);
	bool		BindParam(int32 paramIndex, int32* value, SQLLEN* index);
	bool		BindParam(int32 paramIndex, int64* value, SQLLEN* index);
	bool		BindParam(int32 paramIndex, TIMESTAMP_STRUCT* value, SQLLEN* index); // 여기서 TIMESTAMP_STRUCT는 SQL의 Datetime(2)에 대응한다. SQL Server의 TimeStamp는 테이블 Row Version 관리 용도 데이터니 헷갈리지 말자.
	bool		BindParam(int32 paramIndex, const WCHAR* str, SQLLEN* index);
	bool		BindParam(int32 paramIndex, const BYTE* binary, int32 size, SQLLEN* index);

	bool		BindCol(int32 columnIndex, bool* value, SQLLEN* index);
	bool		BindCol(int32 columnIndex, float* value, SQLLEN* index);
	bool		BindCol(int32 columnIndex, double* value, SQLLEN* index);
	bool		BindCol(int32 columnIndex, int8* value, SQLLEN* index);
	bool		BindCol(int32 columnIndex, int16* value, SQLLEN* index);
	bool		BindCol(int32 columnIndex, int32* value, SQLLEN* index);
	bool		BindCol(int32 columnIndex, int64* value, SQLLEN* index);
	bool		BindCol(int32 columnIndex, TIMESTAMP_STRUCT* value, SQLLEN* index);
	bool		BindCol(int32 columnIndex, WCHAR* str, int32 size, SQLLEN* index);
	bool		BindCol(int32 columnIndex, BYTE* binary, int32 size, SQLLEN* index);

	
private:
	bool		BindParam(SQLUSMALLINT paramIndex, SQLSMALLINT cType /*C언어의 타입*/, SQLSMALLINT sqlType/*SQL의 타입*/, SQLULEN len, SQLPOINTER ptr, SQLLEN* index);
	bool		BindCol(SQLUSMALLINT columIndex, SQLSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN* index); // 컬럼 값 가져올 때에 사용
	void		HandleError(SQLRETURN ret);

private:
	SQLHDBC		_hConnection = SQL_NULL_HANDLE; // DB 연결용 핸들
	SQLHSTMT	_hStatement = SQL_NULL_HANDLE; // 인자 연결용 핸들
};

