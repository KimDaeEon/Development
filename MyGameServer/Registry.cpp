#include "Registry.h"

/// <summary>
/// CRegistry ������, mRootKey �� mIsOpened �� �ʱ�ȭ.
/// </summary>
/// <param name=""></param>
CRegistry::CRegistry(VOID) {
	mRootKey	= NULL;	// ����� ������Ʈ�� Ű�� �����ϴ� ����
	mIsOpened	= FALSE;	// �ش� ������Ʈ���� �������� üũ�ϴ� ����
}

CRegistry::~CRegistry(VOID) {
	
}

/// <summary>
/// Ư�� Ű�� ������Ʈ�� ó�� �� ���� ���.
/// </summary>
/// <param name="rootKey"></param>
/// <param name="subKey"></param>
/// <returns></returns>
BOOL CRegistry::Open(HKEY rootKey, LPCTSTR subKey) {
	
	// subkey �� �����ϴ��� üũ
	if (!subKey) 
		return FALSE;

	// �̹� ���� �ִ� ��� üũ
	if (mIsOpened) 
		return FALSE;

	// ������ Ű�� ��� mRootKey ������ ����
	if (RegOpenKey(rootKey, subKey, &mRootKey) != ERROR_SUCCESS)
		return FALSE;

	// �ش� Ű�� ���� �������Ƿ� TRUE �� ����
	mIsOpened = TRUE;

	return TRUE;
}

/// <summary>
/// Ư�� Ű�� ���� ���� ���.
/// </summary>
/// <param name=""></param>
/// <returns></returns>
BOOL CRegistry::Close(VOID) {
	
	// �̹� ���� �ִ��� üũ
	if (RegCloseKey(mRootKey) != ERROR_SUCCESS)
		return FALSE;

	// �ش� Ű�� ���� �ݾ����Ƿ� FALSE �� ����
	mIsOpened = FALSE;

	return TRUE;
}

/// <summary>
/// Ű�� ������ ���� ���.
/// </summary>
/// <param name="rootKey"></param>
/// <param name="subKey"></param>
/// <returns></returns>
BOOL CRegistry::CreateKey(HKEY rootKey, LPCTSTR subKey) {

	// rootKey �� �������� ������ subKey �� �Է¹޾� Ű�� ����
	if (!subKey)
		return FALSE;

	// �̹� �������� ��� ����
	if (mIsOpened)
		return FALSE;

	// ������Ʈ�� Ű�� �����ϴ� API �Լ� ȣ��
	if (RegCreateKey(rootKey, subKey, &mRootKey) != ERROR_SUCCESS)
		return FALSE;

	// Key �� �����ϸ鼭 �ڵ����� ���� ���·� ����
	mIsOpened = TRUE;

	return TRUE;
}

/// <summary>
/// Ű�� ������ ���� ���.
/// </summary>
/// <param name="rootKey"></param>
/// <param name="subKey"></param>
/// <returns></returns>
BOOL CRegistry::DeleteKey(HKEY rootKey, LPCTSTR subKey) {

	if (!subKey)
		return FALSE;

	// ������Ʈ�� Ű�� �����ϴ� API �Լ��� ȣ��
	if (RegDeleteKey(rootKey, subKey) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

/// <summary>
/// Ű ���� ���� ���� ����, TCAHR ���� ���ڿ� �����͸� ������Ʈ���� �Է��� ���� ���.
/// </summary>
/// <param name="valueName"></param>
/// <param name="value"></param>
/// <returns></returns>
BOOL CRegistry::SetValue(LPCTSTR valueName, LPCTSTR value) {

	if (!valueName || !value) // valueName �Ǵ� value �� ������ FALSE
		return FALSE;
	
	if (!mIsOpened)
		return FALSE;

	// REG_SZ ������ �����͸� �Է�
	if (RegSetValueEx(mRootKey, valueName, 0, REG_SZ, (BYTE*)value, (DWORD)_tcslen(value) * sizeof(TCHAR)) != ERROR)
		return FALSE;

	return TRUE;

}

/// <summary>
/// DWORD �� value ���� ����.
/// </summary>
/// <param name="valueName"></param>
/// <param name="value"></param>
/// <returns></returns>
BOOL CRegistry::SetValue(LPCTSTR valueName, DWORD value) {

	if (!valueName)
		return FALSE;

	if (!mIsOpened)
		return FALSE;

	// REG_DWORD ���� value �� ����
	if (RegSetValueEx(mRootKey, valueName, 0, REG_DWORD, (BYTE*)&value, sizeof(DWORD)) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

/// <summary>
/// MultiSize ���� ���ڿ��� ����� �� ���� �Լ�.
/// </summary>
/// <param name="valueName"></param>
/// <param name="value"></param>
/// <param name="byteLength"></param>
/// <returns></returns>
BOOL CRegistry::SetValueFromMultiSize(LPCTSTR valueName, LPCTSTR value, DWORD byteLength) {

	if (!valueName || !value)
		return FALSE;

	if (!mIsOpened)
		return FALSE;

	// REG_MULTI_SZ �� �����͸� �Է�
	if (RegSetValueEx(mRootKey, valueName, 0, REG_MULTI_SZ, (BYTE*)value, byteLength) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

/// <summary>
/// TCHAR ���� ���ڿ� �����͸� �о���� �Լ�
/// </summary>
/// <param name="valueName"></param>
/// <param name="value"></param>
/// <param name="bufferLength"></param>
/// <returns></returns>
BOOL CRegistry::GetValue(LPCTSTR valueName, LPCTSTR value, LPDWORD bufferLength) {

	DWORD valueType = 0;

	if (!valueName || !value || !bufferLength)
		return FALSE;

	if (!mIsOpened)
		return FALSE;

	// ���ڿ� �����͸� bufferLength �� ����� ���̸�ŭ �о�´�.
	if (RegQueryValueEx(mRootKey, valueName, 0, &valueType, (BYTE*)value, bufferLength) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

/// <summary>
/// DWORD �� �����͸� �о���� �Լ�
/// </summary>
/// <param name="valueName"></param>
/// <param name="value"></param>
/// <returns></returns>
BOOL CRegistry::GetValue(LPCTSTR valueName, LPDWORD value) {

	DWORD valueType = 0;

	// ������ ũ�⸦ �����´�.
	DWORD bufferLength = sizeof(DWORD);

	if (!valueName || !value)
		return FALSE;
	
	if (!mIsOpened)
		return FALSE;

	// TCHAR �� ���ڿ��� ������ �Ͱ� ���� ���·� ���� �����´�.
	if (RegQueryValueEx(mRootKey, valueName, 0, &valueType, (BYTE*)value, &bufferLength) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

/// <summary>
/// ���� �����ϴ� �Լ�
/// </summary>
/// <param name="valueName"></param>
/// <returns></returns>
BOOL CRegistry::DeleteValue(LPCTSTR valueName) {

	if (!valueName)
		return FALSE;

	if (!mIsOpened)
		return FALSE;

	// ������Ʈ���� ���� �����ϴ� API �Լ� ȣ��
	if (RegDeleteValue(mRootKey, valueName) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}