#include "pch.h"
#include "DBModel.h"
using namespace DBModel;
#include <regex>


// ------------------------
//			Column
// ------------------------

myString Column::CreateText()
{
	return DBModel::Helpers::Format(
		L"[%s] %s %s %s",
		_name.c_str(),
		_typeText.c_str(),
		_nullable ? L"NULL" : L"NOT NULL",
		_identity ? DBModel::Helpers::Format(L"IDENTITY(%d, %d)", _seedValue, _incrementValue).c_str() : L"");
}


// --------------------
//		  Index
// --------------------

myString Index::GetUniqueName()
{
	myString ret;

	ret += _primaryKey ? L"PK " : L" ";
	ret += _uniqueConstraint ? L"UK " : L" ";
	ret += (_type == IndexType::Clustered ? L"C " : L"NC ");

	for (const ColumnRef& column : _columns)
	{
		ret += L"*";
		ret += column->_name;
		ret += L" ";
	}

	return ret;
}

myString Index::CreateName(const myString& tableName)
{
	myString ret = L"IX_" + tableName;

	for (const ColumnRef& column : _columns)
	{
		ret += L"_";
		ret += column->_name;
	}

	return ret;
}

myString Index::GetTypeText()
{
	return (_type == IndexType::Clustered ? L"CLUSTERED" : L"NONCLUSTERED");
}

myString Index::GetKeyText()
{
	if (_primaryKey)
		return L"PRIMARY KEY";

	if (_uniqueConstraint)
		return L"UNIQUE";

	return L"";
}

myString Index::CreateColumnsText()
{
	myString ret;

	const int32 size = static_cast<int32>(_columns.size());
	for (int32 i = 0; i < size; i++)
	{
		if (i > 0)
			ret += L", ";

		ret += DBModel::Helpers::Format(L"[%s]", _columns[i]->_name.c_str());
	}

	return ret;
}

bool Index::DependsOn(const myString& columnName)
{
	auto findIt = std::find_if(_columns.begin(), _columns.end(),
		[&](const ColumnRef& column) { return column->_name == columnName; });

	return findIt != _columns.end();
}


// -----------------------
//			Table
// -----------------------

ColumnRef Table::FindColumn(const myString& columnName)
{
	auto findIt = std::find_if(_columns.begin(), _columns.end(),
		[&](const ColumnRef& column) { return column->_name == columnName; });

	if (findIt != _columns.end())
		return *findIt;

	return nullptr;
}


// --------------------
//		Procedure
// --------------------

myString Procedure::GenerateCreateQuery()
{
	const WCHAR* query = L"CREATE PROCEDURE [dbo].[%s] %s AS BEGIN %s END";

	myString paramString = GenerateParamString();
	return DBModel::Helpers::Format(query, _name.c_str(), paramString.c_str(), _body.c_str());
}

myString Procedure::GenerateAlterQuery()
{
	const WCHAR* query = L"ALTER PROCEDURE [dbo].[%s] %s AS	BEGIN %s END";

	myString paramString = GenerateParamString();
	return DBModel::Helpers::Format(query, _name.c_str(), paramString.c_str(), _body.c_str());
}

myString Procedure::GenerateParamString()
{
	myString str;

	const int32 size = static_cast<int32>(_parameters.size());
	for (int32 i = 0; i < size; i++)
	{
		if (i < size - 1)
			str += DBModel::Helpers::Format(L"\t%s %s,\n", _parameters[i]._name.c_str(), _parameters[i]._type.c_str());
		else
			str += DBModel::Helpers::Format(L"\t%s %s", _parameters[i]._name.c_str(), _parameters[i]._type.c_str());
	}

	return str;
}


// ------------------
//		Helpers
// ------------------

myString Helpers::Format(const WCHAR* format, ...)
{
	WCHAR buf[4096];

	va_list ap;
	va_start(ap, format);
	::vswprintf_s(buf, 4096, format, ap);
	va_end(ap);

	return myString(buf);
}

myString Helpers::DataTypeToString(DataType type)
{
	switch (type)
	{
	case DataType::TinyInt:		return TINY_INT;
	case DataType::SmallInt:	return SMALL_INT;
	case DataType::Int:			return INT;
	case DataType::Real:		return REAL;
	case DataType::DateTime:	return DATE_TIME;
	case DataType::Float:		return FLOAT;
	case DataType::Bit:			return BIT;
	case DataType::Numeric:		return NUMERIC;
	case DataType::BigInt:		return BIGINT;
	case DataType::VarBinary:	return VAR_BINARY;
	case DataType::Varchar:		return VAR_CHAR;
	case DataType::Binary:		return BINARY;
	case DataType::NVarChar:	return NVARCHAR;
	default:					return NONE;
	}
}

myString Helpers::RemoveWhiteSpace(const myString& str)
{
	myString ret = str;

	ret.erase(
		std::remove_if(ret.begin(), ret.end(), [=](WCHAR ch) { return ::isspace(ch); }),
		ret.end());

	return ret;
}

DataType Helpers::StringToDataType(const WCHAR* str, OUT int32& maxLen)
{
	std::wregex reg(L"([a-z]+)(\\((max|\\d+)\\))?");
	std::wcmatch ret;

	if (std::regex_match(str, OUT ret, reg) == false)
		return DataType::None;

	if (ret[3].matched)
		maxLen = ::_wcsicmp(ret[3].str().c_str(), L"max") == 0 ? -1 : _wtoi(ret[3].str().c_str());
	else
		maxLen = 0;

	if (::_wcsicmp(ret[1].str().c_str(), TINY_INT) == 0) return DataType::TinyInt;
	if (::_wcsicmp(ret[1].str().c_str(), SMALL_INT) == 0) return DataType::SmallInt;
	if (::_wcsicmp(ret[1].str().c_str(), INT) == 0) return DataType::Int;
	if (::_wcsicmp(ret[1].str().c_str(), REAL) == 0) return DataType::Real;
	if (::_wcsicmp(ret[1].str().c_str(), DATE_TIME) == 0) return DataType::DateTime;
	if (::_wcsicmp(ret[1].str().c_str(), FLOAT) == 0) return DataType::Float;
	if (::_wcsicmp(ret[1].str().c_str(), BIT) == 0) return DataType::Bit;
	if (::_wcsicmp(ret[1].str().c_str(), NUMERIC) == 0) return DataType::Numeric;
	if (::_wcsicmp(ret[1].str().c_str(), BIGINT) == 0) return DataType::BigInt;
	if (::_wcsicmp(ret[1].str().c_str(), VAR_BINARY) == 0) return DataType::VarBinary;
	if (::_wcsicmp(ret[1].str().c_str(), VAR_CHAR) == 0) return DataType::Varchar;
	if (::_wcsicmp(ret[1].str().c_str(), BINARY) == 0) return DataType::Binary;
	if (::_wcsicmp(ret[1].str().c_str(), NVARCHAR) == 0) return DataType::NVarChar;

	return DataType::None;
}