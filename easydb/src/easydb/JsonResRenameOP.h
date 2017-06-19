#ifndef _EASYDB_JSON_RES_RENAME_OP_H_
#define _EASYDB_JSON_RES_RENAME_OP_H_

#include "JsonResOP.h"

namespace edb
{

class JsonResRenameOP : public JsonResOP
{
public:
	JsonResRenameOP(const std::string& filepath, const std::string& old_name, const std::string& new_name);

protected:
	virtual bool OnDoFile(const std::string& filepath, Json::Value& val, const std::string& key);

private:
	const std::string& m_old_name;
	const std::string& m_new_name;

}; // JsonResRenameOP

}

#endif // _EASYDB_JSON_RES_RENAME_OP_H_