#ifndef _EASYDB_CHECK_PARAMS_H_
#define _EASYDB_CHECK_PARAMS_H_

#include <json/json.h>

namespace edb
{

class ICommand;

bool check_number(const ICommand* cmd, int argc, int min);

bool check_folder(const char* dirpath);

bool check_file(const char* filepath);

bool check_json_key(const Json::Value& value, const std::string& key, 
					const std::string& filename);

bool check_params(const char* params, const char* short_cmd, const char* long_cmd);

}

#endif // _EASYDB_CHECK_PARAMS_H_