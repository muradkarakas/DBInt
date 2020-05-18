#pragma once

#define DBINTERFACE_API __declspec(dllexport)

#include "dbIntefaceTypes.h"

#include "..\SodiumShared\SodiumShared.h"

typedef enum {
	HTSQL_COLUMN_TYPE_NOTSET,
	HTSQL_COLUMN_TYPE_TEXT,
	HTSQL_COLUMN_TYPE_NUMBER,
	HTSQL_COLUMN_TYPE_LOB,
	HTSQL_COLUMN_TYPE_DATE,
	HTSQL_COLUMN_TYPE_OBJECT
} SODIUM_DATABASE_COLUMN_TYPE;
	

/*	\brief	Private functions	*/
void								DBInt_InitConnection(DBInt_Connection * conn);

/*	\brief	Public/exported functions	*/
DBINTERFACE_API BOOL				DBInt_IsDatabaseSupported(DBInt_SupportedDatabaseType dbType);
DBINTERFACE_API int					DBInt_IsConnectionOpen(DBInt_Connection *conn);

DBINTERFACE_API 
DBInt_Connection  *	
DBInt_CreateDBConnection(
	HANDLE heapHandle, 
	DBInt_SupportedDatabaseType dbType, 
	const char * hostName, 
	const char * instanceName, 
	const char * databaseName, 
	const char * userName, 
	const char * password);

DBINTERFACE_API void				DBInt_DestroyDBConnection(DBInt_Connection *conn);
DBINTERFACE_API int					DBInt_IsEof(DBInt_Connection *conn, DBInt_Statement *stm);
DBINTERFACE_API void				DBInt_First(DBInt_Connection *conn, DBInt_Statement *stm);
DBINTERFACE_API void				DBInt_Last(DBInt_Connection *conn, DBInt_Statement *stm);
DBINTERFACE_API int					DBInt_Next(DBInt_Connection *conn, DBInt_Statement *stm);
DBINTERFACE_API int					DBInt_Prev(DBInt_Connection *conn, DBInt_Statement *stm);
DBINTERFACE_API DBInt_Statement	  * DBInt_CreateStatement(DBInt_Connection *conn);
DBINTERFACE_API void				DBInt_FreeStatement(DBInt_Connection *conn, DBInt_Statement *stm);
DBINTERFACE_API void				DBInt_Seek(DBInt_Connection *conn, DBInt_Statement *stm, int rowNum);

DBINTERFACE_API BOOL				DBInt_Commit(DBInt_Connection *conn);
DBINTERFACE_API BOOL				DBInt_Rollback(DBInt_Connection *conn);
DBINTERFACE_API void				DBInt_RegisterString(DBInt_Connection *conn, DBInt_Statement *stm, const char *bindVariableName, int maxLength);
DBINTERFACE_API unsigned int		DBInt_GetAffectedRows(DBInt_Connection *conn, DBInt_Statement *stm);
DBINTERFACE_API void				DBInt_BindString(DBInt_Connection *conn, DBInt_Statement *stm, char *bindVariableName, char *bindVariableValue, size_t valueLength);
DBINTERFACE_API void				DBInt_BindNumber(DBInt_Connection *conn, DBInt_Statement *stm, char *bindVariableName, char *bindVariableValue, size_t valueLength);
DBINTERFACE_API void				DBInt_ExecuteAnonymousBlock(DBInt_Connection *conn, DBInt_Statement *stm, const char *sql);
DBINTERFACE_API void				DBInt_ExecuteSelectStatement(DBInt_Connection *conn, DBInt_Statement *stm, const char *sql);
DBINTERFACE_API void				DBInt_ExecuteDescribe(DBInt_Connection *conn, DBInt_Statement *stm, const char *sql);

/*	Index of the first primary key column is 1. That is, position is not zero based.

	CALLER IS RESPONSIBLE TO RELEASE RETUN VALUE 
*/
DBINTERFACE_API	char			  * DBInt_GetPrimaryKeyColumn(DBInt_Connection* conn, const char* schemaName, const char* tableName, int position);

/*	If 'sql' parameter is not NULL, this function call 'Prepare' then executes. Otherwise, caller is responsible to call Prepare before execution.
	If you want to get row id from that function, you should construct SQL command and prepare manually before execution
		
	CALLER IS RESPONSIBLE TO RELEASE RETUN VALUE 
*/
DBINTERFACE_API char			  * DBInt_ExecuteInsertStatement(DBInt_Connection *conn, DBInt_Statement *stm, const char *sql);
DBINTERFACE_API void				DBInt_ExecuteDeleteStatement(DBInt_Connection *conn, DBInt_Statement *stm, const char *sql);
DBINTERFACE_API void				DBInt_ExecuteUpdateStatement(DBInt_Connection *conn, DBInt_Statement *stm, const char *sql);
DBINTERFACE_API void				DBInt_Prepare(DBInt_Connection * conn, DBInt_Statement *stm, const char *sql);
DBINTERFACE_API void				DBInt_BindLob(DBInt_Connection *conn, DBInt_Statement *stm, const char *imageFileName, char *bindVariableName);

/* CALLER IS RESPONSIBLE TO RELEASE RETUN VALUE */
DBINTERFACE_API void			  * DBInt_GetLob(DBInt_Connection * conn, DBInt_Statement *stm, const char *columnName, DWORD *sizeOfValue, BOOL isBase64Excoded);

DBINTERFACE_API 
SODIUM_DATABASE_COLUMN_TYPE			DBInt_GetColumnType(DBInt_Connection * conn, DBInt_Statement * stm, const char * columnName);

DBINTERFACE_API int					DBInt_GetColumnSize(DBInt_Connection *conn, DBInt_Statement *stm, const char *columnName);
DBINTERFACE_API int					DBInt_GetColumnCount(DBInt_Connection *conn, DBInt_Statement *stm);
DBINTERFACE_API int					DBInt_GetLastError(DBInt_Connection *conn);
DBINTERFACE_API const char		  * DBInt_GetLastErrorText(DBInt_Connection *conn);
DBINTERFACE_API const char		  * DBInt_GetColumnValueByColumnName(DBInt_Connection *conn, DBInt_Statement *stm, const char *columnName);
DBINTERFACE_API const char		  * DBInt_GetColumnNameByIndex(DBInt_Connection *conn, DBInt_Statement *stm, unsigned int index);

/* CALLER IS RESPONSIBLE TO RELEASE RETUN VALUE */
DBINTERFACE_API char			  * DBInt_GetDatabaseName(DBInt_Connection * DBIntConnection);

DBINTERFACE_API const char		  * DBInt_GetDatabaseType(DBInt_Connection * DBIntConnection);

