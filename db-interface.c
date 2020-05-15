#include "pch.h"

#include "db-interface.h"
#include <libloaderapi.h>

#include "..\DBInt-Oracle\oracle-interface.h"
#include "..\DBInt-Postgresql\postgresql-interface.h"
#include "..\DBInt-MySql\mysql-interface.h"
#include "..\DBInt-SqlServer\sqlserver-interface.h"


DBINTERFACE_API 
const char * 
DBInt_GetDatabaseType(
	DBInt_Connection * DBIntConnection
)
{
	DBIntConnection->errText = NULL;
	if (DBIntConnection) {
		switch (DBIntConnection->dbType) {
			case SODIUM_ORACLE_SUPPORT: {
				return "oracle";
			}
			case SODIUM_POSTGRESQL_SUPPORT: {
				return "postgresql";
			}
			case SODIUM_MYSQL_SUPPORT: {
				return "mysql";
			}
			case SODIUM_SQLSERVER_SUPPORT: {
				return "sqlserver";
			}
		}
	}
	return NULL;
}

DBINTERFACE_API 
char * 
DBInt_GetDatabaseName(
	DBInt_Connection * DBIntConnection
)
{
	char *retval = "";
	switch (DBIntConnection->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			retval = oracleGetDatabaseName(DBIntConnection);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			retval = postgresqlGetDatabaseName(DBIntConnection);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			retval = mysqlGetDatabaseName(DBIntConnection);
			break;
		}
	}
	return retval;
}


