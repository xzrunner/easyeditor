#include "JsonResChangeDirOP.h"

#include <gum/FilepathHelper.h>

namespace edb
{

JsonResChangeDirOP::JsonResChangeDirOP(const std::string& filepath, 
									   const std::string& old_dir, 
									   const std::string& new_dir)
	: JsonResOP(filepath)
	, m_old_dir(old_dir)
	, m_new_dir(new_dir)
{
}

bool JsonResChangeDirOP::OnDoFile(const std::string& filepath, 
								  Json::Value& val,
								  const std::string& key)
{
	std::string old_relative = val[key].asString();
	std::string absolute_path = m_old_dir + "\\" + old_relative;
	absolute_path = gum::FilepathHelper::Format(absolute_path);	
	std::string new_relative = gum::FilepathHelper::Relative(m_new_dir, absolute_path);
 	val[key] = new_relative;
 	return true;
}

}