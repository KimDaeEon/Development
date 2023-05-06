#include "pch.h"
#include "DBConnection.h"
// --------------------------------
//			DBConnection
// --------------------------------

// TODO: 아래 코드들 무슨 의미인지 다 확인
bool DBConnection::Connect(SQLHENV hEnv, const WCHAR* connectionString)
{
    if (::SQLAllocHandle(SQL_HANDLE_DBC, hEnv, OUT &_hConnection) != SQL_SUCCESS)
    {
        return false;
    }

    WCHAR stringBuffer[MAX_PATH] = { 0 };
    ::wcscpy_s(stringBuffer, connectionString);

    WCHAR resultString[MAX_PATH] = { 0 };
    SQLSMALLINT resultStringLen = { 0 };

    SQLRETURN ret = ::SQLDriverConnectW(
        _hConnection,
        NULL,
        reinterpret_cast<SQLWCHAR*>(stringBuffer),
        _countof(stringBuffer),
        OUT reinterpret_cast<SQLWCHAR*>(resultString),
        _countof(resultString),
        OUT &resultStringLen,
        SQL_DRIVER_NOPROMPT
    );

    if (::SQLAllocHandle(SQL_HANDLE_STMT, _hConnection, &_hStatement) != SQL_SUCCESS)
    {
        return false;
    }

    return (ret == SQL_SUCCESS) || (ret == SQL_SUCCESS_WITH_INFO);
}

void DBConnection::Clear()
{
    if (_hConnection != SQL_NULL_HANDLE)
    {
        ::SQLFreeHandle(SQL_HANDLE_DBC, _hConnection);
        _hConnection = SQL_NULL_HANDLE;
    }

    if (_hStatement != SQL_NULL_HANDLE) 
    {
		::SQLFreeHandle(SQL_HANDLE_STMT, _hStatement);
        _hStatement = SQL_NULL_HANDLE;
    }
}

bool DBConnection::Execute(const WCHAR* query)
{
    SQLRETURN ret = ::SQLExecDirectW(_hStatement, (SQLWCHAR*)query, SQL_NTSL);
    
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        return true;
    }

    HandleError(ret);

    return false;
}

bool DBConnection::Fetch()
{
    SQLRETURN ret = ::SQLFetch(_hStatement);

    switch (ret)
    {
    case SQL_SUCCESS:
    case SQL_SUCCESS_WITH_INFO:
        return true;

    case SQL_NO_DATA:
        return false;

    case SQL_ERROR:
        HandleError(ret);
        return false;

    default:
        CRASH("Unknown SQL state");
        return false;
    }
}

int32 DBConnection::GetRowCount()
{
    SQLLEN count = 0;
    SQLRETURN ret = ::SQLRowCount(_hStatement, OUT &count);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        return static_cast<int32>(count);
    }

    return -1;
}

void DBConnection::UnBind()
{
    ::SQLFreeStmt(_hStatement, SQL_UNBIND);
    ::SQLFreeStmt(_hStatement, SQL_RESET_PARAMS);
    ::SQLFreeStmt(_hStatement, SQL_CLOSE);
}

bool DBConnection::BindParam(int32 paramIndex, bool* value, SQLLEN* index)
{
    return BindParam(paramIndex, SQL_C_TINYINT, SQL_TINYINT, sizeof32(bool), value, index);
}

bool DBConnection::BindParam(int32 paramIndex, float* value, SQLLEN* index)
{
    return BindParam(paramIndex, SQL_C_FLOAT, SQL_REAL, 0, value, index);
}

bool DBConnection::BindParam(int32 paramIndex, double* value, SQLLEN* index)
{
    return BindParam(paramIndex, SQL_C_DOUBLE, SQL_DOUBLE, 0, value, index);
}

bool DBConnection::BindParam(int32 paramIndex, int8* value, SQLLEN* index)
{
    return BindParam(paramIndex, SQL_C_TINYINT, SQL_TINYINT, sizeof32(int8), value, index);
}

bool DBConnection::BindParam(int32 paramIndex, int16* value, SQLLEN* index)
{
    return BindParam(paramIndex, SQL_C_SHORT, SQL_SMALLINT, sizeof32(int16), value, index);
}

bool DBConnection::BindParam(int32 paramIndex, int32* value, SQLLEN* index)
{
	return BindParam(paramIndex, SQL_C_LONG, SQL_INTEGER, sizeof32(int32), value, index);
}

bool DBConnection::BindParam(int32 paramIndex, int64* value, SQLLEN* index)
{
	return BindParam(paramIndex, SQL_C_SBIGINT, SQL_BIGINT, sizeof32(int64), value, index);
}

bool DBConnection::BindParam(int32 paramIndex, TIMESTAMP_STRUCT* value, SQLLEN* index)
{
	return BindParam(paramIndex, SQL_C_TYPE_TIMESTAMP, SQL_C_TYPE_TIMESTAMP, sizeof32(TIMESTAMP_STRUCT), value, index);
}

