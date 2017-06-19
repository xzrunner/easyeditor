#ifndef _EASYDB_JSON_RES_OP_H_
#define _EASYDB_JSON_RES_OP_H_

#include <json/json.h>

#include <string>

namespace edb
{

class JsonResOP
{
public:
	JsonResOP(const std::string& filepath);
	virtual ~JsonResOP() {}

	void Do();

protected:
	virtual void Clear() {}

	virtual bool OnDoFile(const std::string& filepath, Json::Value& val, const std::string& key) = 0;

	virtual void BeforeDoCommon(const Json::Value& value) {}
	virtual void BeforeDoAinm(const Json::Value& value) {}

	virtual void AfterDoGroup(bool dirty, Json::Value& val) {}
	virtual void AfterDoCommon(bool dirty, Json::Value& val, const std::string& key) {}
	virtual void AfterDoTexture(bool dirty, Json::Value& val) {}
	virtual void AfterDoAnim(bool dirty, Json::Value& val) {}

protected:
	static const std::string KEY_SPR;
	static const std::string KEY_COMP;
	static const std::string KEY_PATH;

private:
	bool DoFile(Json::Value& val, const std::string& key);
	bool DoGroup(Json::Value& val, const std::string& key);

	void DoCommon(const std::string& key);
	void DoTexture();
	void DoAnim();
	void DoMesh();
	void DoMask();
	void DoUI();

private:
	static void LoadJson(const std::string& filepath, Json::Value& val);
	static void StoreJson(const std::string& filepath, const Json::Value& val);

protected:
	std::string m_filepath;
	std::string m_base_dir;

}; // JsonResOP

}

#endif // _EASYDB_JSON_RES_OP_H_