#pragma once
#include "Types.h"

NAMESPACE_BEGIN(DBModel)

MAKE_SHARED_PTR_TYPE(Column);
MAKE_SHARED_PTR_TYPE(Index);
MAKE_SHARED_PTR_TYPE(Table);
MAKE_SHARED_PTR_TYPE(Procedure);

// 해당 클래스는 SQL Server 를 사용하기 위한 클래스임을 참고하자.
// MySQL 같은 것을 쓰게된다면 내용을 변경할 필요가 있다.
// DB라는 주석이 있으면 DB에서만 사용하는 정보란 뜻이다.
// XML이라는 주석이 있으면 마찬가지로 XML에서만 사용하는 정보란 뜻이다.

// --------------------------
//			DataType
// --------------------------
// 아래는 SqlServer의 objectType 번호이다. 사용 DB가 바뀌면 이것도 변경 필요
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
	bool					_identity = false; // identity (1,1) 에서 맨 앞 identity 여부 의미
	int64					_seedValue = 0; // identity (1,1) 에서 맨 처음 1 의미, identity 시작 값
	int64					_incrementValue = 0; // identity (1,1) 에서 맨 뒤 1 의미, identity 증가 값
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

// SQL 타입 관련 정적 문자열
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