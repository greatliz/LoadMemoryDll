#include "TestDLL.h"
#include <Windows.h>

TESTDLL_EXPORT char const* TestDLL_GetVersion()
{
	return TESTDLL_VERSION;
}

TESTDLL_EXPORT unsigned int TestDLL_MessageBox(char const* title, char const* content, unsigned int type)
{
	return MessageBoxA(NULL, title, content, type);
}

TESTDLL_EXPORT int TestDLL_Add(int a, int b)
{
	return a + b;
}
