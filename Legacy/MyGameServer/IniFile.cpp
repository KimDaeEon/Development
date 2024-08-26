#include "stdafx.h"
#include "IniFile.h"
// IniFile ���� �����͸� ������ ���� ���ڿ��� �������� ���� �⺻���� �Ѵ�. �׷��� �߰� �߰� ����ȯ ������ �ִ�. �̰��� Registry ���� �������̴�.


CIniFile::CIniFile(VOID) 
{
	ZeroMemory(mFileName, sizeof(mFileName));
}

CIniFile::~CIniFile(VOID) 
{

}

/// <summary>
/// Ini ������ �����ϴ� �Լ�, �Ķ���ͷ� TCHAR ���� �޴´�.
/// </summary>
/// <param name="fileName"></param>
/// <returns></returns>
BOOL CIniFile::Open(LPCTSTR fileName) 
{
	// ���� �̸��� ���� ��� ����
	if (!fileName)
		return FALSE;

	// �̸� ���� �Ǿ� �ִ� MAX_PATH �� �̿��ؼ� ����
	_tcsncpy(mFileName, fileName, MAX_PATH);

	return TRUE;
}

BOOL CIniFile::Close(VOID) 
{
	// TODO: ��� �ƹ��͵� ���ϴµ� �� Close ��� �̸��� ���̼�����.. ���߿� ���� ������ ä��� �Ͻ� ���ΰ�? �ϴ� ZeroMemory �߰�
	ZeroMemory(mFileName, sizeof(mFileName));
	return TRUE;
}

/// <summary>
/// DWORD �� ���� �������� �Լ�
/// </summary>
/// <param name="keyName"></param>
/// <param name="valueName"></param>
/// <param name="value"></param>
/// <returns></returns>
BOOL CIniFile::GetValue(LPCTSTR keyName, LPCTSTR valueName, LPDWORD value) 
{
	// �ϳ��� �Է� �ȵ� �з����Ͱ� �ִٸ� FALSE
	if (!keyName || !valueName || !value)
		return FALSE;

	*value = GetPrivateProfileInt(keyName, valueName, 0, mFileName);

	return TRUE;
}

/// <summary>
/// FLOAT �� ���� �������� �Լ�
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

	// FLOAT ���� ���� �������� �Լ��� ���� ������ �ϴ� ���ڿ� ���·� �����ͼ� ��ȯ
	GetPrivateProfileString(keyName, valueName, _T(""), t_array, 16, mFileName);

	*value = (FLOAT)_tstof(t_array);

	return TRUE;
}

/// <summary>
/// ���̳ʸ� �����͸� �������� �Լ�, ���ڿ��� ������ �� �ִ�.
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
/// DWORD �� ���� �Է��� ���� ���.
/// </summary>
/// <param name="keyName"></param>
/// <param name="valueName"></param>
/// <param name="value"></param>
/// <returns></returns>
BOOL CIniFile::SetValue(LPCTSTR keyName, LPCTSTR valueName, DWORD value) 
{
	if (!keyName || !valueName)
		return FALSE;

	// �� ��ȯ �Լ��� �̿��ؼ� DWORD ���� TCHAR �迭�� ��ȯ
	TCHAR t_array[16] = { 0, };
	_itot(value, t_array, 10); // ���� 10�� 10 �����̶� ���̴�. 10 ������ ���� ���ڸ� ���ڿ��� ����ϴ� ��

	WritePrivateProfileString(keyName, valueName, t_array, mFileName);
	
	return TRUE;
}

/// <summary>
/// �Ϲ� ���ڿ� �����͸� �Է��� ���� ���.
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
/// FLOAT �� �����͸� �Է��� ���� ���
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