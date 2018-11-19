/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
//9:11 PM 11/18/2018
#pragma once
#if !defined(_global_h)
#include "global_h.h"
#endif
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
#include <unistd.h>
#define get_current_dir getcwd
#else
#include <direct.h>
#define get_current_dir _getcwd
#endif//!_WIN32
#if !defined(_util_h)
#define _util_h
typedef enum {
	GET,
	POST,
	UNSUPPORTED
}req_method;
const char* get_current_working_dir(void);
char *get_env(const char* var_name);
void print_envp(char*envp[]);
int print_env_var(char* val, const char* env);
void split_file_name (const std::string& path_str, std::string&dir, std::string&file_name);
void server_physical_path(const std::string& path_str, const std::string& path_info, std::string &root_dir);
req_method determine_req_method(void);
char* get_content_type(void);
std::map<std::string, std::string> read_query_string(void);
void json_parse(const char* json_str, std::list<std::map<std::string, std::string>>&json_obj);
void json_stringify(std::list<std::map<std::string, std::string>>& json_obj, std::string&json_str);
#endif//!_util_h
/*const char* ENV[50] = {
	"APPDATA",
	"OS",
	"Path",
	"PYTHON",
	"TEMP",
	"HTTP_CACHE_CONTROL",
	"HTTP_CONNECTION",
	"HTTP_ACCEPT",
	"HTTP_ACCEPT_ENCODING",
	"HTTP_ACCEPT_LANGUAGE",
	"HTTP_COOKIE",
	"HTTP_HOST",
	"HTTP_USER_AGENT",
	"HTTP_UPGRADE_INSECURE_REQUESTS",
	"HTTP_DNT",
	"APP_POOL_ID",
	"AUTH_TYPE",
	"AUTH_PASSWORD",
	"AUTH_USER",
	"CERT_COOKIE",
	"CERT_FLAGS",
	"CERT_ISSUER",
	"CERT_SERIALNUMBER",
	"CERT_SUBJECT",
	"CONTENT_LENGTH",
	"CONTENT_TYPE",
	"GATEWAY_INTERFACE",
	"HTTPS",
	"HTTPS_KEYSIZE",
	"HTTPS_SECRETKEYSIZE",
	"HTTPS_SERVER_ISSUER",
	"HTTPS_SERVER_SUBJECT",
	"INSTANCE_ID",
	"LOCAL_ADDR",
	"LOGON_USER",
	"PATH_INFO",
	"PATH_TRANSLATED",
	"QUERY_STRING",
	"REMOTE_ADDR",
	"REMOTE_HOST",
	"REMOTE_USER",
	"REQUEST_METHOD",
	"SCRIPT_NAME",
	"SERVER_NAME",
	"SERVER_PORT",
	"SERVER_PORT_SECURE",
	"SERVER_PROTOCOL",
	"SERVER_SOFTWARE",
	"UNMAPPED_REMOTE_USER",
	"PG_SQL"
};*/
