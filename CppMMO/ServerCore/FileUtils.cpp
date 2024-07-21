#include "pch.h"
#include "FileUtils.h"
#include <filesystem>
#include <fstream>

// ----------------------------
//			FileUtils
// ----------------------------

namespace fs = std::filesystem; // C++ 17�� �߰��� ���� �ý��� ���̺귯��

myVector<BYTE> FileUtils::ReadFile(const WCHAR* path)
{
	myVector<BYTE> ret;

	fs::path filePath{ path };

	try
	{
		// ���� ũ�⸦ ����
		const uint32_t fileSize = static_cast<uint32_t>(fs::file_size(filePath));
		ret.resize(fileSize);

		// ������ ���̳ʸ� ���� ����
		std::ifstream inputStream{ filePath, std::ios::binary };
		if (!inputStream)
		{
			// TODO: log�� ����
			std::wcerr << L"Failed to open file: " << path << std::endl;
			CRASH(L"Failed to open file");
		}

		// ���� ������ ���Ϳ� �о����
		inputStream.read(reinterpret_cast<char*>(&ret[0]), fileSize);
		if (!inputStream)
		{
			std::wcerr << L"Failed to read the entire file: " << path << std::endl;
			ret.clear(); // �Ϻθ� �о��� ��� ���͸� ����
			CRASH(L"Failed to read the entire file");
		}
	}
	catch (const fs::filesystem_error& e)
	{
		std::wcerr << L"Filesystem error: " << e.what() << std::endl;
		CRASH(L"Filesystem error");
	}
	catch (const std::exception& e)
	{
		std::wcerr << L"General error: " << e.what() << std::endl;
		CRASH(L"General error");
	}

	return ret;
}

myString FileUtils::Convert(string str)
{
	const int32 srcLen = static_cast<int32>(str.size());

	myString ret;
	if (srcLen == 0)
		return ret;

	const int32 retLen = ::MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<char*>(&str[0]), srcLen, NULL, 0);
	ret.resize(retLen);
	::MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<char*>(&str[0]), srcLen, &ret[0], retLen);

	return ret;
}
