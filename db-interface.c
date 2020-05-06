// db-interface.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "db-interface.h"
#include <libloaderapi.h>

#include "..\DBInt-Oracle\oracle-interface.h"
#include "..\DBInt-Postgresql\postgresql-interface.h"
#include "..\DBInt-MySql\mysql-interface.h"

DBINTERFACE_API const char * DBInt_GetDatabaseType(DBInt_Connection *mkDBConnection) {
	mkDBConnection->errText = NULL;

	if (mkDBConnection) {
		switch (mkDBConnection->dbType) {
			case SODIUM_ORACLE_SUPPORT: {
				return "oracle";
			}
			case SODIUM_POSTGRESQL_SUPPORT: {
				return "postgresql";
			}
		}
	}

	return NULL;
}

DBINTERFACE_API char* DBInt_GetDatabaseName(DBInt_Connection *mkDBConnection) {
	char *retval;
	switch (mkDBConnection->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			retval = oracleGetDatabaseName(mkDBConnection);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			retval = postgresqlGetDatabaseName(mkDBConnection);
			break;
		}
	}
	return retval;
}


void DBInt_InitConnection(DBInt_Connection * conn) {
	if (conn == NULL) {
		return;
	}
	switch (conn->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			oracleInitConnection(conn);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			postgresqlInitConnection(conn);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			mysqlInitConnection(conn);
			break;
		}
	}
}

DBINTERFACE_API 
DBInt_Connection *
DBInt_CreateDBConnection(
	HANDLE heapHandle, 
	DBInt_SupportedDatabaseType dbType, 
	const char * hostName,
	const char * dbName, 
	const char * userName, 
	const char * password
)
{
	DBInt_Connection * conn = NULL;
	switch (dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			conn = oracleCreateConnection(heapHandle, dbType, dbName, userName, password);			
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			conn = postgresqlCreateConnection(heapHandle, dbType, hostName, dbName, userName, password);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			conn = mysqlCreateConnection(heapHandle, dbType, hostName, dbName, userName, password);
			break;
		}
		default: {

			break;
		}
	}
	if (conn->errText == NULL) {
		conn->dbType = dbType;
		conn->heapHandle = heapHandle;
		DBInt_InitConnection(conn);
	}
	return conn;
}

DBINTERFACE_API void DBInt_BindLob(DBInt_Connection *mkDBConnection, DBInt_Statement *stm, const char *imageFileName, char *bindVariableName) {
	switch (mkDBConnection->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			oracleBindLob(mkDBConnection, stm, imageFileName, bindVariableName);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			postgresqlBindLob(mkDBConnection, stm, imageFileName, bindVariableName);
			break;
		}
	}
}

DBINTERFACE_API void *DBInt_GetLob(DBInt_Connection * mkConnection, DBInt_Statement *stm, const char *columnName, DWORD *sizeOfValue, BOOL isBase64Excoded) {
	void *lobContent = NULL;
	void *lobContentRetValue = NULL;

	mkConnection->errText = NULL;
	*sizeOfValue = 0;

	switch (mkConnection->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			lobContent = oracleGetLob(mkConnection, stm, columnName, sizeOfValue);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			lobContent = postgresqlGetLob(mkConnection, stm, columnName, sizeOfValue);
			break;
		}
	}

	if (lobContent) {
		if (isBase64Excoded) {
			void *b64encoded = mkBase64Encode(mkConnection->heapHandle, (unsigned char*)lobContent, *sizeOfValue);
			if (b64encoded) {
				lobContentRetValue = mkStrcat(mkConnection->heapHandle, __FILE__, __LINE__, "data:image;base64,", b64encoded, NULL);
				*sizeOfValue = (DWORD) strlen((char*)lobContentRetValue);
				mkFree(mkConnection->heapHandle, b64encoded);
				mkFree(mkConnection->heapHandle, lobContent);
				lobContent = NULL;
			}			
		}
	}
	if (lobContentRetValue) {
		return lobContentRetValue;
	}
	else {
		return lobContent;
	}
}



