/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#include "pg_sql.h"
LPWSTR s2ws(const char* s) {
	wchar_t wtext[FILENAME_MAX];
	mbstowcs(wtext, s, strlen(s) + 1);//Plus null
	LPWSTR ptr = wtext;
	return ptr;
};
pg_sql::pg_sql() {
	_pg_result = NULL; _conn = NULL; _pgsql_proc_iddl = NULL; _pgsql_module = NULL;
	_PQclear = NULL; _PQerrorMessage = NULL; _PQexec = NULL; _PQfmod = NULL;
	_PQfname = NULL; _PQfsize = NULL; _PQftype = NULL; _PQgetisnull = NULL;
	_PQgetlength = NULL; _PQgetResult = NULL; _PQgetvalue = NULL; _PQnfields = NULL;
	_PQntuples = NULL; _PQputCopyData = NULL; _PQputCopyEnd = NULL; _PQresultErrorMessage = NULL;
	_PQresultStatus = NULL; _PQsetClientEncoding = NULL; _PQsetdbLogin = NULL; _PQstatus = NULL;
	_PQconnectdb = NULL;_PQfinish = NULL; 
	_native_error_text = new char;//'\x0'
};
pg_sql::~pg_sql() {
	_pg_result = NULL; _conn = NULL; _pgsql_proc_iddl = NULL; _pgsql_module = NULL;
	_PQclear = NULL; _PQerrorMessage = NULL; _PQexec = NULL; _PQfmod = NULL;
	_PQfname = NULL; _PQfsize = NULL; _PQftype = NULL; _PQgetisnull = NULL;
	_PQgetlength = NULL; _PQgetResult = NULL; _PQgetvalue = NULL; _PQnfields = NULL;
	_PQntuples = NULL; _PQputCopyData = NULL; _PQputCopyEnd = NULL; _PQresultErrorMessage = NULL;
	_PQresultStatus = NULL; _PQsetClientEncoding = NULL; _PQsetdbLogin = NULL; _PQstatus = NULL;
	_PQconnectdb = NULL;_PQfinish = NULL; 
};

