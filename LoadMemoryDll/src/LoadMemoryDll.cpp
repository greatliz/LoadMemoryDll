#include "LoadMemoryDll.h"

DWORD* GetEnclosingSectionHeader(unsigned int a1, int a2)
{
	unsigned int v2;
	unsigned int v3;
	DWORD* ret;
	int v5;
	unsigned int v6;

	v2 = *(unsigned __int16*)(a2 + 6);
	v3 = 0;
	ret = (DWORD*)(*(unsigned __int16*)(a2 + 20) + a2 + 24);
	if (!v2)
		return 0;

	while (1)
	{
		v5 = ret[2];
		if (!v5)
			v5 = ret[4];
		v6 = ret[3];
		if (a1 >= v6 && a1 < v5 + v6)
			break;
		++v3;
		ret += 10;
		if (v3 >= v2)
			return 0;
	}

	return ret;
}

int GetPtrFromRVA(int a1, int a2, int a3)
{
	int result;

	result = (int)GetEnclosingSectionHeader(a1, a2);

	if (result)
		result = a3 + a1 + *(DWORD*)(result + 20) - *(DWORD*)(result + 12);

	return result;
}

HMODULE WINAPI DD_LoadLibrary(
	LPVOID lpRawDll, 
	HANDLE hInstance
)
{
	BOOL(CALLBACK * pIsBadReadPtr)(const void*, UINT_PTR);
	WORD* v3;
	unsigned __int16 v4;
	WORD* v5;
	unsigned __int16 v6;
	unsigned int v7;
	int v8;
	unsigned int v9;
	DWORD v10;
	DWORD* v11;
	DWORD* v12;
	unsigned __int16 i;
	DWORD* v14;
	int v15;
	int* v16;
	const char* iatImportDLLName;
	HMODULE iatImportDLLHandle;
	int v19;
	int v20;
	char* v21;
	FARPROC* j;
	FARPROC v23;
	char* v24;
	const char* v26;
	char v27;
	int v29;
	int v30;
	DWORD* v31;
	char* v32;
	WORD* v33;
	int v34;
	int v35;
	char** v36;
	char* v37;
	HINSTANCE instance;
	DWORD* v39;
	size_t Size;
	size_t Sizea;
	DWORD* v42;
	struct _SYSTEM_INFO SystemInfo;
	char buffer_1[27];
	char v45[233];

	if (!lpRawDll)
		return 0;
	pIsBadReadPtr = IsBadReadPtr;
	if (IsBadReadPtr(lpRawDll, 0x40u))
		return 0;
	v3 = (WORD*)((char*)lpRawDll + *((DWORD*)lpRawDll + 15));
	if (pIsBadReadPtr(v3, 0xF8u))
		return 0;
	if (v3[10] != 224)
		return 0;
	if (v3[12] != 267)
		return 0;
	v4 = v3[3];
	if (!v4)
		return 0;
	v5 = v3 + 124;
	v6 = 0;
	v7 = (unsigned int)v3 + 40 * v4 - (DWORD)lpRawDll + 248;
	Size = v7;
	do
	{
		v8 = *(DWORD*)&v5[20 * v6 + 6];
		if (v7 < v8 + *(DWORD*)&v5[20 * v6 + 8])
			v7 = v8 + *(DWORD*)&v5[20 * v6 + 8];
		v9 = *(DWORD*)&v5[20 * v6 + 4] + v8;
		if (v7 < v9)
			v7 = v9;
		strcmp((LPCSTR)&v5[20 * v6++], ".rsrc");
	} while (v6 < v3[3]);

	//Create message box if is a trial version, now you can remove it manually.
	MessageBoxA(0, "This is a trial version of Load DLL From Memory. To remove this message box and some other restrictions, please purchase the full version from http://www.dll-decompiler.com.", "Trial Version", 0x40000u);

	GetSystemInfo(&SystemInfo);
	v10 = v7 / SystemInfo.dwPageSize;
	if (v7 % SystemInfo.dwPageSize)
		++v10;
	v11 = (DWORD*)VirtualAlloc(0, v10 * SystemInfo.dwPageSize, 0x1000u, 0x40u);
	v12 = v11;
	if (!v11)
		return 0;
	memmove(v11, lpRawDll, Size);
	for (i = 0; i < v3[3]; ++i)
		memmove(
			(char*)v12 + *(DWORD*)&v5[20 * i + 6],
			(char*)lpRawDll + *(DWORD*)&v5[20 * i + 10],
			*(DWORD*)&v5[20 * i + 8]);
	v14 = (DWORD*)((char*)v12 + v12[15]);
	v42 = v14;
	v15 = v14[32];
	if (!v15)
	{
	LABEL_39:
		VirtualFree(v12, 0, 0x8000u);
		return 0;
	}
	v16 = (int*)(DWORD*)((char*)v12 + v15);
	Sizea = (size_t)v16;
	while (v16[1] || v16[3])
	{
		iatImportDLLName = (char*)v12 + v16[3];
		iatImportDLLHandle = GetModuleHandleA(iatImportDLLName);
		if (!iatImportDLLHandle)
		{
			iatImportDLLHandle = LoadLibraryA(iatImportDLLName);
			if (!iatImportDLLHandle)
			{
				strcpy(buffer_1, "Can not find required DLL ");
				memset(v45, 0, 0xE8u);
				v45[232] = 0;
				v24 = buffer_1;
				if (buffer_1[0])
				{
					while (*++v24)
						;
				}
				v26 = iatImportDLLName;
				if (*iatImportDLLName)
				{
					do
					{
						if (v24 - buffer_1 >= 130)
							break;
						v27 = v26[1];
						*v24++ = *v26++;
					} while (v27);
				}
				*v24 = 0;
				strcat(buffer_1, ". If this is a MFC DLL, then you need to create a MFC project.");
				MessageBoxA(0, buffer_1, "Error", 0);
				goto LABEL_39;
			}
		}
		v19 = *v16;
		if (!*v16)
			v19 = v16[4];
		v20 = *(DWORD*)((char*)v12 + v19);
		v21 = (char*)v12 + v19;
		for (j = (FARPROC*)((char*)v12 + v16[4]); v20; ++j)
		{
			v23 = v20 >= 0 ? GetProcAddress(iatImportDLLHandle, (LPCSTR)v12 + v20 + 2) : GetProcAddress(iatImportDLLHandle, (LPCSTR)(unsigned __int16)v20);
			*j = v23;
			if (!j)
				goto LABEL_39;
			v20 = *((DWORD*)v21 + 1);
			v21 += 4;
		}
		v14 = v42;
		Sizea += 20;
		v16 = (int*)Sizea;
	}
	v29 = v14[40];
	if (v29)
	{
		v30 = *(DWORD*)((char*)v12 + v29);
		v31 = (DWORD*)((char*)v12 + v29);
		v32 = (char*)v12 + v14[20];
		if (v30)
		{
			do
			{
				v33 = (WORD*)(v31 + 2);
				if (v31 + 2 < (DWORD*)((char*)v31 + v31[1]))
				{
					do
					{
						if ((*v33 & 0xF000) == 12288)
						{
							v34 = *v31 + (*v33 & 0xFFF);
							v35 = *(DWORD*)((char*)v12 + v34) - v14[13];
							v36 = (char**)((char*)v12 + v34);
							v37 = (char*)v12 + v35;
							if (v37 >= (char*)v12 && v37 < v32)
								*v36 = v37;
						}
						else if ((*v33 & 0xF000) != 0)
						{
							goto LABEL_39;
						}
						++v33;
					} while (v33 < (WORD*)((char*)v31 + v31[1]));
				}
				v31 = (DWORD*)((char*)v31 + v31[1]);
			} while (*v31);
		}
	}
	instance = (HINSTANCE)hInstance;
	v39 = (DWORD*)((char*)v12 + v14[10]);
	if (!hInstance)
		instance = (HINSTANCE)v12;
	((void(__stdcall*)(HINSTANCE, int, DWORD))v39)(instance, 1, 0);
	return (HMODULE)v12;
}