DBINTERFACE_API void DBInt_RegisterString(DBInt_Connection *mkDBConnection, DBInt_Statement *stm, const char *bindVariableName, int maxLength) {
	switch (mkDBConnection->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			oracleRegisterString(mkDBConnection, stm, bindVariableName, maxLength);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			postgresqlRegisterString(mkDBConnection, stm, bindVariableName, maxLength);
			break;
		}
	}
}


DBINTERFACE_API void DBInt_ExecuteDeleteStatement(DBInt_Connection *mkDBConnection, DBInt_Statement *stm, const char *sql) {
	if (mkDBConnection && sql) {
		mkDBConnection->errText = NULL;
		switch (mkDBConnection->dbType) {
			case SODIUM_ORACLE_SUPPORT: {
				oracleExecuteDeleteStatement(mkDBConnection, stm, sql);
				break;
			}
			case SODIUM_POSTGRESQL_SUPPORT: {
				postgresqlExecuteDeleteStatement(mkDBConnection, stm, sql);
				break;
			}
			case SODIUM_MYSQL_SUPPORT: {
				mysqlExecuteDeleteStatement(mkDBConnection, stm, sql);
				break;
			}
		}
	}
}

DBINTERFACE_API void DBInt_ExecuteAnonymousBlock(DBInt_Connection *mkDBConnection, DBInt_Statement *stm, const char *sql) {
	if (mkDBConnection && sql) {
		mkDBConnection->errText = NULL;
		switch (mkDBConnection->dbType) {
			case SODIUM_ORACLE_SUPPORT: {
				oracleExecuteAnonymousBlock(mkDBConnection, stm, sql);
				break;
			}
			case SODIUM_POSTGRESQL_SUPPORT: {
				postgresqlExecuteAnonymousBlock(mkDBConnection, stm, sql);
				break;
			}
			case SODIUM_MYSQL_SUPPORT: {
				mysqlExecuteAnonymousBlock(mkDBConnection, stm, sql);
				break;
			}
		}
	}
}


DBINTERFACE_API void DBInt_ExecuteUpdateStatement(DBInt_Connection *mkDBConnection, DBInt_Statement *stm, const char *sql) {
	if (mkDBConnection && sql) {
		switch (mkDBConnection->dbType) {
			case SODIUM_ORACLE_SUPPORT: {
				oracleExecuteUpdateStatement(mkDBConnection, stm, sql);
				break;
			}
			case SODIUM_POSTGRESQL_SUPPORT: {
				postgresqlExecuteUpdateStatement(mkDBConnection, stm, sql);
				break;
			}
			case SODIUM_MYSQL_SUPPORT: {
				mysqlExecuteUpdateStatement(mkDBConnection, stm, sql);
				break;
			}
		}
	}
}

DBINTERFACE_API void	DBInt_ExecuteDescribe(DBInt_Connection *mkDBConnection, DBInt_Statement *stm, const char *sql) {
	if (mkDBConnection && (stm || sql)) {
		mkDBConnection->errText = NULL;
		switch (mkDBConnection->dbType) {
			case SODIUM_ORACLE_SUPPORT: {
				oracleExecuteDescribe(mkDBConnection, stm, sql);
				break;
			}
			case SODIUM_POSTGRESQL_SUPPORT: {
				postgresqlExecuteDescribe(mkDBConnection, stm, sql);
				break;
			}
			case SODIUM_MYSQL_SUPPORT: {
				mysqlExecuteDescribe(mkDBConnection, stm, sql);
				break;
			}
		}
	}
}

DBINTERFACE_API void	DBInt_ExecuteSelectStatement(DBInt_Connection *mkDBConnection, DBInt_Statement *stm, const char *sql) {
	if (mkDBConnection && (stm || sql)) {
		switch (mkDBConnection->dbType) {
			case SODIUM_ORACLE_SUPPORT: {
				oracleExecuteSelectStatement(mkDBConnection, stm, sql);
				break;
			}
			case SODIUM_POSTGRESQL_SUPPORT: {
				postgresqlExecuteSelectStatement(mkDBConnection, stm, sql);
				break;
			}
			case SODIUM_MYSQL_SUPPORT: {
				mysqlExecuteSelectStatement(mkDBConnection, stm, sql);
				break;
			}
		}
	}
}

