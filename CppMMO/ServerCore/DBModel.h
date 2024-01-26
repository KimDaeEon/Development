#pragma once
#include "Types.h"

NAMESPACE_BEGIN(DBModel)

MAKE_SHARED_PTR_TYPE(Column);
MAKE_SHARED_PTR_TYPE(Index);
MAKE_SHARED_PTR_TYPE(Table);
MAKE_SHARED_PTR_TYPE(Procedure);

// �ش� Ŭ������ SQL Server �� ����ϱ� ���� Ŭ�������� ��������.
// MySQL ���� ���� ���Եȴٸ� ������ ������ �ʿ䰡 �ִ�.
// DB��� �ּ��� ������ DB������ ����ϴ� ������ ���̴�.
// XML�̶�� �ּ��� ������ ���������� XML������ ����ϴ� ������ ���̴�.

// --------------------------
//			DataType
// --------------------------
// �Ʒ��� SqlServer�� objectType ��ȣ�̴�. ��� DB�� �ٲ�� �̰͵� ���� �ʿ�
enum class DataType
{
	None = 0,
	TinyInt = 48,
	SmallInt = 52,
	Int = 56,
	Real = 59,
	DateTime = 61,
	Float = 62,
	Bit = 104,
	Numeric = 108,
	BigInt = 127,
	VarBinary = 165,
	Varchar = 167,
	Binary = 173,
	NVarChar = 231,
};

// ------------------------
//			Column
// ------------------------
class Column
{
public:
	myString				CreateText();

public:
	myString				_name;
	int32					_columnId = 0; // DB
	DataType				_type = DataType::None;
	myString				_typeText;
	int32					_maxLength = 0;
	bool					_nullable = false;
	bool					_identity = false; // identity (1,1) ���� �� �� identity ���� �ǹ�
	int64					_seedValue = 0; // identity (1,1) ���� �� ó�� 1 �ǹ�, identity ���� ��
	int64					_incrementValue = 0; // identity (1,1) ���� �� �� 1 �ǹ�, identity ���� ��
	myString				_default;
	myString				_defaultConstraintName; // DB
};

// ------------------------
//		  IndexType
// ------------------------
enum class IndexType
{
	Clustered = 1,
	NonClustered = 2
};

class Index
{
public:
	myString				GetUniqueName();
	myString				CreateName(const myString& tableName);
	myString				GetTypeText();
	myString				GetKeyText();
	myString				CreateColumnsText();
	bool					DependsOn(const myString& columnName);

public:
	myString				_name; // DB
	int32					_indexId = 0; // DB
	IndexType				_type = IndexType::NonClustered;
	bool					_primaryKey = false;
	bool					_uniqueConstraint = false;
	myVector<ColumnRef>		_columns;
};

// -----------------------
//			Table
// -----------------------
class Table
{
public:
	ColumnRef				FindColumn(const myString& columnName);

public:
	int32					_objectId = 0; // DB
	myString				_name;
	myVector<ColumnRef>		_columns;
	myVector<IndexRef>		_indexes;
};

// --------------------
//		Procedure
// --------------------
struct Param
{
	myString				_name;
	myString				_type;
};

class Procedure
{
public:
	myString				GenerateCreateQuery();
	myString				GenerateAlterQuery();
	myString				GenerateParamString();

public:
	myString				_name;
	myString				_fullBody; // DB
	myString				_body; // XML
	myVector<Param>			_parameters;  // XML
};

// ------------------
//		Helpers
// ------------------
class Helpers
{
public:
	static myString			Format(const WCHAR* format, ...);
	static myString			DataTypeToString(DataType type);
	static myString			RemoveWhiteSpace(const myString& str);
	static DataType			StringToDataType(const WCHAR* str, OUT int32& maxLen);
};

// SQL Ÿ�� ���� ���� ���ڿ�
constexpr wchar_t TINY_INT[] = L"TinyInt";
constexpr wchar_t SMALL_INT[] = L"SmallInt";
constexpr wchar_t INT[] = L"Int";
constexpr wchar_t REAL[] = L"Real";
constexpr wchar_t DATE_TIME[] = L"DateTime";
constexpr wchar_t FLOAT[] = L"Float";
constexpr wchar_t BIT[] = L"Bit";
constexpr wchar_t NUMERIC[] = L"Numeric";
constexpr wchar_t BIGINT[] = L"BigInt";
constexpr wchar_t VAR_BINARY[] = L"VarBinary";
constexpr wchar_t VAR_CHAR[] = L"Varchar";
constexpr wchar_t BINARY[] = L"Binary";
constexpr wchar_t NVARCHAR[] = L"NVarChar";
constexpr wchar_t NONE[] = L"None";

NAMESPACE_END