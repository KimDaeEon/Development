#include "pch.h"
#include "FileUtils.h"
#include <filesystem>
#include <fstream>

// ----------------------------
//			FileUtils
// ----------------------------

namespace fs = std::filesystem; // C++ 17에 추가된 파일 시스템 라이브러리

myVector<BYTE> FileUtils::ReadFile(const WCHAR* path)
{
	myVector<BYTE> ret;

	fs::path filePath{ path };

	try
	{
		// 파일 크기를 얻음
		const uint32_t fileSize = static_cast<uint32_t>(fs::file_size(filePath));
		ret.resize(fileSize);

		// 파일을 바이너리 모드로 열기
		std::ifstream inputStream{ filePath, std::ios::binary };
		if (!inputStream)
		{
			// TODO: log로 변경
			std::wcerr << L"Failed to open file: " << path << std::endl;
			CRASH(L"Failed to open file");
		}

		// 파일 내용을 벡터에 읽어오기
		inputStream.read(reinterpret_cast<char*>(&ret[0]), fileSize);
		if (!inputStream)
		{
			std::wcerr << L"Failed to read the entire file: " << path << std::endl;
			ret.clear(); // 일부만 읽었을 경우 벡터를 비우기
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