DBINTERFACE_API void	DBInt_Prepare(DBInt_Connection * mkConnection, DBInt_Statement *stm, const char *sql) {
	switch (mkConnection->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			oraclePrepare(mkConnection, stm, sql);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			postgresqlPrepare(mkConnection, stm, sql);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			mysqlPrepare(mkConnection, stm, sql);
			break;
		}
	}
}

DBINTERFACE_API char * DBInt_ExecuteInsertStatement(DBInt_Connection *mkDBConnection, DBInt_Statement *stm, const char *sql) {
	char *retval = NULL;
	if (mkDBConnection) {
		switch (mkDBConnection->dbType) {
			case SODIUM_ORACLE_SUPPORT: {
				retval = oracleExecuteInsertStatement(mkDBConnection, stm, sql);
				break;
			}
			case SODIUM_POSTGRESQL_SUPPORT: {
				retval = postgresqlExecuteInsertStatement(mkDBConnection, stm, sql);
				break;
			}
			case SODIUM_MYSQL_SUPPORT: {
				retval = mysqlExecuteInsertStatement(mkDBConnection, stm, sql);
				break;
			}
		}
	}
	return retval;
}

DBINTERFACE_API void DBInt_BindString(DBInt_Connection *mkDBConnection, DBInt_Statement *stm, char *bindVariableName, char *bindVariableValue, size_t valueLength) {
	switch (mkDBConnection->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			oracleBindString(mkDBConnection, stm, bindVariableName, bindVariableValue, valueLength);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			postgresqlBindString(mkDBConnection, stm, bindVariableName, bindVariableValue, valueLength);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			mysqlBindString(mkDBConnection, stm, bindVariableName, bindVariableValue, valueLength);
			break;
		}
	}
}

DBINTERFACE_API void DBInt_BindNumber(
	DBInt_Connection *mkDBConnection, 
	DBInt_Statement *stm, 
	char *bindVariableName, 
	char *bindVariableValue, 
	size_t valueLength
)
{
	switch (mkDBConnection->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			oracleBindString(mkDBConnection, stm, bindVariableName, bindVariableValue, valueLength);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			postgresqlBindString(mkDBConnection, stm, bindVariableName, bindVariableValue, valueLength);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			mysqlBindNumber(mkDBConnection, stm, bindVariableName, bindVariableValue, valueLength);
			break;
		}
	}
}

DBINTERFACE_API unsigned int DBInt_GetAffectedRows(DBInt_Connection *mkDBConnection, DBInt_Statement *stm) {
	int retval;
	switch (mkDBConnection->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			retval = oracleGetAffectedRows(mkDBConnection, stm);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			retval = postgresqlGetAffectedRows(mkDBConnection, stm);
			break;
		}
	}
	return retval;
}

DBINTERFACE_API int DBInt_GetColumnCount(DBInt_Connection *mkDBConnection, DBInt_Statement *stm) {
	int retval = 0;
	switch (mkDBConnection->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			retval = oracleGetColumnCount(mkDBConnection, stm);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			retval = postgresqlGetColumnCount(mkDBConnection, stm);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			retval = mysqlGetColumnCount(mkDBConnection, stm);
			break;
		}
	}
	return retval;
}

DBINTERFACE_API BOOL DBInt_Rollback(DBInt_Connection *mkDBConnection) {
	BOOL retval;
	mkDBConnection->errText = NULL;

	switch (mkDBConnection->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			retval = oracleRollback(mkDBConnection);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			retval = postgresqlRollback(mkDBConnection);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			retval = mysqlRollback(mkDBConnection);
			break;
		}
	}
	return retval;
}


DBINTERFACE_API BOOL DBInt_Commit(DBInt_Connection *mkDBConnection) {
	BOOL retval;
	mkDBConnection->errText = NULL;

	switch (mkDBConnection->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			retval = oracleCommit(mkDBConnection);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			retval = postgresqlCommit(mkDBConnection);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			retval = mysqlCommit(mkDBConnection);
			break;
		}
	}
	return retval;
}

DBINTERFACE_API 
char *
DBInt_GetPrimaryKeyColumn(
	DBInt_Connection *mkDBConnection, 
	const char *schemaName,
	const char *tableName,
	int			position
)
{
	char * retval;
	mkDBConnection->errText = NULL;

	switch (mkDBConnection->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			retval = oracleGetPrimaryKeyColumn(mkDBConnection, schemaName, tableName, position);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			retval = postgresqlGetPrimaryKeyColumn(mkDBConnection, schemaName, tableName, position);
			break;
		}
	}
	return retval;
}

