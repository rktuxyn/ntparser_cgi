/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#include "util.h"
//9:11 PM 11/18/2018
/*std::string get_current_working_dir(void) {
	char buff[FILENAME_MAX];
	get_current_dir(buff, FILENAME_MAX);
	std::string current_working_dir(buff);
	return current_working_dir;
};*/
const char* get_current_working_dir(void) {
	static char cwd[FILENAME_MAX];
	if (get_current_dir(cwd, FILENAME_MAX) == NULL)
		*cwd = '\x0';
	return cwd;
}
char *get_env(const char* var_name) {
	char *env_val;
	size_t len;
	errno_t err = _dupenv_s(&env_val, &len, var_name);
	return env_val;
};
void print_envp(char*envp[]) {
	int count = 0;
	for (++envp; *envp; ++envp) {
		// use *argv, or if you don't like:
		char* a = *envp;
		std::cout << a << "<br/>";
		count++;
		// use a...
	}
};
int print_env_var(char* val, const char* env) {
	if (val != nullptr) {
		std::cout << env << "==>" << val << "<br/>";
		return 1;
	}
	std::cout << env << "==>Not Found!!!" << "<br/>";
	return -1;
};
void split_file_name (const std::string& path_str, std::string&dir, std::string&file_name) {
	size_t found;
	found = path_str.find_last_of("/\\");
	dir = path_str.substr(0, found);
	file_name = path_str.substr(found + 1);
};
void server_physical_path(const std::string& path_str, const std::string& path_info, std::string &root_dir) {
	std::string p_i = std::regex_replace(std::string(path_info), std::regex("(?:/)"), "\\");
	root_dir = path_str.substr(0, path_str.find(p_i));
};
req_method determine_req_method(void) {
	char* REQUEST_METHOD = get_env("REQUEST_METHOD");
	if (REQUEST_METHOD == NULL)
		return req_method::UNSUPPORTED;
	if (strcmp(REQUEST_METHOD, "GET") == 0)
		return req_method::GET;
	if (strcmp(REQUEST_METHOD, "POST") == 0)
		return req_method::POST;
	return req_method::UNSUPPORTED;
};
char* get_content_type(void) {
	char* CONTENT_TYPE = get_env("CONTENT_TYPE");
	if ((CONTENT_TYPE != NULL) && (CONTENT_TYPE[0] == '\0')) {
		return new char[10]{ "text/html" };
	}
	return CONTENT_TYPE;
};
std::map<std::string, std::string> read_query_string(void) {
	std::string query(get_env("QUERY_STRING"));
	std::map<std::string, std::string> data;
	if (query.empty())return data;
	std::regex pattern("([\\w+%]+)=([^&]*)");
	auto words_begin = std::sregex_iterator(query.begin(), query.end(), pattern);
	auto words_end = std::sregex_iterator();
	for (std::sregex_iterator i = words_begin; i != words_end; i++) {
		std::string key = (*i)[1].str();
		std::string value = (*i)[2].str();
		data[key] = value;
	}
	return data;
}
void json_parse(const char * json_str, std::list<std::map<std::string, std::string>>& json_obj) {
#if defined(_WIN64)
	//
#endif//!_WIN64
	std::regex pattern("([\\w+%]+):([^,]*)");
};
void json_stringify(std::list<std::map<std::string, std::string>>& json_obj, std::string & json_str) {
	json_str = "[\r\n";
	for (auto s = json_obj.begin(); s != json_obj.end(); ++s) {
		json_str += "{\r\n";
		auto _map = *s;
		for (auto itr = _map.begin(); itr != _map.end(); itr++) {
			json_str += itr->first;
			json_str += ":";
			json_str += itr->second;
		}
		json_str += "}\r\n";
	}
	json_str += "]";
}
;