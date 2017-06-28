#include "JsonResParserOP.h"
#include "Database.h"

#include <gum/StringHelper.h>

namespace edb
{

JsonResParserOP::JsonResParserOP(const std::string& filepath, const Database& db,
								 std::string& export_name, std::set<int>& output)
	: JsonResOP(filepath)
	, m_db(db)
	, m_export_name(export_name)
	, m_output(output)
{
}

void JsonResParserOP::Clear()
{
	m_export_name.clear();
	m_output.clear();
}

bool JsonResParserOP::OnDoFile(const std::string& filepath, Json::Value& val, const std::string& key)
{
	// todo: out pkg
	if (filepath.find(m_db.GetDirPath()) == std::string::npos) {
		return false;
	}

	int id = m_db.QueryByPath(filepath);
	m_output.insert(id);
	return false;
}

void JsonResParserOP::BeforeDoCommon(const Json::Value& value)
{
	SetExportName(value);
}

void JsonResParserOP::BeforeDoAinm(const Json::Value& value)
{
	SetExportName(value);
}

void JsonResParserOP::SetExportName(const Json::Value& value)
{
	static const std::string KEY_EXPORT_NAME = "name";
	m_export_name = value[KEY_EXPORT_NAME].asString();
	gum::StringHelper::ToLower(m_export_name);
}

}