DBINTERFACE_API int DBInt_GetLastError(DBInt_Connection *mkDBConnection) {
	int retval;

	switch (mkDBConnection->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			retval = oracleGetLastError(mkDBConnection);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			retval = postgresqlGetLastError(mkDBConnection);
			break;
		}
	}
	return retval;
}

DBINTERFACE_API const char * DBInt_GetLastErrorText(DBInt_Connection *mkDBConnection) {
	const char *retval = NULL;

	switch (mkDBConnection->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			retval = oracleGetLastErrorText(mkDBConnection);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			retval = postgresqlGetLastErrorText(mkDBConnection);
			break;
		}
	}
	return retval;
}


DBINTERFACE_API void DBInt_Seek(DBInt_Connection *mkDBConnection, DBInt_Statement *stm, int rowNum) {
	
	mkDBConnection->errText = NULL;
	switch (mkDBConnection->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			oracleSeek(mkDBConnection, stm, rowNum);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			postgresqlSeek(mkDBConnection, stm, rowNum);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			mysqlSeek(mkDBConnection, stm, rowNum);
			break;
		}
	}
	
}

DBINTERFACE_API DBInt_Statement	*DBInt_CreateStatement(DBInt_Connection *mkDBConnection) {
	DBInt_Statement	*retval = NULL;

	switch (mkDBConnection->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			retval = oracleCreateStatement(mkDBConnection);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			retval = postgresqlCreateStatement(mkDBConnection);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			retval = mysqlCreateStatement(mkDBConnection);
			break;
		}
	}

	return retval;
}
	

DBINTERFACE_API void	DBInt_FreeStatement(DBInt_Connection *mkDBConnection, DBInt_Statement *stm) {

	switch (mkDBConnection->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			oracleFreeStatement(mkDBConnection, stm);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			postgresqlFreeStatement(mkDBConnection, stm);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			mysqlFreeStatement(mkDBConnection, stm);
			break;
		}
	}

}

DBINTERFACE_API int DBInt_Next(DBInt_Connection *mkDBConnection, DBInt_Statement *stm) {
	int retval;

	switch (mkDBConnection->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			retval = oracleNext(stm);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			retval = postgresqlNext(stm);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			retval = mysqlNext(stm);
			break;
		}
	}

	return retval;
}

DBINTERFACE_API void DBInt_First(DBInt_Connection *mkDBConnection, DBInt_Statement *stm) {
	switch (mkDBConnection->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			oracleFirst(mkDBConnection, stm);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			postgresqlFirst(stm);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			mysqlFirst(mkDBConnection, stm);
			break;
		}
	}
}

DBINTERFACE_API void DBInt_Last(DBInt_Connection *mkDBConnection, DBInt_Statement *stm) {
	switch (mkDBConnection->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			oracleLast(mkDBConnection, stm);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			postgresqlLast(stm);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			mysqlLast(mkDBConnection, stm);
			break;
		}
	}
}

DBINTERFACE_API int DBInt_Prev(DBInt_Connection *mkDBConnection, DBInt_Statement *stm) {
	int retval;
	switch (mkDBConnection->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			retval = oraclePrev(stm);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			retval = postgresqlPrev(stm);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			retval = mysqlPrev(stm);
			break;
		}
	}
	return retval;
}

DBINTERFACE_API int	DBInt_IsEof(DBInt_Connection *mkDBConnection, DBInt_Statement *stm) {
	int retval = FALSE;

	switch (mkDBConnection->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			retval = oracleIsEof(stm);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			retval = postgresqlIsEof(stm);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			retval = mysqlIsEof(stm);
			break;
		}
	}

	return retval;
}

DBINTERFACE_API BOOL DBInt_IsDatabaseSupported(DBInt_SupportedDatabaseType dbType) {
	return (dbType == SODIUM_ORACLE_SUPPORT || dbType == SODIUM_POSTGRESQL_SUPPORT || dbType == SODIUM_MYSQL_SUPPORT);
}

