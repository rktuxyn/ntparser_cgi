/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
//2:09 AM 11/18/2018 START
//4:11 AM 11/18/2018 END
#pragma warning(disable : 4996)
#if !defined(_global_h)
#include "global_h.h"
#endif//!_global_h
#if !defined(_util_h)
#include "util.h"
#endif
#if !defined(_pg_sql_h)
#include "npgsql/pg_sql.h"
#endif
using namespace std;
using namespace NTemplate::Compiler;
int main(int argc, char *argv[], char*envp[]) {

	char* CONTENT_TYPE = get_content_type();
	std::cout << "Content-Type: " << CONTENT_TYPE << "\n\n" << "\r\n";
	req_method method = determine_req_method();
	if (method == req_method::UNSUPPORTED) {
		//printf("Status-Code: 405-Method Not Allowed\n\n");
		std::cout << "This method " << get_env("REQUEST_METHOD") << "not supported!!!" << "\r\n";
		return EXIT_SUCCESS;
	}

	char* PATH_INFO = get_env("PATH_INFO");
	char* HTTP_COOKIE = get_env("HTTP_COOKIE");
	char* REMOTE_ADDR = get_env("REMOTE_ADDR");
	char* PATH_TRANSLATED = get_env("PATH_TRANSLATED");
	char* execute_path = argv[0];
	std::string ex_dir;
	std::string ex_name;
	split_file_name(execute_path, ex_dir, ex_name);
	ex_dir = ex_dir + "\\";
	std::string root_dir;
	server_physical_path(PATH_TRANSLATED, PATH_INFO, root_dir);
	auto query_string = read_query_string();

	if (method == req_method::POST) {
		int ilen = atoi(get_env("CONTENT_LENGTH"));
		char* bufp = new char[ilen + 1];
		size_t size = sizeof bufp;
		malloc(size);
		fread(bufp, size, 1, stdin);
		printf("The POST data is<P>%s\n", bufp);
		free(bufp);
		return EXIT_SUCCESS;
	}

	template_result* tr = new template_result();
	parser_settings* ps = new parser_settings();
	root_dir = root_dir + "\\";
	ps->dir = root_dir.c_str();
	ps->page_path = PATH_INFO;
	tr->remove_new_line = false;
	ntemplate_parse_x(ps, tr);
	if (tr->is_error == true) {
		printf(tr->err_msg.c_str());
		//print_envp(envp);
		delete tr; delete ps;
		return EXIT_SUCCESS;
	}
	else if (tr->is_script_template == true) {
		run_template_x(tr, "{MY__DATA:1}", ex_dir.c_str());
	}
	printf(tr->t_source.c_str());
	delete tr; delete ps;

	/*[PostgreSQL 9.5.2, compiled by Visual C++ build 1800, 64-bit]*/
	char* pg_sql_bin = get_env("PG_SQL");
	std::string pgsql_bin(pg_sql_bin);
	//std::string pgsql_bin("C:\\Program Files\\PostgreSQL\\9.5\\bin");
	pgsql_bin += "\\libpq.dll";
	pg_sql* pgsql = new pg_sql();
	const char* pg_conn = "Server=localhost; Port=5432; UserId=postgres;Password=pg1234;Database=sow_crypto; keepalive=10; CommandTimeout=100000;";
	int rec = pgsql->init(pgsql_bin.c_str());
	if (rec < 0) {
		std::cout << "UNABLE TO LOAD PGSQL LIB==>" << pgsql_bin << "<br/>";
	}
	else {
		rec = pgsql->connect(pg_conn);
		if (rec < 0) {
			std::cout << "Unable to connect db==>" << pgsql_bin << "<br/>";
		};
		//int* result = 0;
		//pgsql->execute_scalar("select count(*) from auth.login", result);
		pgsql->exit_nicely();
		//std::cout << result << "\r\n";
	};
	
	free(pgsql);
	
	//std::cout << "UNABLE TO LOAD PGSQL LIB==>" << pgsql_bin << "<br/>";
	/*[/PostgreSQL 9.5.2, compiled by Visual C++ build 1800, 64-bit]*/
	//print_envp(envp);
	return 0;
};