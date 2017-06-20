#ifndef _EASYDB_JSON_RES_CHANGE_PATH_OP_H_
#define _EASYDB_JSON_RES_CHANGE_PATH_OP_H_

#include "JsonResOP.h"

namespace edb
{

class JsonResChangePathOP : public JsonResOP
{
public:
	JsonResChangePathOP(const std::string& filepath, const std::string& old_path, 
		const std::string& new_path);

protected:
	virtual bool OnDoFile(const std::string& filepath, Json::Value& val, const std::string& key);

private:
	const std::string& m_old_path;
	const std::string& m_new_path;

}; // JsonResChangePathOP

}

#endif // _EASYDB_JSON_RES_CHANGE_PATH_OP_H_