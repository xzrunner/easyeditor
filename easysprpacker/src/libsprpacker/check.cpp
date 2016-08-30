#include "check.h"

namespace erespacker
{

bool is_name_valid(const std::string& name)
{
	return !name.empty() && name[0] != '_';
}

}