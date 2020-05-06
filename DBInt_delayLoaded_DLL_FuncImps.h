#pragma once

#include "db-interface.h"

 
// delay-loaded fail functions
DBInt_Connection  *	DBInt_CreateDBConnectionNotImplemented(HANDLE heapHandle, DBInt_SupportedDatabaseType dbType, const char * dbName, const char * userName, const char * password);
void				DBInt_DestroyDBConnectionNotImplemented(DBInt_Connection *mkDBConnection);

