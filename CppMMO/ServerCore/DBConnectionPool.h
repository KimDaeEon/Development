#pragma once
#include "DBConnection.h"

// -----------------------------------
//			DBConnectionPool
// -----------------------------------
class DBConnectionPool
{
public:
	DBConnectionPool();
	~DBConnectionPool();

	bool			Connect(int32 connectionCount, const WCHAR* connectionString);

	DBConnection*	Pop();
	void			Push(DBConnection* connection);

private:
	void			Clear();

	Mutex 						_mutex;
	SQLHENV						_hEnv = SQL_NULL_HANDLE; // SQL Handle Environment
	myVector<DBConnection*>		_connections;
};

