#pragma once
#include <sql.h>
#include <sqlext.h>

// --------------------------------
//			DBConnection
// --------------------------------
enum
{
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
	bool		BindParam(int32 paramIndex, TIMESTAMP_STRUCT* value, SQLLEN* index);
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
	// 인자 넘겨줄 때에 사용
	bool		BindParam(SQLUSMALLINT paramIndex, SQLSMALLINT cType, SQLSMALLINT sqlType, SQLULEN len, SQLPOINTER ptr, SQLLEN* index);
	
	// 컬럼을 가져올 때에 사용
	bool		BindCol(SQLUSMALLINT columIndex, SQLUSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN* index);
	void		HandleError(SQLRETURN ret);

private:
	// 아래처럼 DB 연결용 핸들과 인자를 연결해서 보내는 상태 핸들이 필요하단 것을 알아두자.
	SQLHDBC		_hConnection = SQL_NULL_HANDLE;
	SQLHSTMT	_hStatement = SQL_NULL_HANDLE;
};