FARPROC WINAPI DD_GetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
	int v8;
	DWORD* v9;
	unsigned __int16* v10;
	DWORD* v11;
	DWORD* v12;
	unsigned int v13;
	unsigned int v14;
	DWORD* v15;
	bool v16;
	FARPROC result;
	const char* v18;
	unsigned int v19;
	DWORD* i;
	unsigned int v21;
	DWORD* v22;
	DWORD* v23;
	unsigned __int16* v24;

	if (!(int)hModule)
		return 0;
	v2 = *(DWORD*)((int)hModule + 60);
	v3 = v2 + (int)hModule;
	if (*(WORD*)(v2 + (int)hModule + 20) != 224)
		return 0;
	if (*(WORD*)(v3 + 24) != 267)
		return 0;
	v4 = *(DWORD*)(v3 + 120);
	if (!GetEnclosingSectionHeader(v4, v2 + (int)hModule))
		return 0;
	GetPtrFromRVA(v4, v3, (int)hModule);
	v5 = *(DWORD*)(v3 + 120);
	if (!v5)
		return 0;
	v6 = *(DWORD*)(v5 + (int)hModule + 36);
	v7 = *(DWORD*)(v5 + (int)hModule + 32);
	v8 = *(DWORD*)(v5 + (int)hModule + 28);
	v9 = (DWORD*)((int)hModule + v5);
	v10 = (unsigned __int16*)((int)hModule + v6);
	v11 = (DWORD*)((int)hModule + v7);
	v12 = (DWORD*)((int)hModule + v8);
	v24 = v10;
	v23 = v11;
	v22 = v12;
	if ((unsigned int)lpProcName >> 16)
	{
		v13 = 0;
		v21 = 0;
		while (!*v12)
		{
		LABEL_14:
			v21 = ++v13;
			if (v13 > v9[5] - 1)
				return 0;
		}
		v14 = 0;
		v15 = v11;
		while (1)
		{
			if (*v10 == v13)
			{
				v16 = strcmp((LPCSTR)(*v15 + (int)hModule), lpProcName) == 0;
				v12 = v22;
				v13 = v21;
				if (v16)
					break;
			}
			++v14;
			++v10;
			++v15;
			if (v14 > v9[6] - 1)
			{
				v11 = v23;
				v10 = v24;
				goto LABEL_14;
			}
		}
		result = FARPROC((int)hModule + v22[v21]);
	}
	else
	{
		v18 = (const char*)v9[4];
		v19 = 0;
		for (i = v12; lpProcName != v18 || !*i; ++i)
		{
			++v19;
			++v18;
			if (v19 > v9[5] - 1)
				return 0;
		}
		result = FARPROC((int)hModule + v12[v19]);
	}
	return result;
}

BOOL WINAPI DD_FreeLibrary(HMODULE hModule)
{
	if (!hModule)
		return FALSE;

	((void(CALLBACK*)(LPVOID, DWORD, DWORD))((char*)hModule
		+ *(DWORD*)((char*)hModule + *((DWORD*)hModule + 15) + 40)))(
			hModule,
			0,
			0);

	VirtualFree(hModule, 0, 0x8000u);

	return TRUE;
}