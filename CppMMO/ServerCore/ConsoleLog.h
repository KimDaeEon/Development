#pragma once

// ------------------------
//		  ConsoleLog
// ------------------------

// 단순히 색깔을 바꿔서 콘솔로그를 찍는 클래스이다.
enum class LogColor
{
	BLACK,
	WHITE,
	RED,
	GREEN,
	BLUE,
	YELLOW,
};

class ConsoleLog
{
	enum { BUFFER_SIZE = 4096 };

public:
	ConsoleLog();
	~ConsoleLog();

public:
	void		WriteStdOut(LogColor color, const WCHAR* str, ...);
	void		WriteStdErr(LogColor color, const WCHAR* str, ...);

protected:
	void		SetColor(bool stdOut, LogColor color);

private:
	HANDLE		_stdOut;
	HANDLE		_stdErr;
};