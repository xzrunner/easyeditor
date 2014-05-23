#include "check_params.h"
#include "ICommand.h"

#include <wx/filename.h>
#include <iostream>

namespace edb
{

bool check_number(const ICommand* cmd, int argc, int min)
{
	if (argc >= min) {
		return true;
	}

	std::cerr << "Need more params" << std::endl;
	std::cerr << cmd->Usage() << std::endl;
	return false;
}

bool check_folder(const char* dirpath)
{
	if (wxFileName::DirExists(dirpath)) {
		return true;
	}

	std::cerr << "error dir: " << dirpath << std::endl;
	return false;
}

bool check_json_key(const Json::Value& value, const std::string& key,
					const std::string& filename)
{
	if (!value[key].isNull()) {
		return true;
	}

	std::cerr << "error key! file: " << filename << ", key: " << key << std::endl;
	return false;
}

}