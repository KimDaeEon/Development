#pragma once

// MFC ������Ʈ�� �ƴϸ� �Ʒ��� #define�� ���� ����ð��� ������ �� �ִٰ� �Ѵ�.
#define WIN32_LEAN_AND_MEAN

// �Ʒ�ó���ϸ� ��Ŀ���� ���̺귯���� �߰� ���ص� ���̺귯���� �߰��ȴ�.
// ��� �Է��� ���� \\ <- �̰� 2���� �ϴ°� ���� ����.
#ifdef _DEBUG
#pragma comment(lib, "Debug\\ServerCore.lib") 
#else
#pragma comment(lib, "Release\\ServerCore.lib")
#endif

#include "CorePch.h"