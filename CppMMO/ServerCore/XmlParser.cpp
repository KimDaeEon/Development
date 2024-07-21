#include "pch.h"
#include "XmlParser.h"
#include "FileUtils.h"
#include "CoreGlobal.h"

// --------------------------
//			XmlNode
// --------------------------
_locale_t kr = _create_locale(LC_NUMERIC, "kor");

bool XmlNode::GetBoolAttr(const WCHAR* key, bool defaultValue)
{
	XmlAttributeType* attr = _node->first_attribute(key);
	if (attr)
		return ::_wcsicmp(attr->value(), L"true") == 0;

	return defaultValue;
}


int8 XmlNode::GetInt8Attr(const WCHAR* key, int8 defaultValue)
{
	XmlAttributeType* attr = _node->first_attribute(key);
	if (attr)
		return static_cast<int8>(::_wtoi(attr->value()));

	return defaultValue;
}

int16 XmlNode::GetInt16Attr(const WCHAR* key, int16 defaultValue)
{
	XmlAttributeType* attr = _node->first_attribute(key);
	if (attr)
		return static_cast<int16>(::_wtoi(attr->value()));

	return defaultValue;
}

int32 XmlNode::GetInt32Attr(const WCHAR* key, int32 defaultValue)
{
	XmlAttributeType* attr = _node->first_attribute(key);
	if (attr)
		return ::_wtoi(attr->value());

	return defaultValue;
}

int64 XmlNode::GetInt64Attr(const WCHAR* key, int64 defaultValue)
{
	xml_attribute<WCHAR>* attr = _node->first_attribute(key);
	if (attr)
		return ::_wtoi64(attr->value());

	return defaultValue;
}

float XmlNode::GetFloatAttr(const WCHAR* key, float defaultValue)
{
	XmlAttributeType* attr = _node->first_attribute(key);
	if (attr)
		return static_cast<float>(::_wtof(attr->value()));

	return defaultValue;
}

double XmlNode::GetDoubleAttr(const WCHAR* key, double defaultValue)
{
	XmlAttributeType* attr = _node->first_attribute(key);
	if (attr)
		return ::_wtof_l(attr->value(), kr);

	return defaultValue;
}

const WCHAR* XmlNode::GetStringAttr(const WCHAR* key, const WCHAR* defaultValue)
{
	XmlAttributeType* attr = _node->first_attribute(key);
	if (attr)
		return attr->value();

	return defaultValue;
}

bool XmlNode::GetBoolValue(bool defaultValue)
{
	WCHAR* val = _node->value();
	if (val)
		return ::_wcsicmp(val, L"true") == 0;

	return defaultValue;
}

int8 XmlNode::GetInt8Value(int8 defaultValue)
{
	WCHAR* val = _node->value();
	if (val)
		return static_cast<int8>(::_wtoi(val));

	return defaultValue;
}

int16 XmlNode::GetInt16Value(int16 defaultValue)
{
	WCHAR* val = _node->value();
	if (val)
		return static_cast<int16>(::_wtoi(val));
	return defaultValue;
}

int32 XmlNode::GetInt32Value(int32 defaultValue)
{
	WCHAR* val = _node->value();
	if (val)
		return static_cast<int32>(::_wtoi(val));

	return defaultValue;
}

int64 XmlNode::GetInt64Value(int64 defaultValue)
{
	WCHAR* val = _node->value();
	if (val)
		return static_cast<int64>(::_wtoi64(val));

	return defaultValue;
}

float XmlNode::GetFloatValue(float defaultValue)
{
	WCHAR* val = _node->value();
	if (val)
		return static_cast<float>(::_wtof(val));

	return defaultValue;
}

double XmlNode::GetDoubleValue(double defaultValue)
{
	WCHAR* val = _node->value();
	if (val)
		return ::_wtof_l(val, kr);

	return defaultValue;
}

const WCHAR* XmlNode::GetStringValue(const WCHAR* defaultValue)
{
	WCHAR* val = _node->first_node()->value();
	if (val)
		return val;

	return defaultValue;
}

XmlNode XmlNode::FindChild(const WCHAR* key)
{
	return XmlNode(_node->first_node(key));
}

// XmlNodeType = xml_node<WCHAR>로 되어 있어서 WCHAR를 key로 써도 문제 없다.
myVector<XmlNode> XmlNode::FindChildren(const WCHAR* key)
{
	myVector<XmlNode> nodes;

	xml_node<WCHAR>* node = _node->first_node(key);

	while (node)
	{
		nodes.push_back(XmlNode(node));
		node = node->next_sibling(key);
	}

	return nodes;
}


// ---------------------------
//			XmlParser
// ---------------------------
bool XmlParser::ParseFromFile(const WCHAR* path, OUT XmlNode& root)
{
	myVector<BYTE> bytes = FileUtils::ReadFile(path);
	_data = FileUtils::Convert(string(bytes.begin(), bytes.end()));

	if (_data.empty())
	{
		// TODO: log
		std::wcout << (L"Data Empty", path) << endl;;
		return false;
	}

	_document = myMakeShared<XmlDocumentType>();

	// parse<0>은 파싱 관련 아무 플래그도 설정하지 않은 것을 의미
	// RapidXML 라이브러리가 XML 문자열을 내부 트리 구조로 변환한다.
	_document->parse<0>(reinterpret_cast<WCHAR*>(&_data[0]));
	root = XmlNode(_document->first_node()); // 현재 TestDB.xml의 <GameDB> 부분(맨 처음)부터 시작이라고 보면 된다.
	return true;
}