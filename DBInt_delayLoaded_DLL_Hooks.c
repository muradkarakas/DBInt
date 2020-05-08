#include "pch.h"

#include "DBInt_delayLoaded_DLL_Hooks.h"

FARPROC WINAPI 
delayedDllFailHook(
	unsigned dliNotify, 
	PDelayLoadInfo pdli
)
{
	switch (dliNotify) {
		case dliFailLoadLib: {
			mkCoreDebug(__FILE__, __LINE__, "\nDBInt.dll says: DLL not found : ", pdli->szDll, NULL);
			HMODULE self = GetModuleHandle(0);
			return (FARPROC)self;
			break;
		}
		case dliFailGetProc: {
			mkCoreDebug(__FILE__, __LINE__, "\nDBInt.dll says: Function not found : ", pdli->dlp.szProcName, NULL);
			if ((strcmp(pdli->dlp.szProcName, "oracleCreateConnection") == 0) || (strcmp(pdli->dlp.szProcName, "postgresqlCreateConnection") == 0)) {
				return (FARPROC)DBInt_CreateDBConnectionNotImplemented;
			}
			else if ((strcmp(pdli->dlp.szProcName, "oracleDestroyConnection") == 0) || (strcmp(pdli->dlp.szProcName, "postgresqlDestroyConnection") == 0)) {
				return (FARPROC)DBInt_DestroyDBConnectionNotImplemented;
			}
			else {
				mkCoreDebug(__FILE__, __LINE__, "\nFunction not mapped to local one : ", pdli->dlp.szProcName, NULL);
			}
			break;
		}
	}
	return 0;
}
extern PfnDliHook   __pfnDliFailureHook2 = delayedDllFailHook;

FARPROC WINAPI 
delayHook(
	unsigned dliNotify, 
	PDelayLoadInfo pdli
)
{
	switch (dliNotify) {
		case dliStartProcessing:
			// If you want to return control to the helper, return 0.
			// Otherwise, return a pointer to a FARPROC helper function
			// that will be used instead, thereby bypassing the rest 
			// of the helper.
			break;
		case dliNotePreLoadLibrary:
			// If you want to return control to the helper, return 0.
			// Otherwise, return your own HMODULE to be used by the 
			// helper instead of having it call LoadLibrary itself.
			break;
		case dliNotePreGetProcAddress:
			// If you want to return control to the helper, return 0.
			// If you choose you may supply your own FARPROC function 
			// address and bypass the helper's call to GetProcAddress.
			break;
		case dliFailLoadLib:
			// LoadLibrary failed.
			// If you don't want to handle this failure yourself, return 0.
			// In this case the helper will raise an exception 
			// (ERROR_MOD_NOT_FOUND) and exit.
			// If you want to handle the failure by loading an alternate 
			// DLL (for example), then return the HMODULE for 
			// the alternate DLL. The helper will continue execution with 
			// this alternate DLL and attempt to find the
			// requested entrypoint via GetProcAddress.
			break;
		case dliFailGetProc:
			// GetProcAddress failed.
			// If you don't want to handle this failure yourself, return 0.
			// In this case the helper will raise an exception 
			// (ERROR_PROC_NOT_FOUND) and exit.
			// If you choose you may handle the failure by returning 
			// an alternate FARPROC function address.
			break;
		case dliNoteEndProcessing:
			// This notification is called after all processing is done. 
			// There is no opportunity for modifying the helper's behavior
			// at this point except by longjmp()/throw()/RaiseException. 
			// No return value is processed.
			break;
		default:
			return NULL;
	}
	return NULL;
}
extern PfnDliHook   __pfnDliNotifyHook2 = delayHook;
