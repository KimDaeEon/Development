#include"Crypt.h"

const INT c1 = 52845;
const INT c2 = 22719;
const INT KEY = 72957;

/// <summary>
/// ��ȣȭ �Լ��̴�.
/// </summary>
/// <param name="source"></param>
/// <param name="destination"></param>
/// <param name="length"></param>
/// <returns></returns>
BOOL CCrypt::Encrypt(BYTE* source, BYTE* destination, DWORD length) {
	DWORD i;
	INT key = KEY;

	if (!source || !destination || length <= 0)
		return FALSE;

	for (i = 0; i < length; i++) {
		destination[i] = source[i] ^ key >> 8; // ������ �� ������ ���̴µ�, ������ ���� key ������ XOR ������ 2���ϸ� ���� ������ ���ƿ´ٴ� ���̴�. 
		// >>8 �̷� �͵� ��� ū �ǹ̴� ����, ��ȣȭ ������ �߰��� ���̶� ���� �ȴ�.
		key = (destination[i] + key) * c1 + c2; 
	}

	return TRUE;
}

/// <summary>
/// ��ȣȭ �Լ��̴�.
/// </summary>
/// <param name="source"></param>
/// <param name="destination"></param>
/// <param name="length"></param>
/// <returns></returns>
BOOL CCrypt::Decrypt(BYTE* source, BYTE* destination, DWORD length) {
	DWORD i;
	BYTE previousBlock;
	INT key = KEY;

	if (!source || !destination || length <= 0)
		return FALSE;

	for (i = 0; i < length; i++) {
		previousBlock = source[i]; // Encrypt ���� �� ���� XOR ���꿡 ����ϴ� key �� �̿������Ƿ� �ϴ� �����ص־� �Ѵ�.
		destination[i] = source[i] ^ key >> 8; // ������ XOR �߾��� ������ �ٽ� XOR ������ �Ѵ�. �׷��� ���� ������ ���ƿ´�.
		key = (previousBlock + key) * c1 + c2;
	}

	return TRUE;
}