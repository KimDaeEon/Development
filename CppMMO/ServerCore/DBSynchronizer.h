#include "DBConnection.h"
#include "DBModel.h"

// --------------------------
//		 DBSynchronizer
// --------------------------

class DBSynchronizer
{
	enum
	{
		PROCEDURE_MAX_LEN = 10000
	};

	// 아래의 순서대로 Update가 진행된다.
	enum UpdateStep : uint8
	{
		DropIndex,
		AlterColumn,
		AddColumn,
		CreateTable,
		DefaultConstraint,
		CreateIndex,
		DropColumn,
		DropTable,
		StoredProcecure,

		Max
	};

	enum ColumnFlag : uint8
	{
		Type = 1 << 0,
		Nullable = 1 << 1,
		Identity = 1 << 2,
		Default = 1 << 3,
		Length = 1 << 4,
	};

public:
	DBSynchronizer(DBConnection& conn) : _dbConn(conn) {}
	~DBSynchronizer();

	bool		Synchronize(const WCHAR* path);

private:
	void		ParseXmlDB(const WCHAR* path);
	bool		GatherDBTables();
	bool		GatherDBIndexes();
	bool		GatherDBStoredProcedures();

	// TODO: 아래 Compare 함수들은 틀린 부분이 있을 수 있으니 사전에 DB 마이그레이션 테스트 해보면서 틀린 부분 찾으면 고쳐야 한다.
	void		CompareDBModel();
	void		CompareTables(DBModel::TableRef dbTable, DBModel::TableRef xmlTable);
	void		CompareColumns(DBModel::TableRef dbTable, DBModel::ColumnRef dbColumn, DBModel::ColumnRef xmlColumn);
	void		CompareStoredProcedures();

	void		ExecuteUpdateQueries();

private:
	DBConnection& _dbConn;

	myVector<DBModel::TableRef>			_xmlTables; // XML 에서 읽어온 테이블 정보
	myVector<DBModel::ProcedureRef>		_xmlProcedures; // XML 에서 읽어온 SP 정보
	mySet<myString>						_xmlRemovedTables;

	myVector<DBModel::TableRef>			_dbTables; // 실제 DB에서 읽어온 테이블 정보
	myVector<DBModel::ProcedureRef>		_dbProcedures; // 실제 DB에서 읽어온 SP 정보

private:
	mySet<myString>						_dependentIndexes;
	myVector<myString>					_updateQueries[UpdateStep::Max];
};