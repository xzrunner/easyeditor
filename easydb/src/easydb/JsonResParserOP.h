#ifndef _EASYDB_JSON_RES_PARSER_OP_H_
#define _EASYDB_JSON_RES_PARSER_OP_H_

#include "JsonResOP.h"

#include <set>

namespace edb
{

class Database;

class JsonResParserOP : public JsonResOP
{
public:
	JsonResParserOP(const std::string& filepath, const Database& db,
		std::string& export_name, std::set<int>& output);

protected:
	virtual void Clear() override;

	virtual bool OnDoFile(const std::string& filepath, Json::Value& val, const std::string& key) override;

	virtual void BeforeDoCommon(const Json::Value& value) override;
	virtual void BeforeDoAinm(const Json::Value& value) override;

private:
	void SetExportName(const Json::Value& value);

private:
	const Database& m_db;

	std::string& m_export_name;
	
	std::set<int>& m_output;

}; // JsonResParserOP

}

#endif // _EASYDB_JSON_RES_PARSER_OP_H_