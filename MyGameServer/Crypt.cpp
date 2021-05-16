#include"Crypt.h"

const INT c1 = 52845;
const INT c2 = 22719;
const INT KEY = 72957;

/// <summary>
/// 암호화 함수이다.
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
		destination[i] = source[i] ^ key >> 8; // 연산이 좀 복잡해 보이는데, 요점은 같은 key 값으로 XOR 연산을 2번하면 원래 값으로 돌아온다는 것이다. 
		// >>8 이런 것도 사실 큰 의미는 없고, 암호화 변수를 추가한 것이라 보면 된다.
		key = (destination[i] + key) * c1 + c2; 
	}

	return TRUE;
}

/// <summary>
/// 복호화 함수이다.
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
		previousBlock = source[i]; // Encrypt 에서 이 값을 XOR 연산에 사용하는 key 로 이용했으므로 일단 저장해둬야 한다.
		destination[i] = source[i] ^ key >> 8; // 이전에 XOR 했었던 값으로 다시 XOR 연산을 한다. 그러면 원래 값으로 돌아온다.
		key = (previousBlock + key) * c1 + c2;
	}

	return TRUE;
}