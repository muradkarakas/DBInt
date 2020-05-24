/**
 * This file is part of Sodium Language project
 *
 * Copyright © 2020 Murad Karakaþ <muradkarakas@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License v3.0
 * as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 *	https://choosealicense.com/licenses/gpl-3.0/
 */

#include "pch.h"

#include "DBInt_delayLoaded_DLL_FuncImps.h"

DBInt_Connection  *	
DBInt_CreateDBConnectionNotImplemented(
	HANDLE heapHandle, 
	DBInt_SupportedDatabaseType dbType, 
	const char * dbName, 
	const char * userName, 
	const char * password
)
{
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

void 
DBInt_DestroyDBConnectionNotImplemented(
	DBInt_Connection * conn)
{
	// Nothing to do
}