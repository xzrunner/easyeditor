#ifndef _EASYDB_JSON_RES_CHANGE_DIR_OP_H_
#define _EASYDB_JSON_RES_CHANGE_DIR_OP_H_

#include "JsonResOP.h"

namespace edb
{

class JsonResChangeDirOP : public JsonResOP
{
public:
	JsonResChangeDirOP(const std::string& filepath, const std::string& old_dir, 
		const std::string& new_dir);

protected:
	virtual bool OnDoFile(const std::string& filepath, Json::Value& val, const std::string& key);

private:
	const std::string& m_old_dir;
	const std::string& m_new_dir;

}; // JsonResChangeDirOP

}

#endif // _EASYDB_JSON_RES_CHANGE_DIR_OP_H_