DBINTERFACE_API int DBInt_IsConnectionOpen(DBInt_Connection * mkDBConnection)
{
	int retval;

	switch (mkDBConnection->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			retval = oracleIsConnectionOpen(mkDBConnection);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			retval = postgresqlIsConnectionOpen(mkDBConnection);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			retval = mysqlIsConnectionOpen(mkDBConnection);
			break;
		}
	}

	return retval;
}

DBINTERFACE_API void DBInt_DestroyDBConnection(DBInt_Connection *mkDBConnection) {
	if (mkDBConnection) {
		switch (mkDBConnection->dbType) {
			case SODIUM_ORACLE_SUPPORT: {
				oracleDestroyConnection(mkDBConnection);
				break;
			}
			case SODIUM_POSTGRESQL_SUPPORT: {
				postgresqlDestroyConnection(mkDBConnection);
				break;
			}
			case SODIUM_MYSQL_SUPPORT: {
				mysqlDestroyConnection(mkDBConnection);
				break;
			}
		}
		mkFree(mkDBConnection->heapHandle, mkDBConnection);
	}
}


DBINTERFACE_API const char	*DBInt_GetColumnValueByColumnName(DBInt_Connection *mkDBConnection, DBInt_Statement *stm, const char *columnName) {
	const char	*ret = NULL;

	if (mkDBConnection && columnName) {
		switch (mkDBConnection->dbType) {
			case SODIUM_ORACLE_SUPPORT: {
				ret = oracleGetColumnValueByColumnName(mkDBConnection, stm, columnName);
				break;
			}
			case SODIUM_POSTGRESQL_SUPPORT: {
				ret = postgresqlGetColumnValueByColumnName(mkDBConnection, stm, columnName);
				break;
			}
			case SODIUM_MYSQL_SUPPORT: {
				ret = mysqlGetColumnValueByColumnName(mkDBConnection, stm, columnName);
				break;
			}
		}
	}
	return ret;
}


DBINTERFACE_API const char * DBInt_GetColumnNameByIndex(DBInt_Connection *mkDBConnection, DBInt_Statement *stm, unsigned int index) {
	const char	*columnName = NULL;

	switch (mkDBConnection->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			columnName = oracleGetColumnNameByIndex(mkDBConnection, stm, index);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			columnName = postgresqlGetColumnNameByIndex(mkDBConnection, stm, index);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			columnName = mysqlGetColumnNameByIndex(mkDBConnection, stm, index);
			break;
		}
	}
	return columnName;
}


DBINTERFACE_API int DBInt_GetColumnSize(DBInt_Connection *mkDBConnection, DBInt_Statement *stm, const char *columnName) {
	int ret = 0;
	if (mkDBConnection && columnName) {
		switch (mkDBConnection->dbType) {
			case SODIUM_ORACLE_SUPPORT: {
				ret = oracleGetColumnSize(mkDBConnection, stm, columnName);
				break;
			}
			case SODIUM_POSTGRESQL_SUPPORT: {
				ret = postgresqlGetColumnSize(mkDBConnection, stm, columnName);
				break;
			}
			case SODIUM_MYSQL_SUPPORT: {
				ret = mysqlGetColumnSize(mkDBConnection, stm, columnName);
				break;
			}
		}
	}
	return ret;
}


DBINTERFACE_API HTSQL_COLUMN_TYPE DBInt_GetColumnType(DBInt_Connection *mkDBConnection, DBInt_Statement *stm, const char *columnName) {
	HTSQL_COLUMN_TYPE ret = HTSQL_COLUMN_TYPE_NOTSET;
	if (mkDBConnection && columnName) {
		switch (mkDBConnection->dbType) {
			case SODIUM_ORACLE_SUPPORT: {
				ret = oracleGetColumnType(mkDBConnection, stm, columnName);
				break;
			}
			case SODIUM_POSTGRESQL_SUPPORT: {
				ret = postgresqlGetColumnType(mkDBConnection, stm, columnName);
				break;
			}
			case SODIUM_MYSQL_SUPPORT: {
				ret = mysqlGetColumnType(mkDBConnection, stm, columnName);
				break;
			}
		}
	}
	return ret;
}