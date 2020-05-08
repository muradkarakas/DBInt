#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <windows.h>

__declspec(dllimport) BOOL IsOracleClientDriverLoaded;
__declspec(dllimport) BOOL IsPostgreSqLClientDriverLoaded;

