
// Parameters class - Loads parameters from command line  
#pragma warning(disable : 4996)
#include <stdio.h> 
#include <string.h>
#include <string>
#include <algorithm>
#include "pg_sql_parameters.h"
//#include "file.h"
#include "os_support.h"
#include <string>
#if defined(WIN32) || defined(WIN64)
#define DIR_SEPARATOR_CHAR '\\'
#define DIR_SEPARATOR_STR  "\\"
struct _finddata_t;
#else
#define DIR_SEPARATOR_CHAR '/'
#define DIR_SEPARATOR_STR  "/"
#endif

static bool is_set(const char *in) { if (in != NULL && *in != '\x0') return true; else return false; }
static char* skip_spaces(const char *input) {
	if (input == NULL)
		return NULL;

	const char *cur = input;

	while (*cur) {
		if (*cur != L' ' && *cur != L'\r' && *cur != L'\n' && *cur != L'\t')
			break;

		cur++;
	}

	return (char*)cur;
};
static void trim_trailing_spaces(std::string &input) {
	// Get the string length
	size_t len = input.length();
	size_t nlen = len;

	if (len <= 0)
		return;

	// Calculate the number of spaces at the end
	while (nlen && input[nlen - 1] == L' ')
		nlen--;

	if (nlen < len)
		input.erase(nlen);
};
static char* skip_comments(const char *input) {
	if (input == NULL)
		return NULL;

	char *cur = (char*)input;

	while (*cur) {
		cur = skip_spaces(cur);

		// Single line comment
		if ((*cur == '-' && cur[1] == '-') || (*cur == '/' && cur[1] == '/')) {
			while (*cur && *cur != '\n')
				cur++;
		}
		else
			break;
	}

	return cur;
};

// Constructor
Parameters::Parameters() {
	_cfg_file = NULL;
	_cfg_read_ok = false;
	*_cfg_read_err = '\x0';
};

// Return the value of the parameter if it is set to True or Yes
const char* Parameters::GetTrue(const char *name) {
	const char *value = Get(name);

	if (value != NULL && *value != '\x0' &&
		(_stricmp(value, "yes") == 0 || _stricmp(value, "true") == 0))
		return value;

	return NULL;
};

// Return the value of the parameter if it is set to False or No
const char* Parameters::GetFalse(const char *name) {
	const char *value = Get(name);

	if (value != NULL && *value != '\x0' &&
		(_stricmp(value, "no") == 0 || _stricmp(value, "false") == 0))
		return value;

	return NULL;
};

// Get integer value or the default
int Parameters::GetInt(const char *name, int def) {
	const char *value = Get(name);

	if (is_set(value)) {
		int num = def;
		sscanf(value, "%d", &num);

		if (num != def)
			return num;
	}

	return def;
};
// Process parameters in command line
int Parameters::Load(int argc, char** argv) {
	// Load command line parameters
	int rc = LoadCommandLine(argc, argv);

	return rc;
};

// Load command line parameters
int Parameters::LoadCommandLine(int argc, char** argv) {
	int rc = 0;

	// Check if any parameters are specified
	if (argc == 1 || argv == NULL)
		return -1;

	std::string comline;

	// Merge all command line parameters into a single string
	for (int i = 1; i < argc; i++) {
		comline += argv[i];
		comline += " ";
	}

	// Load parameters from the command line string
	rc = LoadStr(comline.c_str());

	return rc;
};

// Load parameters from string and configuration file
int Parameters::LoadStr(const char *input) {
	// NULL input is allowed to read from configuration file only

	// Delete all current parameters
	_map.clear();

	// Load from string
	if (input != NULL)
		LoadStrItem(input);

	// Load from configuration file
	//LoadConfigFile();

	return 0;
};

// Load parameters from a string only
int Parameters::LoadStrItem(const char *input) {
	if (input == NULL)
		return -1;

	const char *cur = input;

	// Process parameters
	while (*cur) {
		std::string name;
		std::string value;

		cur = skip_spaces(cur);

		// Check for help option specified as ? or /?
		if (*cur == '?' || (*cur == '-' && cur[1] == '?')) {
			// Add help option and ignore all remaining options
			_map.insert(ParametersPair(HELP_PARAMETER, "yes"));
			break;
		}

		// Get the parameter name until = or new line
		while (*cur && *cur != '=' && *cur != '\r' && *cur != '\n' && *cur != '\t') {
			// Check for parameter without value
			if (*cur == PARAMETER_START_TOKEN && name.empty() == false)
				break;

			name += *cur;
			cur++;
		}

		trim_trailing_spaces(name);

		if (*cur == '=')
			cur++;

		cur = skip_spaces(cur);

		// Get the parameter value until - or new line
		while (*cur && *cur != '\r' && *cur != '\n' && *cur != '\t') {
			// if - is faced, previuos must be space, next mustn't be space (to allow - in directory path, host names etc.)
			if (*cur == PARAMETER_START_TOKEN && cur[-1] == ' ' && cur[1] != ' ')
				break;

			value += *cur;
			cur++;
		}

		trim_trailing_spaces(value);

		// Parameter names are case-insensitive, convert to lower case
		std::transform(name.begin(), name.end(), name.begin(), ::tolower);

		// Don't add parameter if it already exists (give priority to command line and parameters defined first)
		if (_map.count(name) == 0) {
			_map.insert(ParametersPair(name, value));
		}
	}

	return 0;
};
// Get the value by key
char* Parameters::Get(const char *key){
	if(key == NULL)
		return NULL;

	std::string skey = key;

	// Find the value
	ParametersMap::iterator i = _map.find(skey);

	// Key not found
	if(i == _map.end())
		return NULL;

	return (char*)i->second.c_str();
};
