#pragma once

#define TESTDLL_VERSION "LoadMemoryDLL full decompiled version 1.0"
#define TESTDLL_EXPORT extern "C" __declspec(dllexport)

TESTDLL_EXPORT char const* TestDLL_GetVersion();
TESTDLL_EXPORT unsigned int TestDLL_MessageBox(char const* title, char const* content, unsigned int type);
TESTDLL_EXPORT int TestDLL_Add(int a, int b);
