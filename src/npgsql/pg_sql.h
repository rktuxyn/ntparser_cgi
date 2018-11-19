/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
//2:16 AM 11/19/2018
//#pragma warning(suppress : 4996)
#pragma warning(disable : 4996)
#pragma once
#if !defined(_global_h)
#include "../global_h.h"
#endif//!_global_h
#if !defined(_os_support_h)
#include "../os_support.h"
#endif
#include <libpq-fe.h>
#if !defined(_pg_sql_h)
#define _pg_sql_h
#if !(defined(_WIN32)||defined(_WIN64)) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
#define LIBPQ_C_DLL				"libpq.so.5"
#else
#define LIBPQ_C_DLL				"libpq.dll"
#endif
#define LIBPQ_DLL_LOAD_ERROR	"Error loading PostgreSQL " LIBPQ_C_DLL ": " 
//#include "pg_sql_parameters.h"
// libpq C library functions
typedef void (*PQclearFunc)(PGresult *res);
typedef char* (*PQerrorMessageFunc)(const PGconn *conn);
typedef PGresult* (*PQexecFunc)(PGconn *conn, const char *query);
typedef int (*PQfmodFunc)(const PGresult *res, int column_number);
typedef char* (*PQfnameFunc)(const PGresult *res, int column_number);
typedef int (*PQfsizeFunc)(const PGresult *res, int column_number);
typedef Oid (*PQftypeFunc)(const PGresult *res, int column_number);
typedef int (*PQgetisnullFunc)(const PGresult *res, int row_number, int column_number);
typedef int (*PQgetlengthFunc)(const PGresult *res, int row_number, int column_number);
typedef PGresult* (*PQgetResultFunc)(PGconn *conn);
typedef char* (*PQgetvalueFunc)(const PGresult *res, int row_number, int column_number);
typedef int (*PQnfieldsFunc)(const PGresult *res);
typedef int (*PQntuplesFunc)(const PGresult *res);
typedef int (*PQputCopyDataFunc)(PGconn *conn, const char *buffer, int nbytes);
typedef int (*PQputCopyEndFunc)(PGconn *conn, const char *errormsg);
typedef char* (*PQresultErrorMessageFunc)(const PGresult *res);
typedef ExecStatusType (*PQresultStatusFunc)(const PGresult *res);
typedef int	(*PQsetClientEncodingFunc)(PGconn *conn, const char *encoding);
typedef PGconn* (*PQsetdbLoginFunc)(const char *pghost, const char *pgport, const char *pgoptions, const char *pgtty, const char *dbName, const char *login, const char *pwd);
typedef ConnStatusType (*PQstatusFunc)(const PGconn *conn);
typedef PGconn* (*PQconnectdbFunc)(const char *conninfo);
typedef void (*PQfinishFunc)(PGconn *conn);
#define PG_DT_VARCHAR		1043	
#define PG_DT_BPCHAR		1042    // Blank padded CHAR
#define PG_DT_INT2			21		// SMALLINT	
#define PG_DT_INT4			23
#define PG_DT_NUMERIC		1700	// NUMERIC and DECIMAL
#define PG_DT_DATE			1082
#define PG_DT_FLOAT			701
#if !(defined(_WIN32)||defined(_WIN64)) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
typedef void* h_get_proc_iddl;
typedef void* h_module;
#else
typedef HINSTANCE h_get_proc_iddl;
typedef HMODULE h_module;
#endif
class pg_sql {
private:
	bool _connected;
	// PostgreSQL connection
	PGconn *_conn;
	// Open cursor result
	PGresult *_pg_result;
	// Number of rows already fetched to buffers
	int _cursor_rows_fetched;
	// Number of columns for COPY command
	size_t _copy_cols_count;
	// PostgreSQL libpq C library HINSTANCE
	h_get_proc_iddl _pgsql_proc_iddl;
	// PostgreSQL libpq C library DDL
	h_module _pgsql_module;
	// libpq C library functions
	PQclearFunc _PQclear;
	PQerrorMessageFunc _PQerrorMessage;
	PQexecFunc _PQexec;
	PQfmodFunc _PQfmod;
	PQfnameFunc _PQfname;
	PQfsizeFunc _PQfsize;
	PQftypeFunc _PQftype;
	PQgetisnullFunc _PQgetisnull;
	PQgetlengthFunc _PQgetlength;
	PQgetResultFunc _PQgetResult;//
	PQgetvalueFunc _PQgetvalue;//
	PQnfieldsFunc _PQnfields;//
	PQntuplesFunc _PQntuples;//
	PQputCopyDataFunc _PQputCopyData;
	PQputCopyEndFunc _PQputCopyEnd;
	PQresultErrorMessageFunc _PQresultErrorMessage;
	PQresultStatusFunc _PQresultStatus;
	PQsetClientEncodingFunc _PQsetClientEncoding;
	PQsetdbLoginFunc _PQsetdbLogin;
	PQconnectdbFunc _PQconnectdb;
	PQfinishFunc _PQfinish;
	PQstatusFunc _PQstatus;
	// Error information
	int _error;
	char _error_text[1024];
	// Native error information
	int _native_error;
	char* _native_error_text;
public:
	pg_sql();
	~pg_sql();
	// Initialize API
	virtual int init(const char* lib_path);
	// Connect to the database
	virtual int connect(const char *conn);
	virtual void exit_nicely();
	// Initialize session
	//void init_session();
	// Get row count for the specified object
	virtual int get_row_count(const char *object, int *count);
	// Execute the statement and get scalar result
	virtual int execute_scalar(const char *query, int *result);
	// Execute the statement
	virtual int execute_non_query(const char *query);
private:
	virtual void parse_connection_string(const char *conn, std::string& user, std::string& pwd, std::string& server, std::string& port, std::string& db);
#if !(defined(_WIN32)||defined(_WIN64)) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
	virtual int load_pgsql_lib(const char* name);
#else
	virtual int load_pgsql_lib(const char* name);
#endif
	// Free connection and environment handles 
	void deallocate();
	// Find PostgreSQL installation paths
	void find_pg_paths(std::list<std::string> &paths);
	// Set error code and message for the last API call
	void set_error();
};
//5:30 AM 11/19/2018 Connected
//7:55 AM 11/19/2018 END
#endif//!_pg_sql_h
