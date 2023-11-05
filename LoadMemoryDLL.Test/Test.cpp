#include "LoadMemoryDll.h"
#include <stdio.h>
#include <sys/stat.h>

#ifdef _DEBUG
	#define TESTDLL_PATH "../Debug/LoadMemoryDll.TestDLL.dll"
#else
	#define TESTDLL_PATH "../Release/LoadMemoryDll.TestDLL.dll"
#endif

int main(int argc, char* argv[])
{


	FILE* fp = fopen(TESTDLL_PATH, "rb");
	struct stat statbuf;
	stat(TESTDLL_PATH, &statbuf);
	size_t dll_size = statbuf.st_size;

	char* dll_buffer = new char[dll_size];
	fread(dll_buffer, sizeof(char), dll_size, fp);

	fclose(fp);

	HMODULE hDLL = DD_LoadLibrary(dll_buffer, GetModuleHandleA(NULL));
	if (!hDLL) return -1;

	typedef char const* (*fnTestDLL_GetVersion)();
	typedef unsigned int (*fnTestDLL_MessageBox)(char const* title, char const* content, unsigned int type);
	typedef int (*fnTestDLL_Add)(int a, int b);

	fnTestDLL_GetVersion pTestDLL_GetVersion = (fnTestDLL_GetVersion)DD_GetProcAddress(hDLL, "TestDLL_GetVersion");
	fnTestDLL_MessageBox pTestDLL_MessageBox = (fnTestDLL_MessageBox)DD_GetProcAddress(hDLL, "TestDLL_MessageBox");
	fnTestDLL_Add pTestDLL_Add = (fnTestDLL_Add)DD_GetProcAddress(hDLL, "TestDLL_Add");

	if (!pTestDLL_GetVersion || !pTestDLL_MessageBox || !pTestDLL_Add) return -1;

	printf("TestDLL_GetVersion returned: %s\n", pTestDLL_GetVersion());
	printf("Called TestDLL_MessageBox with (%cMessage Box From DLL%c, %cMessage Box From DLL%c, MB_ICONINFORMATION | MB_YESNO).\n", '"', '"', '"', '"');
	printf("TestDLL_MessageBox returned: %d\n", pTestDLL_MessageBox("Message Box From DLL", "Message Box From DLL", MB_ICONINFORMATION | MB_YESNO));
	printf("TestDLL_Add with (1, 9) returned: %d\n", pTestDLL_Add(1, 9));

	DD_FreeLibrary(hDLL);

	return 0;
}