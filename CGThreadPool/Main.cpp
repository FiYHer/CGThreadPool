
#include "CGThreadPool.h"
using namespace cgtp;

bool g_bool = true;

void Func(void* pBuffer)
{
	static int nIndex = 100;
	printf("nIndex is :%d  pBuffer is :%s  TID:%d \n",
		nIndex++, (char*)pBuffer, GetCurrentThreadId());

}

int main(int argc, char* argv[])
{
	HANDLE hEvent = CreateEventA(0, false, false, 0);
	CGThreadPool Test;
	Test.Run_Thread_Func(Func, (void*)"Buffer is This", 1, hEvent);
	WaitForSingleObject(hEvent, INFINITE);
	return 0;
}
