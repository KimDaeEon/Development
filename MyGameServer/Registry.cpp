#include "Registry.h"

/// <summary>
/// CRegistry 생성자, mRootKey 와 mIsOpened 를 초기화.
/// </summary>
/// <param name=""></param>
CRegistry::CRegistry(VOID) {
	mRootKey	= NULL;	// 사용할 레지스트리 키를 저장하는 변수
	mIsOpened	= FALSE;	// 해달 레지스트리를 열었는지 체크하는 변수
}

CRegistry::~CRegistry(VOID) {
	
}

/// <summary>
/// 특정 키로 레지스트릴 처음 열 때에 사용.
/// </summary>
/// <param name="rootKey"></param>
/// <param name="subKey"></param>
/// <returns></returns>
BOOL CRegistry::Open(HKEY rootKey, LPCTSTR subKey) {
	
	// subkey 가 존재하는지 체크
	if (!subKey) 
		return FALSE;

	// 이미 열려 있는 경우 체크
	if (mIsOpened) 
		return FALSE;

	// 지정된 키를 열어서 mRootKey 변수에 저장
	if (RegOpenKey(rootKey, subKey, &mRootKey) != ERROR_SUCCESS)
		return FALSE;

	// 해당 키에 대해 열었으므로 TRUE 로 세팅
	mIsOpened = TRUE;

	return TRUE;
}

/// <summary>
/// 특정 키를 닫을 때에 사용.
/// </summary>
/// <param name=""></param>
/// <returns></returns>
BOOL CRegistry::Close(VOID) {
	
	// 이미 닫혀 있는지 체크
	if (RegCloseKey(mRootKey) != ERROR_SUCCESS)
		return FALSE;

	// 해당 키에 대해 닫았으므로 FALSE 로 세팅
	mIsOpened = FALSE;

	return TRUE;
}

/// <summary>
/// 키를 생성할 때에 사용.
/// </summary>
/// <param name="rootKey"></param>
/// <param name="subKey"></param>
/// <returns></returns>
BOOL CRegistry::CreateKey(HKEY rootKey, LPCTSTR subKey) {

	// rootKey 를 기준으로 생성할 subKey 를 입력받아 키를 생성
	if (!subKey)
		return FALSE;

	// 이미 열려있을 경우 실패
	if (mIsOpened)
		return FALSE;

	// 레지스트리 키를 생성하는 API 함수 호출
	if (RegCreateKey(rootKey, subKey, &mRootKey) != ERROR_SUCCESS)
		return FALSE;

	// Key 를 생성하면서 자동으로 열린 상태로 세팅
	mIsOpened = TRUE;

	return TRUE;
}

/// <summary>
/// 키를 삭제할 때에 사용.
/// </summary>
/// <param name="rootKey"></param>
/// <param name="subKey"></param>
/// <returns></returns>
BOOL CRegistry::DeleteKey(HKEY rootKey, LPCTSTR subKey) {

	if (!subKey)
		return FALSE;

	// 레지스트리 키를 삭제하는 API 함수를 호출
	if (RegDeleteKey(rootKey, subKey) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

/// <summary>
/// 키 값에 대해 값을 설정, TCAHR 형의 문자열 데이터를 레지스트리에 입력할 때에 사용.
/// </summary>
/// <param name="valueName"></param>
/// <param name="value"></param>
/// <returns></returns>
BOOL CRegistry::SetValue(LPCTSTR valueName, LPCTSTR value) {

	if (!valueName || !value) // valueName 또는 value 가 없으면 FALSE
		return FALSE;
	
	if (!mIsOpened)
		return FALSE;

	// REG_SZ 형으로 데이터를 입력
	if (RegSetValueEx(mRootKey, valueName, 0, REG_SZ, (BYTE*)value, (DWORD)_tcslen(value) * sizeof(TCHAR)) != ERROR)
		return FALSE;

	return TRUE;

}

/// <summary>
/// DWORD 형 value 값을 설정.
/// </summary>
/// <param name="valueName"></param>
/// <param name="value"></param>
/// <returns></returns>
BOOL CRegistry::SetValue(LPCTSTR valueName, DWORD value) {

	if (!valueName)
		return FALSE;

	if (!mIsOpened)
		return FALSE;

	// REG_DWORD 형태 value 값 저장
	if (RegSetValueEx(mRootKey, valueName, 0, REG_DWORD, (BYTE*)&value, sizeof(DWORD)) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

/// <summary>
/// MultiSize 형의 문자열을 사용할 때 쓰는 함수.
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

	// REG_MULTI_SZ 형 데이터를 입력
	if (RegSetValueEx(mRootKey, valueName, 0, REG_MULTI_SZ, (BYTE*)value, byteLength) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

/// <summary>
/// TCHAR 형의 문자열 데이터를 읽어오는 함수
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

	// 문자열 데이터를 bufferLength 에 저장된 길이만큼 읽어온다.
	if (RegQueryValueEx(mRootKey, valueName, 0, &valueType, (BYTE*)value, bufferLength) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

/// <summary>
/// DWORD 형 데이터를 읽어오는 함수
/// </summary>
/// <param name="valueName"></param>
/// <param name="value"></param>
/// <returns></returns>
BOOL CRegistry::GetValue(LPCTSTR valueName, LPDWORD value) {

	DWORD valueType = 0;

	// 버퍼의 크기를 가져온다.
	DWORD bufferLength = sizeof(DWORD);

	if (!valueName || !value)
		return FALSE;
	
	if (!mIsOpened)
		return FALSE;

	// TCHAR 형 문자열을 가져온 것과 같은 형태로 값을 가져온다.
	if (RegQueryValueEx(mRootKey, valueName, 0, &valueType, (BYTE*)value, &bufferLength) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

/// <summary>
/// 값을 삭제하는 함수
/// </summary>
/// <param name="valueName"></param>
/// <returns></returns>
BOOL CRegistry::DeleteValue(LPCTSTR valueName) {

	if (!valueName)
		return FALSE;

	if (!mIsOpened)
		return FALSE;

	// 레지스트리의 값을 삭제하는 API 함수 호출
	if (RegDeleteValue(mRootKey, valueName) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}