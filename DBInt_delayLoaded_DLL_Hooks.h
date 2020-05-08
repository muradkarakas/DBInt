#pragma once

#define DELAYIMP_INSECURE_WRITABLE_HOOKS
#include "..\SodiumShared\SodiumShared.h"
#include <delayimp.h>
#include "DBInt_delayLoaded_DLL_FuncImps.h"
#include "db-interface.h"


// delay-loaded DLLs implementation functions
FARPROC WINAPI		delayedDllFailHook(unsigned dliNotify, PDelayLoadInfo pdli);
FARPROC WINAPI		delayHook(unsigned dliNotify, PDelayLoadInfo pdli);


