#pragma once

/*******************************************************************************
  Date        : Jan 2014
  Description :	Load Dll From Memroy header file
  Copyright   : 2014 Dll-Decompiler.com, All rights reserved.
  Website     : http://www.dll-decompiler.com

  This source code file was wrote by greatliz author, github: https://github.com/greatliz/LoadMemoryDll/
*******************************************************************************/

#ifndef __LOAD_MEMORY_DLL_H__
#define __LOAD_MEMORY_DLL_H__

/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif

#include <WTypes.h>

	/*
	** Load a DLL from memory. This function does not check the IMAGE_DOS_SIGNATURE and the IMAGE_NT_SIGNATURE,
	** so you can erase the 'MZ'and 'PE' signature in your DLL file.
	*/
	HMODULE WINAPI DD_LoadLibrary(
		LPVOID lpRawDll, /* Raw data of the DLL file. */
		HANDLE hInstance /* The handle of the module which contains the DLL resources, sometimes it is necessary.
							If the DLL contains necessary resources, then you need to export the resources, and
							add them into the project. */
	);

	/*
	** Retrieves the address of an exported function or variable from the DLL.
	*/
	FARPROC WINAPI DD_GetProcAddress(
		HMODULE hModule,  /* A handle to the DLL module, The DD_LoadLibrary function returns this handle. */
		LPCSTR lpProcName /* The function or variable name, or the function's ordinal value. If this parameter is
							 an ordinal value, it must be in the low-order word; the high-order word must be zero. */
	);

	/*
	** Frees the loaded library (DLL) module.
	*/
	BOOL WINAPI DD_FreeLibrary(
		HMODULE hModule /* A handle to the DLL module, The DD_LoadLibrary function returns this handle. */
	);

#ifdef __cplusplus
}
#endif

#endif //__LOAD_MEMORY_DLL_H__