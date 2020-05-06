#include "DBInt_delayLoaded_DLL_FuncImps.h"

DBInt_Connection  *	DBInt_CreateDBConnectionNotImplemented(HANDLE heapHandle, DBInt_SupportedDatabaseType dbType, const char * dbName, const char * userName, const char * password) {
	DBInt_Connection *mkConnection = (DBInt_Connection *)mkMalloc(heapHandle, sizeof(DBInt_Connection), __FILE__, __LINE__);
	mkConnection->dbType = dbType;
	mkConnection->heapHandle = heapHandle;
	strcpy_s(mkConnection->hostName, HOST_NAME_LENGTH, "localhost");
	mkConnection->err = TRUE;
	switch (dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			mkConnection->errText = "\"ORACLE Database Abtraction Implementation Layer DLL\" does not exists or one of the function does not exists in that dll file";
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			mkConnection->errText = "\"POSTGRESQL Database Abtraction Implementation Layer DLL\" does not exists or one of the function does not exists in that dll file";
			break;
		}
	}
	return mkConnection;
}

void DBInt_DestroyDBConnectionNotImplemented(DBInt_Connection *mkDBConnection) {
	// Nothing to do
}