void 
DBInt_InitConnection(
	DBInt_Connection * conn
)
{
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
		case SODIUM_SQLSERVER_SUPPORT: {
			conn = sqlserverCreateConnection(heapHandle, dbType, hostName, dbName, userName, password);
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

DBINTERFACE_API
void 
DBInt_BindLob(
	DBInt_Connection * conn, 
	DBInt_Statement * stm, 
	const char * imageFileName, 
	char * bindVariableName
)
{
	switch (conn->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			oracleBindLob(conn, stm, imageFileName, bindVariableName);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			postgresqlBindLob(conn, stm, imageFileName, bindVariableName);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			mysqlBindLob(conn, stm, imageFileName, bindVariableName);
			break;
		}
	}
}

DBINTERFACE_API 
void * 
DBInt_GetLob(
	DBInt_Connection * conn, 
	DBInt_Statement * stm, 
	const char * columnName, 
	DWORD * sizeOfValue, 
	BOOL isBase64Excoded
)
{
	void *lobContent = NULL;
	void *lobContentRetValue = NULL;

	conn->errText = NULL;
	*sizeOfValue = 0;

	switch (conn->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			lobContent = oracleGetLob(conn, stm, columnName, sizeOfValue);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			lobContent = postgresqlGetLob(conn, stm, columnName, sizeOfValue);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			lobContent = mysqlGetLob(conn, stm, columnName, sizeOfValue);
			break;
		}
	}

	if (lobContent) {
		if (isBase64Excoded) {
			void *b64encoded = mkBase64Encode(conn->heapHandle, (unsigned char*)lobContent, *sizeOfValue);
			if (b64encoded) {
				lobContentRetValue = mkStrcat(conn->heapHandle, __FILE__, __LINE__, "data:image;base64,", b64encoded, NULL);
				*sizeOfValue = (DWORD) strlen((char*)lobContentRetValue);
				mkFree(conn->heapHandle, b64encoded);
				mkFree(conn->heapHandle, lobContent);
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



DBINTERFACE_API
void 
DBInt_RegisterString(
	DBInt_Connection * conn, 
	DBInt_Statement * stm, 
	const char * bindVariableName, 
	int maxLength
)
{
	switch (conn->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			oracleRegisterString(conn, stm, bindVariableName, maxLength);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			postgresqlRegisterString(conn, stm, bindVariableName, maxLength);
			break;
		}
	}
}


DBINTERFACE_API
void 
DBInt_ExecuteDeleteStatement(
	DBInt_Connection * conn, 
	DBInt_Statement * stm, 
	const char * sql
)
{
	if (conn && sql) {
		conn->errText = NULL;
		switch (conn->dbType) {
			case SODIUM_ORACLE_SUPPORT: {
				oracleExecuteDeleteStatement(conn, stm, sql);
				break;
			}
			case SODIUM_POSTGRESQL_SUPPORT: {
				postgresqlExecuteDeleteStatement(conn, stm, sql);
				break;
			}
			case SODIUM_MYSQL_SUPPORT: {
				mysqlExecuteDeleteStatement(conn, stm, sql);
				break;
			}
		}
	}
}

DBINTERFACE_API
void 
DBInt_ExecuteAnonymousBlock(
	DBInt_Connection * conn,
	DBInt_Statement * stm, 
	const char * sql
)
{
	if (conn && sql) {
		conn->errText = NULL;
		switch (conn->dbType) {
			case SODIUM_ORACLE_SUPPORT: {
				oracleExecuteAnonymousBlock(conn, stm, sql);
				break;
			}
			case SODIUM_POSTGRESQL_SUPPORT: {
				postgresqlExecuteAnonymousBlock(conn, stm, sql);
				break;
			}
			case SODIUM_MYSQL_SUPPORT: {
				mysqlExecuteAnonymousBlock(conn, stm, sql);
				break;
			}
		}
	}
}


DBINTERFACE_API
void 
DBInt_ExecuteUpdateStatement(
	DBInt_Connection * conn, 
	DBInt_Statement * stm, 
	const char * sql
)
{
	if (conn && sql) {
		switch (conn->dbType) {
			case SODIUM_ORACLE_SUPPORT: {
				oracleExecuteUpdateStatement(conn, stm, sql);
				break;
			}
			case SODIUM_POSTGRESQL_SUPPORT: {
				postgresqlExecuteUpdateStatement(conn, stm, sql);
				break;
			}
			case SODIUM_MYSQL_SUPPORT: {
				mysqlExecuteUpdateStatement(conn, stm, sql);
				break;
			}
		}
	}
}

DBINTERFACE_API
void
DBInt_ExecuteDescribe(
	DBInt_Connection * conn, 
	DBInt_Statement * stm, 
	const char * sql
)
{
	if (conn && (stm || sql)) {
		conn->errText = NULL;
		switch (conn->dbType) {
			case SODIUM_ORACLE_SUPPORT: {
				oracleExecuteDescribe(conn, stm, sql);
				break;
			}
			case SODIUM_POSTGRESQL_SUPPORT: {
				postgresqlExecuteDescribe(conn, stm, sql);
				break;
			}
			case SODIUM_MYSQL_SUPPORT: {
				mysqlExecuteDescribe(conn, stm, sql);
				break;
			}
		}
	}
}

DBINTERFACE_API 
void
DBInt_ExecuteSelectStatement(
	DBInt_Connection * conn, 
	DBInt_Statement * stm, 
	const char * sql
)
{
	if (conn && (stm || sql)) {
		switch (conn->dbType) {
			case SODIUM_ORACLE_SUPPORT: {
				oracleExecuteSelectStatement(conn, stm, sql);
				break;
			}
			case SODIUM_POSTGRESQL_SUPPORT: {
				postgresqlExecuteSelectStatement(conn, stm, sql);
				break;
			}
			case SODIUM_MYSQL_SUPPORT: {
				mysqlExecuteSelectStatement(conn, stm, sql);
				break;
			}
		}
	}
}

DBINTERFACE_API 
void
DBInt_Prepare(
	DBInt_Connection * conn,
	DBInt_Statement * stm, 
	const char * sql
)
{
	switch (conn->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			oraclePrepare(conn, stm, sql);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			postgresqlPrepare(conn, stm, sql);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			mysqlPrepare(conn, stm, sql);
			break;
		}
	}
}

DBINTERFACE_API
char * 
DBInt_ExecuteInsertStatement(
	DBInt_Connection * conn, 
	DBInt_Statement * stm, 
	const char * sql)
{
	char * retval = NULL;
	if (conn) {
		switch (conn->dbType) {
			case SODIUM_ORACLE_SUPPORT: {
				retval = oracleExecuteInsertStatement(conn, stm, sql);
				break;
			}
			case SODIUM_POSTGRESQL_SUPPORT: {
				retval = postgresqlExecuteInsertStatement(conn, stm, sql);
				break;
			}
			case SODIUM_MYSQL_SUPPORT: {
				retval = mysqlExecuteInsertStatement(conn, stm, sql);
				break;
			}
		}
	}
	return retval;
}

DBINTERFACE_API
void 
DBInt_BindString(
	DBInt_Connection * conn, 
	DBInt_Statement * stm, 
	char * bindVariableName, 
	char * bindVariableValue, 
	size_t valueLength
)
{
	switch (conn->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			oracleBindString(conn, stm, bindVariableName, bindVariableValue, valueLength);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			postgresqlBindString(conn, stm, bindVariableName, bindVariableValue, valueLength);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			mysqlBindString(conn, stm, bindVariableName, bindVariableValue, valueLength);
			break;
		}
	}
}

DBINTERFACE_API
void 
DBInt_BindNumber(
	DBInt_Connection * conn, 
	DBInt_Statement * stm, 
	char * bindVariableName, 
	char * bindVariableValue, 
	size_t valueLength
)
{
	switch (conn->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			oracleBindString(conn, stm, bindVariableName, bindVariableValue, valueLength);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			postgresqlBindString(conn, stm, bindVariableName, bindVariableValue, valueLength);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			mysqlBindNumber(conn, stm, bindVariableName, bindVariableValue, valueLength);
			break;
		}
	}
}

