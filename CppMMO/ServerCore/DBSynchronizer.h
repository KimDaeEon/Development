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

	// �Ʒ��� ������� Update�� ����ȴ�.
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

	// TODO: �Ʒ� Compare �Լ����� Ʋ�� �κ��� ���� �� ������ ������ DB ���̱׷��̼� �׽�Ʈ �غ��鼭 Ʋ�� �κ� ã���� ���ľ� �Ѵ�.
	void		CompareDBModel();
	void		CompareTables(DBModel::TableRef dbTable, DBModel::TableRef xmlTable);
	void		CompareColumns(DBModel::TableRef dbTable, DBModel::ColumnRef dbColumn, DBModel::ColumnRef xmlColumn);
	void		CompareStoredProcedures();

	void		ExecuteUpdateQueries();

private:
	DBConnection& _dbConn;

	myVector<DBModel::TableRef>			_xmlTables; // XML ���� �о�� ���̺� ����
	myVector<DBModel::ProcedureRef>		_xmlProcedures; // XML ���� �о�� SP ����
	mySet<myString>						_xmlRemovedTables;

	myVector<DBModel::TableRef>			_dbTables; // ���� DB���� �о�� ���̺� ����
	myVector<DBModel::ProcedureRef>		_dbProcedures; // ���� DB���� �о�� SP ����

private:
	mySet<myString>						_dependentIndexes;
	myVector<myString>					_updateQueries[UpdateStep::Max];
};