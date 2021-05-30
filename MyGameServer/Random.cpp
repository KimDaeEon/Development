#include "Random.h"

CRandom::CRandom(VOID) {

}

CRandom::~CRandom(VOID) {

}

// 기본이 랜덤을 돌릴 때에 쓰는 시드값 입력
BOOL CRandom::Init(UINT seed) {
	mSeed = seed;
	
	return TRUE;
}

/// <summary>
/// 간단하게 랜덤 값을 받아오는 함수, 숫자 크기가 ComplexRandom 보다 작다.
/// </summary>
/// <param name=""></param>
/// <returns></returns>
INT CRandom::Rand(VOID) { 
	return ( ((mSeed = mSeed * 214013L + 2531011L) >> 16) & 0x7FFF);
}


/// <summary>
/// Rand(VOID) 보다 복잡하게 랜덤 값을 받아오는 함수
/// </summary>
/// <param name=""></param>
/// <returns></returns>
INT CRandom::ComplexRandom(VOID) {
	UINT next = mSeed;
	INT result;

	next *= 1103515245;
	next += 12345;
	result = (UINT)(next / 65536) % 2048;

	next *= 1103515245;
	next += 12345;
	result <<= 10;
	result ^= (UINT)(next / 65536) % 1024;

	next *= 1103515245;
	next += 12345;
	result <<= 10;
	result ^= (UINT)(next / 65536) % 1024;

	mSeed = next;
	return result;
}