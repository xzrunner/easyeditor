#include "JsonResDelOP.h"

namespace edb
{

JsonResDelOP::JsonResDelOP(const std::string& filepath, const std::string& rm_filepath)
	: JsonResOP(filepath)
	, m_rm_filepath(rm_filepath)
{
}

bool JsonResDelOP::OnDoFile(const std::string& filepath, Json::Value& val, const std::string& key)
{
	if (filepath == m_rm_filepath) {
		val.removeMember(key);
		return true;
	} else {
		return false;
	}
}

}