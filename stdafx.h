// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

	__declspec(dllimport) BOOL IsOracleClientDriverLoaded;
	__declspec(dllimport) BOOL IsPostgreSqLClientDriverLoaded;

#ifdef __cplusplus
}
#endif
