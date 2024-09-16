#include "pch.h"
#include "DBConnectionPool.h"

// -----------------------------------
//			DBConnectionPool
// -----------------------------------
DBConnectionPool::DBConnectionPool()
{
}

DBConnectionPool::~DBConnectionPool()
{
	Clear();
}

bool DBConnectionPool::Connect(int32 connectionCount, const WCHAR* connectionString)
{
	LockGuard lock(_mutex);

	// 데이터 베이스 관리하기 위한 환경 핸들 할당
	if (::SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, OUT & _hEnv) != SQL_SUCCESS)
	{
		return false;
	}

	// 환경 핸들에 속성 부여
	if (::SQLSetEnvAttr(_hEnv, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0) != SQL_SUCCESS)
	{
		return false;
	}

	for (int32 i = 0; i < connectionCount; i++)
	{
		DBConnection* connection = myNew<DBConnection>();
		if (connection->Connect(_hEnv, connectionString) == false)
		{
			cout << "DB Connect fail!" << endl;
			return false;
		}
		_connections.push_back(connection);
	}

	return true;
}

void DBConnectionPool::Clear()
{
	LockGuard lock(_mutex);

	if (_hEnv != SQL_NULL_HANDLE)
	{
		::SQLFreeHandle(SQL_HANDLE_ENV, _hEnv);
		_hEnv = SQL_NULL_HANDLE;
	}

	for (DBConnection* connection : _connections)
	{
		myDelete(connection);
	}

	_connections.clear();
}

DBConnection* DBConnectionPool::Pop()
{
	LockGuard lock(_mutex);

	if (_connections.empty())
	{
		return nullptr;
	}

	DBConnection* connection = _connections.back();
	_connections.pop_back();

	return connection;
}

void DBConnectionPool::Push(DBConnection* connection)
{
	LockGuard lock(_mutex);
	_connections.push_back(connection);
}
