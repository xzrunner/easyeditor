#include "JsonResChangePathOP.h"

#include <gum/FilepathHelper.h>

namespace edb
{

JsonResChangePathOP::JsonResChangePathOP(const std::string& filepath, 
										 const std::string& old_path, 
										 const std::string& new_path)
	: JsonResOP(filepath)
	, m_old_path(old_path)
	, m_new_path(new_path)
{
}

bool JsonResChangePathOP::OnDoFile(const std::string& filepath, 
								   Json::Value& val, 
								   const std::string& key)
{
	if (filepath == m_old_path) {
		std::string relative = gum::FilepathHelper::Relative(m_base_dir, m_new_path);
		val[key] = relative;
		return true;
	} else {
		return false;
	}
}

}