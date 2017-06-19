#ifndef _EASYDB_JSON_RES_FIX_OP_H_
#define _EASYDB_JSON_RES_FIX_OP_H_

#include "JsonResOP.h"

namespace edb
{

class JsonResFixOP : public JsonResOP
{
public:
	JsonResFixOP(const std::string& filepath);

protected:
	virtual bool OnDoFile(const std::string& filepath, Json::Value& val, const std::string& key);

	virtual void AfterDoGroup(bool dirty, Json::Value& val);
	virtual void AfterDoCommon(bool dirty, Json::Value& val, const std::string& key);
	virtual void AfterDoTexture(bool dirty, Json::Value& val);
	virtual void AfterDoAnim(bool dirty, Json::Value& val);

}; // JsonResFixOP

}

#endif // _EASYDB_JSON_RES_FIX_OP_H_