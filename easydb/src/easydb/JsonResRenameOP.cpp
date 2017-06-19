#include "JsonResRenameOP.h"

#include <gum/FilepathHelper.h>

namespace edb
{

JsonResRenameOP::JsonResRenameOP(const std::string& filepath, const std::string& old_name, 
								 const std::string& new_name)
	: JsonResOP(filepath)
	, m_old_name(old_name)
	, m_new_name(new_name)
{
}

bool JsonResRenameOP::OnDoFile(const std::string& filepath, Json::Value& val, const std::string& key)
{
	if (gum::FilepathHelper::Filename(filepath) == m_old_name) {
		std::string absolute = gum::FilepathHelper::Dir(filepath) + "\\" + m_new_name;
		std::string relative = gum::FilepathHelper::Relative(m_base_dir, absolute);
		val[key] = relative;
		return true;
	} else {
		return false;
	}
}

}