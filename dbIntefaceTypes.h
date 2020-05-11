#pragma once

#include <windows.h>

#if defined(DBInt) || defined(SodiumExtension) || defined(SodiumDebugger)
	/*	Oracle Definitions */
	typedef struct OCI_Statement OCI_Statement;
	typedef struct OCI_Resultset OCI_Resultset;
	typedef struct OCI_Connection OCI_Connection;
	
	/*	PostgreSql Definitions */
	typedef struct PGresult	PGresult;
	typedef struct PGconn	PGconn;
	typedef struct Oid	Oid;

	/*	MYSQL Definitions */
	typedef struct MYSQL_BIND MYSQL_BIND;
	typedef struct MYSQL_STMT MYSQL_STMT;
	typedef struct MYSQL MYSQL;
#endif

#ifdef DBIntPostgresql

	#include <libpq-fe.h>
	
	/*	Oracle Definitions */
	typedef struct OCI_Statement OCI_Statement;
	typedef struct OCI_Resultset OCI_Resultset;
	typedef struct OCI_Connection OCI_Connection;

	/*	MYSQL Definitions */
	typedef struct MYSQL_BIND MYSQL_BIND;
	typedef struct MYSQL_STMT MYSQL_STMT;
	typedef struct MYSQL MYSQL;

#endif

#ifdef DBIntOracle

	#include <ocilib.h>

	/*	PostgreSql Definitions */
	typedef struct PGresult	PGresult;
	typedef struct PGconn	PGconn;
	typedef struct Oid	Oid;

	/*	MYSQL Definitions */
	typedef struct MYSQL_BIND MYSQL_BIND;
	typedef struct MYSQL_STMT MYSQL_STMT;
	typedef struct MYSQL MYSQL;

#endif

#ifdef DBIntMySql

	#include <mysql.h>

	/*	Oracle Definitions */
	typedef struct OCI_Statement OCI_Statement;
	typedef struct OCI_Resultset OCI_Resultset;
	typedef struct OCI_Connection OCI_Connection;

	/*	PostgreSql Definitions */
	typedef struct PGresult	PGresult;
	typedef struct PGconn	PGconn;
	typedef struct Oid	Oid;

#endif

typedef enum {
	SODIUM_NOTSET_SUPPORT,
	SODIUM_ORACLE_SUPPORT,
	SODIUM_POSTGRESQL_SUPPORT,
	SODIUM_MYSQL_SUPPORT,
	SODIUM_SQLSERVER_SUPPORT,
	SODIUM_CASSANDRA_SUPPORT,
	SODIUM_MONGODB_SUPPORT,
	SODIUM_MSACCESS_SUPPORT
} DBInt_SupportedDatabaseType;

typedef struct {
	OCI_Statement					*statement;
	OCI_Resultset					*resultset;
	BOOL							eof;		
	int								currentRowNum;
} DBInt_Statement_Oracle;


typedef struct {
	PGresult						*resultSet;
	int								currentRowNum;
	int								bindVariableCount;
	Oid								*paramTypes;
	int								*paramSizes;
	int								*paramFormats;
	void							**bindVariables;	// dynamic array of character array
} DBInt_Statement_Postgresql;
	
typedef struct {
	MYSQL_BIND					  * bindVariables;
	int								paramCount;
	int								bindedVariableCount;
	MYSQL_BIND					  *	rs;
	MYSQL_STMT					  * statement;
	//unsigned long long				currentRowNum;
	BOOL							eof;
	int								bindVariableCount;
	Oid							   * paramTypes;
	int							   * paramFormats;
	unsigned long				   * sizeOfColumns; //An array of unsigned long integers representing the size of each column(not including any terminating null bytes).
} DBInt_Statement_Mysql;

typedef struct _DBInt_Statement  {
	union {
		DBInt_Statement_Oracle		oracle;
		DBInt_Statement_Postgresql	postgresql;
		DBInt_Statement_Mysql		mysql;
	} statement;
} DBInt_Statement;

	
typedef struct {
	char					  hostName[15];
	DBInt_SupportedDatabaseType		dbType;
	union {
		OCI_Connection	*oracleHandle;
		PGconn			*postgresqlHandle;
		MYSQL			*mysqlHandle;
	} connection;
	HANDLE                      heapHandle;
	const char	 				  *errText;
	BOOL		 				       err;
} DBInt_Connection;