DBINTERFACE_API 
unsigned int 
DBInt_GetAffectedRows(
	DBInt_Connection * conn, 
	DBInt_Statement * stm
)
{
	int retval = 0;
	switch (conn->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			retval = oracleGetAffectedRows(conn, stm);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			retval = postgresqlGetAffectedRows(conn, stm);
			break;
		}
	}
	return retval;
}

DBINTERFACE_API
int 
DBInt_GetColumnCount(
	DBInt_Connection * conn, 
	DBInt_Statement * stm
)
{
	int retval = 0;
	switch (conn->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			retval = oracleGetColumnCount(conn, stm);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			retval = postgresqlGetColumnCount(conn, stm);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			retval = mysqlGetColumnCount(conn, stm);
			break;
		}
	}
	return retval;
}

DBINTERFACE_API
BOOL 
DBInt_Rollback(
	DBInt_Connection * conn
)
{
	BOOL retval = FALSE;
	conn->errText = NULL;
	switch (conn->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			retval = oracleRollback(conn);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			retval = postgresqlRollback(conn);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			retval = mysqlRollback(conn);
			break;
		}
	}
	return retval;
}


DBINTERFACE_API
BOOL 
DBInt_Commit(
	DBInt_Connection * conn
)
{
	BOOL retval = FALSE;
	conn->errText = NULL;
	switch (conn->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			retval = oracleCommit(conn);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			retval = postgresqlCommit(conn);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			retval = mysqlCommit(conn);
			break;
		}
	}
	return retval;
}

DBINTERFACE_API 
char *
DBInt_GetPrimaryKeyColumn(
	DBInt_Connection *conn, 
	const char *schemaName,
	const char *tableName,
	int			position
)
{
	char * retval;
	conn->errText = NULL;
	switch (conn->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			retval = oracleGetPrimaryKeyColumn(conn, schemaName, tableName, position);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			retval = postgresqlGetPrimaryKeyColumn(conn, schemaName, tableName, position);
			break;
		}
	}
	return retval;
}

DBINTERFACE_API
int 
DBInt_GetLastError(
	DBInt_Connection * conn
)
{
	int retval = 0;
	switch (conn->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			retval = oracleGetLastError(conn);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			retval = postgresqlGetLastError(conn);
			break;
		}
	}
	return retval;
}

DBINTERFACE_API
const char * 
DBInt_GetLastErrorText(
	DBInt_Connection * conn
)
{
	const char *retval = NULL;
	switch (conn->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			retval = oracleGetLastErrorText(conn);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			retval = postgresqlGetLastErrorText(conn);
			break;
		}
	}
	return retval;
}


DBINTERFACE_API
void
DBInt_Seek(
	DBInt_Connection * conn, 
	DBInt_Statement * stm,
	int rowNum
)
{
	conn->errText = NULL;
	switch (conn->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			oracleSeek(conn, stm, rowNum);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			postgresqlSeek(conn, stm, rowNum);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			mysqlSeek(conn, stm, rowNum);
			break;
		}
	}
}

DBINTERFACE_API
DBInt_Statement	* 
DBInt_CreateStatement(
	DBInt_Connection * conn
)
{
	DBInt_Statement	*retval = NULL;
	switch (conn->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			retval = oracleCreateStatement(conn);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			retval = postgresqlCreateStatement(conn);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			retval = mysqlCreateStatement(conn);
			break;
		}
	}
	return retval;
}
	

DBINTERFACE_API 
void
DBInt_FreeStatement(
	DBInt_Connection * conn,
	DBInt_Statement * stm
)
{
	switch (conn->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			oracleFreeStatement(conn, stm);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			postgresqlFreeStatement(conn, stm);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			mysqlFreeStatement(conn, stm);
			break;
		}
	}
}

DBINTERFACE_API
int DBInt_Next(
	DBInt_Connection *conn, 
	DBInt_Statement * stm
)
{
	int retval;
	switch (conn->dbType) {
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

DBINTERFACE_API 
void 
DBInt_First(
	DBInt_Connection *conn,
	DBInt_Statement *stm
)
{
	switch (conn->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			oracleFirst(conn, stm);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			postgresqlFirst(stm);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			mysqlFirst(conn, stm);
			break;
		}
	}
}

DBINTERFACE_API 
void 
DBInt_Last(
	DBInt_Connection *conn, 
	DBInt_Statement *stm
)
{
	switch (conn->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			oracleLast(conn, stm);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			postgresqlLast(stm);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			mysqlLast(conn, stm);
			break;
		}
	}
}