// Initialize API
int pg_sql::init(const char* lib_path) {
	if (_pgsql_module == NULL) {
		// Try to load the library by default path
		//_pgsql_module = LoadLibraryEx(L"C:\\Program Files\\PostgreSQL\\9.5\\bin\\libpq.dll", NULL, 0x00000008);
#if !(defined(_WIN32)||defined(_WIN64)) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
		_pgsql_module = dlopen(lib_path, RTLD_NOW); 
#else
		_pgsql_module = LoadLibraryEx(s2ws(lib_path), NULL, 0x00000008);
#endif
		// DLL load failed
		if (_pgsql_module == NULL) {
			return -1;
		}
	}
	// Get functions
	if (_pgsql_module != NULL) {
		_PQclear = (PQclearFunc)os_support::get_proc_address(_pgsql_module, "PQclear");
		_PQerrorMessage = (PQerrorMessageFunc)os_support::get_proc_address(_pgsql_module, "PQerrorMessage");
		_PQexec = (PQexecFunc)os_support::get_proc_address(_pgsql_module, "PQexec");
		_PQfmod = (PQfmodFunc)os_support::get_proc_address(_pgsql_module, "PQfmod");
		_PQfname = (PQfnameFunc)os_support::get_proc_address(_pgsql_module, "PQfname");
		_PQfsize = (PQfsizeFunc)os_support::get_proc_address(_pgsql_module, "PQfsize");
		_PQftype = (PQftypeFunc)os_support::get_proc_address(_pgsql_module, "PQftype");
		_PQgetisnull = (PQgetisnullFunc)os_support::get_proc_address(_pgsql_module, "PQgetisnull");
		_PQgetlength = (PQgetlengthFunc)os_support::get_proc_address(_pgsql_module, "PQgetlength");
		_PQgetResult = (PQgetResultFunc)os_support::get_proc_address(_pgsql_module, "PQgetResult");
		_PQgetvalue = (PQgetvalueFunc)os_support::get_proc_address(_pgsql_module, "PQgetvalue");
		_PQnfields = (PQnfieldsFunc)os_support::get_proc_address(_pgsql_module, "PQnfields");
		_PQntuples = (PQntuplesFunc)os_support::get_proc_address(_pgsql_module, "PQntuples");
		_PQputCopyData = (PQputCopyDataFunc)os_support::get_proc_address(_pgsql_module, "PQputCopyData");
		_PQputCopyEnd = (PQputCopyEndFunc)os_support::get_proc_address(_pgsql_module, "PQputCopyEnd");
		_PQresultErrorMessage = (PQresultErrorMessageFunc)os_support::get_proc_address(_pgsql_module, "PQresultErrorMessage");
		_PQresultStatus = (PQresultStatusFunc)os_support::get_proc_address(_pgsql_module, "PQresultStatus");
		_PQsetClientEncoding = (PQsetClientEncodingFunc)os_support::get_proc_address(_pgsql_module, "PQsetClientEncoding");
		_PQsetdbLogin = (PQsetdbLoginFunc)os_support::get_proc_address(_pgsql_module, "PQsetdbLogin");
		_PQstatus = (PQstatusFunc)os_support::get_proc_address(_pgsql_module, "PQstatus");
		_PQconnectdb = (PQconnectdbFunc)os_support::get_proc_address(_pgsql_module, "PQconnectdb");
		_PQfinish = (PQfinishFunc)os_support::get_proc_address(_pgsql_module, "PQfinish");

		if (_PQclear == NULL || _PQerrorMessage == NULL || _PQexec == NULL || _PQfmod == NULL ||
			_PQfname == NULL || _PQfsize == NULL || _PQftype == NULL || _PQgetisnull == NULL ||
			_PQgetlength == NULL || _PQgetResult == NULL || _PQgetvalue == NULL || _PQnfields == NULL ||
			_PQntuples == NULL || _PQputCopyData == NULL || _PQputCopyEnd == NULL ||
			_PQresultErrorMessage == NULL || _PQresultStatus == NULL || _PQsetClientEncoding == NULL ||
			_PQsetdbLogin == NULL || _PQstatus == NULL )
			return -1;
	}
	else {
		os_support::get_last_error_text(LIBPQ_DLL_LOAD_ERROR, _native_error_text, 1024);
		return -1;
	}

	return 1;
};
void pg_sql::find_pg_paths(std::list<std::string> &paths) {
#if !(defined(_WIN32)||defined(_WIN64)) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
	return;
#else
	HKEY hKey, hSubkey;
	// At least for PostgreSQL 9.0 
	int rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\PostgreSQL\\Installations"), 0, KEY_READ, &hKey);
	if (rc != ERROR_SUCCESS)
		return;
	char key[1024];
	wchar_t wtext[1024];
	DWORD i = 0;
	bool more = true;
	// Enumerate through all keys
	while (more) {
		// Size modified with each call to RegEnumKeyEx
		int size = 1024;
		// Get next key
		rc = RegEnumKeyEx(hKey, i, wtext, (LPDWORD)&size, NULL, NULL, NULL, NULL);
		if (rc != ERROR_SUCCESS) {
			more = false;
			break;
		}
		i++;
		// At least for version 9.0 key is "postgresql-90"
		if (strncmp(key, "postgresql", 10) != 0)
			continue;
		rc = RegOpenKeyEx(hKey, wtext, 0, KEY_READ, &hSubkey);
		if (rc != ERROR_SUCCESS)
			break;
		char location[1024];
		int value_size = 1024;
		rc = RegQueryValueEx(hSubkey, TEXT("Base Directory"), NULL, NULL, (LPBYTE)location, (LPDWORD)&value_size);
		if (rc == ERROR_SUCCESS)
			paths.push_back(std::string(location));
		RegCloseKey(hSubkey);
	}
	RegCloseKey(hKey);

#endif
};
void pg_sql::deallocate() {};
void pg_sql::parse_connection_string(const char * conn, std::string & user, std::string & pwd, std::string & server, std::string & port, std::string & db) {
	if (conn == NULL)
		return;
	std::string query(conn);
	std::regex pattern("([\\w+%]+)=([^;]*)");
	std::map<std::string, std::string> conn_obj;
	auto words_begin = std::sregex_iterator(query.begin(), query.end(), pattern);
	auto words_end = std::sregex_iterator();
	for (std::sregex_iterator i = words_begin; i != words_end; i++) {
		std::string key = (*i)[1].str();
		std::string value = (*i)[2].str();
		conn_obj[key] = value;
	};
	user = conn_obj["UserId"];
	pwd = conn_obj["Password"];
	db = conn_obj["Database"];
	server = conn_obj["Server"];
	port = conn_obj["Port"];
	conn_obj.clear();
};
int pg_sql::connect(const char *conn) {
	// Check if already connected
	if (_connected == true)
		return 1;
	std::string user, pwd, server, port, db;
	parse_connection_string(conn, user, pwd, server, port, db);
	//_PQsetdbLogin(const char *pghost, const char *pgport, const char *pgoptions, const char *pgtty, const char *dbName, const char *login, const char *pwd)
	_conn = _PQsetdbLogin(
		(server.empty() ? NULL : server.c_str()),
		(port.empty() ? NULL : port.c_str()), NULL, NULL,
		(db.empty() ? NULL : db.c_str()),
		(user.empty() ? NULL : user.c_str()),
		(pwd.empty() ? NULL : pwd.c_str()));
	if (_PQstatus(_conn) != CONNECTION_OK) {
		set_error();
		return -1;
	}
	_connected = true;
	return 1;
};
void pg_sql::exit_nicely() {
	if (!_connected)return;
	_PQfinish (_conn);
	return;
};
// Get row count for the specified object
int pg_sql::get_row_count(const char *object, int *count) {
	if (object == NULL)
		return -1;
	std::string query = "SELECT COUNT(*) FROM ";
	query += object;
	// Execute the query
	int rc = execute_scalar(query.c_str(), count);
	return rc;
};
// Execute the statement and get scalar result
int pg_sql::execute_scalar(const char *query, int *result) {
	if (query == NULL || result == NULL)
		return -1;
	// Execute the statement
	PGresult *res = _PQexec(_conn, query);
	bool exists = false;
	// Get the value of the first column of the first row
	if (_PQresultStatus(res) == PGRES_TUPLES_OK) {
		char *value = _PQgetvalue(res, 0, 0);
		sscanf_s(value, "%d", result);
		exists = true;
	}
	else
		set_error();
	_PQclear(res);
	return (exists == true) ? 0 : -1;
};
// Execute the statement
int pg_sql::execute_non_query(const char *query) {
	if (query == NULL)
		return -1;
	// Execute the query
	PGresult *result = _PQexec(_conn, query);
	bool error = false;
	int rc = _PQresultStatus(result);
	// Error raised
	if (rc != PGRES_COMMAND_OK) {
		set_error();
		error = true;
	}
	_PQclear(result);
	return (error == true) ? -1 : 0;
};
// Set error code and message for the last API call
void pg_sql::set_error() {
	// Get native error text
	char* erro_msg = _PQerrorMessage(_conn);
	_native_error_text = new char[strlen(erro_msg) + 1];
	strcpy_s(_native_error_text, sizeof _native_error_text, erro_msg);
	_error = -1;
	*_error_text = '\x0';
};
#if !(defined(_WIN32)||defined(_WIN64)) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
int pg_sql::load_pgsql_lib(const char* name) {
	if (_pgsql_proc_iddl) {
		return 1;
	}
	_pgsql_proc_iddl = os_support::load_library(name);
	if (!_pgsql_proc_iddl) {
		return -1;
	}
	return 1;
};
#else
int pg_sql::load_pgsql_lib(const char* name) {
	if (_pgsql_proc_iddl) {
		return 1;
	}
	_pgsql_proc_iddl = os_support::load_library(s2ws(name));
	if (!_pgsql_proc_iddl) {
		return -1;
	}
	return 1;

};
#endif