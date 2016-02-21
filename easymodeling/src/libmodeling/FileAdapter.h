#ifndef _EASYMODELING_FILE_ADAPTER_H_
#define _EASYMODELING_FILE_ADAPTER_H_

#include <json/json.h>

namespace emodeling
{

class World;
class Body;
class Fixture;
class Joint;

class FileApapter
{
public:
	static World* ToWorld(const Json::Value& worldValue);
	static Body* ToBody(const Json::Value& bodyValue, const std::string& dlg);
	static Fixture* ToFixture(const Json::Value& fixtureValue);
	static Joint* ToJoint(const Json::Value& jointValue, 
		const std::vector<Body*>& bodies);

public:
	~FileApapter();

	void Resolve(const std::string& filepath);

	Body* QueryBody(size_t index) const;

	Body* QueryBody(const std::string& name) const;
	Joint* QueryJoint(const std::string& name) const;

private:
	void Clear();

private:
	std::vector<Body*> m_bodies;

	std::map<std::string, Body*> m_name_body_map;
	std::map<std::string, Joint*> m_name_joint_map;

}; // FileApapter

}

#endif // _EASYMODELING_FILE_ADAPTER_H_
