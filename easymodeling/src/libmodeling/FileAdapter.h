
#ifndef LIBMODELING_FILE_ADAPTER_H
#define LIBMODELING_FILE_ADAPTER_H


#include <json/json.h>

namespace libmodeling
{
	class World;
	class Body;
	class Fixture;
	class Joint;

	class FileApapter
	{
	public:
		static World* j2World(const Json::Value& worldValue);
		static Body* j2bBody(const Json::Value& bodyValue, const std::string& dlg);
		static Fixture* j2bFixture(const Json::Value& fixtureValue);
		static Joint* j2bJoint(const Json::Value& jointValue, 
			const std::vector<Body*>& bodies);

	public:
		~FileApapter();

		void resolve(const wxString& filepath);

		Body* queryBody(size_t index) const;

		Body* queryBody(const wxString& name) const;
		Joint* queryJoint(const wxString& name) const;

	private:
		void clear();

	private:
		std::vector<Body*> m_bodies;

		std::map<wxString, Body*> m_nameBodyMap;
		std::map<wxString, Joint*> m_nameJointMap;

	}; // FileApapter
}

#endif // LIBMODELING_FILE_ADAPTER_H
