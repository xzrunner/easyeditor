#ifndef _EASYEDITOR_BODY_DATA_H_
#define _EASYEDITOR_BODY_DATA_H_

#include "ResourcesMgr.h"

#include <SM_Vector.h>
#include <CU_RefCountObj.h>

#include <Box2D/Box2D.h>

#include <vector>

namespace ee
{

class BodyData;
typedef ResourcesMgr<BodyData> BodyDataMgr;

class BodyData : public cu::RefCountObj
{
public:
	enum Type
	{
		e_chain	= 0,
		e_polygon,
		e_circle,
		e_shapes,
		e_mesh
	};

	struct FixtureData
	{
		std::vector<sm::vec2> vertices;
		float density, friction, restitution;

		FixtureData() {
			density = 1;
			friction = 0.5f;
			restitution = 0.7f;
		}

		void setFixtureDef(b2FixtureDef& fixtureDef) const {
			fixtureDef.density = density;
			fixtureDef.friction = friction;
			fixtureDef.restitution = restitution;
		}

	}; // FixtureData

public:
	BodyData();
	~BodyData();

	Type type() const;

	const std::vector<FixtureData*>& getAllFixturesData() const;

	const std::string& getFilename() const;

	void insertFixture(FixtureData* fixtureData) { 
		m_fixtures.push_back(fixtureData); 
	}

	bool LoadFromFile(const std::string& filename);

private:
	void clear();

	void loadFromMeshFile(const std::string& filename);
	void loadFromShapeFile(const std::string& filename);

private:
	std::string m_filename;

	Type m_type;

	std::vector<FixtureData*> m_fixtures;

	friend class ResourcesMgr<BodyData>;

}; // BodyData

inline BodyData::Type BodyData::type() const
{	
	return m_type;
}

inline const std::vector<BodyData::FixtureData*>& BodyData::getAllFixturesData() const
{
	return m_fixtures;
}

inline const std::string& BodyData::getFilename() const
{
	return m_filename;
}

}

#endif // _EASYEDITOR_BODY_DATA_H_