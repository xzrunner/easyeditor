#ifndef _EASYDB_JSON_RES_DEL_OP_H_
#define _EASYDB_JSON_RES_DEL_OP_H_

#include "JsonResOP.h"

namespace edb
{

class JsonResDelOP : public JsonResOP
{
public:
	JsonResDelOP(const std::string& filepath, const std::string& rm_filepath);

protected:
	virtual bool OnDoFile(const std::string& filepath, Json::Value& val, const std::string& key) override;

private:
	const std::string& m_rm_filepath;

}; // JsonResDelOP

}

#endif // _EASYDB_JSON_RES_DEL_OP_H_