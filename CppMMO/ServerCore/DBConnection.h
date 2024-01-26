#pragma once
#include <sql.h>
#include <sqlext.h>

// --------------------------------
//			DBConnection
// --------------------------------
enum
{
	INVALID_SQL_ROW_COUNT = -1,
	WVARCHAR_MAX = 4000, // SQL Server 4000, MySQL�� ��� 65535
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
	bool		BindParam(int32 paramIndex, TIMESTAMP_STRUCT* value, SQLLEN* index); // ���⼭ TIMESTAMP_STRUCT�� SQL�� Datetime(2)�� �����Ѵ�. SQL Server�� TimeStamp�� ���̺� Row Version ���� �뵵 �����ʹ� �򰥸��� ����.
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
	bool		BindParam(SQLUSMALLINT paramIndex, SQLSMALLINT cType /*C����� Ÿ��*/, SQLSMALLINT sqlType/*SQL�� Ÿ��*/, SQLULEN len, SQLPOINTER ptr, SQLLEN* index);
	bool		BindCol(SQLUSMALLINT columIndex, SQLSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN* index); // �÷� �� ������ ���� ���
	void		HandleError(SQLRETURN ret);

private:
	SQLHDBC		_hConnection = SQL_NULL_HANDLE; // DB ����� �ڵ�
	SQLHSTMT	_hStatement = SQL_NULL_HANDLE; // ���� ����� �ڵ�
};