// ODBC가 얼마나 메모리를 할당하고 사용해야하는지 알 수 없기에 바이트 단위 길이를 알려준다.
// 그래서 size에 * 2를 하는데, BindCol과 차이가 있다는 것을 참고하자.
bool DBConnection::BindParam(int32 paramIndex, const WCHAR* str, SQLLEN* index)
{
    SQLULEN size = static_cast<SQLULEN>((::wcslen(str) + 1) * 2); // wchar 이므로 크기 * 2
    *index = SQL_NTSL;

    if (size > WVARCHAR_MAX)
    {
        return BindParam (paramIndex, SQL_C_WCHAR, SQL_WLONGVARCHAR, size, (SQLPOINTER)str, index);
    }
    else
    {
        return BindParam (paramIndex, SQL_C_WCHAR, SQL_WVARCHAR, size, (SQLPOINTER)str, index);
    }
}

// 이미지 파일 같은 것 저장할 때에 사용
bool DBConnection::BindParam(int32 paramIndex, const BYTE* binary, int32 size, SQLLEN* index)
{
    if (binary == nullptr)
    {
        *index = SQL_NULL_DATA;
        size = 1;
    }
    else
    {
        *index = size;
    }

    if (size > BINARY_MAX)
    {
        return BindParam(paramIndex, SQL_C_BINARY, SQL_LONGVARBINARY, size, (BYTE*)binary, index);
    }
    else
    {
        return BindParam(paramIndex, SQL_C_BINARY, SQL_BINARY, size, (BYTE*)binary, index);
    }
}

bool DBConnection::BindCol(int32 columnIndex, bool* value, SQLLEN* index)
{
    return BindCol(columnIndex, SQL_C_TINYINT, sizeof32(bool), value, index);
}

bool DBConnection::BindCol(int32 columnIndex, float* value, SQLLEN* index)
{
    return BindCol(columnIndex, SQL_C_FLOAT, sizeof32(float), value, index);
}

bool DBConnection::BindCol(int32 columnIndex, double* value, SQLLEN* index)
{
    return BindCol(columnIndex, SQL_C_DOUBLE, sizeof32(double), value, index);
}

bool DBConnection::BindCol(int32 columnIndex, int8* value, SQLLEN* index)
{
    return BindCol(columnIndex, SQL_C_TINYINT, sizeof32(int8), value, index);
}

bool DBConnection::BindCol(int32 columnIndex, int16* value, SQLLEN* index)
{
    return BindCol(columnIndex, SQL_C_SHORT, sizeof32(int16), value, index);
}

bool DBConnection::BindCol(int32 columnIndex, int32* value, SQLLEN* index)
{
    return BindCol(columnIndex, SQL_C_LONG, sizeof32(int32), value, index);
}

bool DBConnection::BindCol(int32 columnIndex, int64* value, SQLLEN* index)
{
    return BindCol(columnIndex, SQL_C_SBIGINT, sizeof32(int64), value, index);
}

bool DBConnection::BindCol(int32 columnIndex, TIMESTAMP_STRUCT* value, SQLLEN* index)
{
    return BindCol(columnIndex, SQL_C_TYPE_TIMESTAMP, sizeof32(TIMESTAMP_STRUCT), value, index);
}

// 여기서는 BindParam과 다르게 size*2를 하지 않는데, WCHAR 배열 형태로 데이터가 메모리에 올라오기에
// WCHAR의 크기인 size 를 그대로 넣어줘야 한다.
bool DBConnection::BindCol(int32 columnIndex, WCHAR* str, int32 size, SQLLEN* index)
{
    return BindCol(columnIndex, SQL_C_WCHAR, size, str, index);
}

bool DBConnection::BindCol(int32 columnIndex, BYTE* binary, int32 size, SQLLEN* index)
{
    return BindCol(columnIndex, SQL_BINARY, size, binary, index);
}

bool DBConnection::BindParam(SQLUSMALLINT paramIndex, SQLSMALLINT cType, SQLSMALLINT sqlType, SQLULEN len, SQLPOINTER ptr, SQLLEN* index)
{
	SQLRETURN ret = ::SQLBindParameter(_hStatement, paramIndex, SQL_PARAM_INPUT, cType, sqlType, len, 0, ptr, 0, index);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		HandleError(ret);
		return false;
	}

	return true;
}

bool DBConnection::BindCol(SQLUSMALLINT columIndex, SQLUSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN* index)
{
	SQLRETURN ret = ::SQLBindCol(_hStatement, columIndex, cType, value, len, index);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		HandleError(ret);
		return false;
	}

	return true;
}

void DBConnection::HandleError(SQLRETURN ret)
{
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        cout << "Invalid HandleError(DB)" << endl;
        return;
    }

    SQLSMALLINT index = 1;
    SQLWCHAR sqlState[MAX_PATH] = { 0 };
    SQLINTEGER nativeErr = 0;
    SQLWCHAR errMsg[MAX_PATH] = { 0 };
    SQLSMALLINT msgLen = 0;
    SQLRETURN errorRet = 0;

    while (true)
    {
        errorRet = ::SQLGetDiagRecW(
            SQL_HANDLE_STMT,
            _hStatement,
            index,
            OUT sqlState,
            OUT &nativeErr,
            OUT errMsg,
            _countof(errMsg),
            OUT &msgLen
        );

        if (errorRet == SQL_NO_DATA)
        {
            break;
        }

        if (errorRet != SQL_SUCCESS && errorRet != SQL_SUCCESS_WITH_INFO)
        {
            break;
        }

        // TODO : Log
        wcout.imbue(locale("kor"));
        wcout << errMsg << endl;

        index++;
    }
}