DBINTERFACE_API
int 
DBInt_Prev(
	DBInt_Connection *conn, 
	DBInt_Statement *stm
)
{
	int retval;
	switch (conn->dbType) {
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

DBINTERFACE_API 
int	
DBInt_IsEof(
	DBInt_Connection *conn, 
	DBInt_Statement *stm
)
{
	int retval = FALSE;
	switch (conn->dbType) {
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

DBINTERFACE_API 
BOOL 
DBInt_IsDatabaseSupported(
	DBInt_SupportedDatabaseType dbType
)
{
	return (dbType == SODIUM_ORACLE_SUPPORT || dbType == SODIUM_POSTGRESQL_SUPPORT || dbType == SODIUM_MYSQL_SUPPORT);
}

DBINTERFACE_API 
int 
DBInt_IsConnectionOpen(
	DBInt_Connection * conn
)
{
	int retval;
	switch (conn->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			retval = oracleIsConnectionOpen(conn);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			retval = postgresqlIsConnectionOpen(conn);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			retval = mysqlIsConnectionOpen(conn);
			break;
		}
	}
	return retval;
}

DBINTERFACE_API 
void 
DBInt_DestroyDBConnection(
	DBInt_Connection *conn
) 
{
	if (conn) {
		switch (conn->dbType) {
			case SODIUM_ORACLE_SUPPORT: {
				oracleDestroyConnection(conn);
				break;
			}
			case SODIUM_POSTGRESQL_SUPPORT: {
				postgresqlDestroyConnection(conn);
				break;
			}
			case SODIUM_MYSQL_SUPPORT: {
				mysqlDestroyConnection(conn);
				break;
			}
		}
		mkFree(conn->heapHandle, conn);
	}
}


DBINTERFACE_API 
const char*
DBInt_GetColumnValueByColumnName(
	DBInt_Connection *conn, 
	DBInt_Statement *stm, 
	const char *columnName
)
{
	const char	*ret = NULL;
	if (conn && columnName) {
		switch (conn->dbType) {
			case SODIUM_ORACLE_SUPPORT: {
				ret = oracleGetColumnValueByColumnName(conn, stm, columnName);
				break;
			}
			case SODIUM_POSTGRESQL_SUPPORT: {
				ret = postgresqlGetColumnValueByColumnName(conn, stm, columnName);
				break;
			}
			case SODIUM_MYSQL_SUPPORT: {
				ret = mysqlGetColumnValueByColumnName(conn, stm, columnName);
				break;
			}
		}
	}
	return ret;
}


DBINTERFACE_API 
const char * 
DBInt_GetColumnNameByIndex(
	DBInt_Connection *conn, 
	DBInt_Statement *stm, 
	unsigned int index
)
{
	const char	*columnName = NULL;
	switch (conn->dbType) {
		case SODIUM_ORACLE_SUPPORT: {
			columnName = oracleGetColumnNameByIndex(conn, stm, index);
			break;
		}
		case SODIUM_POSTGRESQL_SUPPORT: {
			columnName = postgresqlGetColumnNameByIndex(conn, stm, index);
			break;
		}
		case SODIUM_MYSQL_SUPPORT: {
			columnName = mysqlGetColumnNameByIndex(conn, stm, index);
			break;
		}
	}
	return columnName;
}


DBINTERFACE_API 
int 
DBInt_GetColumnSize(
	DBInt_Connection * conn, 
	DBInt_Statement * stm, 
	const char * columnName
) 
{
	int ret = 0;
	if (conn && columnName) {
		switch (conn->dbType) {
			case SODIUM_ORACLE_SUPPORT: {
				ret = oracleGetColumnSize(conn, stm, columnName);
				break;
			}
			case SODIUM_POSTGRESQL_SUPPORT: {
				ret = postgresqlGetColumnSize(conn, stm, columnName);
				break;
			}
			case SODIUM_MYSQL_SUPPORT: {
				ret = mysqlGetColumnSize(conn, stm, columnName);
				break;
			}
		}
	}
	return ret;
}


DBINTERFACE_API 
SODIUM_DATABASE_COLUMN_TYPE 
DBInt_GetColumnType(
	DBInt_Connection * conn, 
	DBInt_Statement *stm, 
	const char *columnName
) 
{
	SODIUM_DATABASE_COLUMN_TYPE ret = HTSQL_COLUMN_TYPE_NOTSET;
	if (conn && columnName) {
		switch (conn->dbType) {
			case SODIUM_ORACLE_SUPPORT: {
				ret = oracleGetColumnType(conn, stm, columnName);
				break;
			}
			case SODIUM_POSTGRESQL_SUPPORT: {
				ret = postgresqlGetColumnType(conn, stm, columnName);
				break;
			}
			case SODIUM_MYSQL_SUPPORT: {
				ret = mysqlGetColumnType(conn, stm, columnName);
				break;
			}
		}
	}
	return ret;
}