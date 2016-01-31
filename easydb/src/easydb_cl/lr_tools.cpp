#include "lr_tools.h"



namespace edb
{

std::string get_lr_name_from_file(const std::string& filepath)
{
	std::string name = ee::FileHelper::GetFilename(filepath);
	size_t pos = name.find_last_of(".");
	if (pos == std::string::npos) {
		pos = name.find_last_of("_lr") - 2;
	}
	name = name.substr(0, pos);
	return name;
}

}