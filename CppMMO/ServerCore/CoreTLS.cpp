#include "pch.h"
#include "CoreTLS.h"

// 그럴 일은 웬만하면 없겠지만 LThreadId는 16bit 범위를 벗어나면 안된다.
// WriteLock에서 ThreadId를 16bit 범위 내라고 가정하기 때문이다.
thread_local uint32 LThreadId = 0;