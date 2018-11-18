#include "util.h"
//9:11 PM 11/18/2018
std::string GetCurrentWorkingDir(void) {
	char buff[FILENAME_MAX];
	GetCurrentDir(buff, FILENAME_MAX);
	std::string current_working_dir(buff);
	return current_working_dir;
};
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
req_method determine_req_method() {
	char* REQUEST_METHOD = get_env("REQUEST_METHOD");
	if (REQUEST_METHOD == NULL)
		return req_method::UNSUPPORTED;
	if (strcmp(REQUEST_METHOD, "GET") == 0)
		return req_method::GET;
	if (strcmp(REQUEST_METHOD, "POST") == 0)
		return req_method::POST;
	return req_method::UNSUPPORTED;
};
char* get_content_type() {
	char* CONTENT_TYPE = get_env("CONTENT_TYPE");
	if ((CONTENT_TYPE != NULL) && (CONTENT_TYPE[0] == '\0')) {
		return new char[10]{ "text/html" };
	}
	return CONTENT_TYPE;
};
std::map<std::string, std::string> read_query_string() {
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
};