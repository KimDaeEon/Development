#include "stdafx.h"
#include "IniFile.h"
// IniFile 에서 데이터를 가져올 때에 문자열로 가져오는 것을 기본으로 한다. 그래서 중간 중간 형변환 과정이 있다. 이것이 Registry 와의 차이점이다.


CIniFile::CIniFile(VOID) 
{
	ZeroMemory(mFileName, sizeof(mFileName));
}

CIniFile::~CIniFile(VOID) 
{

}

/// <summary>
/// Ini 파일을 오픈하는 함수, 파라미터로 TCHAR 형을 받는다.
/// </summary>
/// <param name="fileName"></param>
/// <returns></returns>
BOOL CIniFile::Open(LPCTSTR fileName) 
{
	// 파일 이름이 없는 경우 실패
	if (!fileName)
		return FALSE;

	// 미리 정의 되어 있는 MAX_PATH 를 이용해서 복사
	_tcsncpy(mFileName, fileName, MAX_PATH);

	return TRUE;
}

BOOL CIniFile::Close(VOID) 
{
	// TODO: 사실 아무것도 안하는데 왜 Close 라고 이름을 붙이셨을까.. 나중에 무슨 내용을 채우려 하신 것인가? 일단 ZeroMemory 추가
	ZeroMemory(mFileName, sizeof(mFileName));
	return TRUE;
}

/// <summary>
/// DWORD 형 값을 가져오는 함수
/// </summary>
/// <param name="keyName"></param>
/// <param name="valueName"></param>
/// <param name="value"></param>
/// <returns></returns>
BOOL CIniFile::GetValue(LPCTSTR keyName, LPCTSTR valueName, LPDWORD value) 
{
	// 하나라도 입력 안된 패러미터가 있다면 FALSE
	if (!keyName || !valueName || !value)
		return FALSE;

	*value = GetPrivateProfileInt(keyName, valueName, 0, mFileName);

	return TRUE;
}

/// <summary>
/// FLOAT 형 값을 가져오는 함수
/// </summary>
/// <param name="keyName"></param>
/// <param name="valueName"></param>
/// <param name="value"></param>
/// <returns></returns>
BOOL CIniFile::GetValue(LPCTSTR keyName, LPCTSTR valueName, FLOAT* value)
{
	if (!keyName || !valueName || !value)
		return FALSE;
	
	TCHAR t_array[16] = { 0, };

	// FLOAT 형을 따로 가져오는 함수가 없기 때문에 일단 문자열 형태로 가져와서 변환
	GetPrivateProfileString(keyName, valueName, _T(""), t_array, 16, mFileName);

	*value = (FLOAT)_tstof(t_array);

	return TRUE;
}

/// <summary>
/// 바이너리 데이터를 가져오는 함수, 문자열도 가져올 수 있다.
/// </summary>
/// <param name="keyName"></param>
/// <param name="valueName"></param>
/// <param name="value"></param>
/// <param name="bufferLength"></param>
/// <returns></returns>
BOOL CIniFile::GetValue(LPCTSTR keyName, LPCTSTR valueName, LPTSTR value, LPDWORD bufferLength) 
{
	if (!keyName || !valueName || !value || !bufferLength)
		return FALSE;

	*bufferLength = GetPrivateProfileString(keyName, valueName, _T(""), value, *bufferLength, mFileName);

	return TRUE;
}

/// <summary>
/// DWORD 형 값을 입력할 때에 사용.
/// </summary>
/// <param name="keyName"></param>
/// <param name="valueName"></param>
/// <param name="value"></param>
/// <returns></returns>
BOOL CIniFile::SetValue(LPCTSTR keyName, LPCTSTR valueName, DWORD value) 
{
	if (!keyName || !valueName)
		return FALSE;

	// 형 변환 함수를 이용해서 DWORD 값을 TCHAR 배열로 변환
	TCHAR t_array[16] = { 0, };
	_itot(value, t_array, 10); // 끝에 10은 10 진법이란 뜻이다. 10 진법에 따라 숫자를 문자열로 기록하는 것

	WritePrivateProfileString(keyName, valueName, t_array, mFileName);
	
	return TRUE;
}

/// <summary>
/// 일반 문자열 데이터를 입력할 때에 사용.
/// </summary>
/// <param name="keyName"></param>
/// <param name="valueName"></param>
/// <param name="value"></param>
/// <returns></returns>
BOOL CIniFile::SetValue(LPCTSTR keyName, LPCTSTR valueName, LPCTSTR value) 
{
	if (!keyName || !valueName || !value)
		return FALSE;

	WritePrivateProfileString(keyName, valueName, value, mFileName);

	return TRUE;
}

/// <summary>
/// FLOAT 형 데이터를 입력할 때에 사용
/// </summary>
/// <param name="keyName"></param>
/// <param name="valueName"></param>
/// <param name="value"></param>
/// <returns></returns>
BOOL CIniFile::SetValue(LPCTSTR keyName, LPCTSTR valueName, FLOAT value) {

	if (!keyName || !valueName)
		return FALSE;

	TCHAR t_array[16] = { 0, };
	_stprintf(t_array, _T("%f"), value);

	WritePrivateProfileString(keyName, valueName, t_array, mFileName);

	return TRUE;
}