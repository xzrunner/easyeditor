#pragma once

#include "common/ResourcesMgr.h"

#include <vector>
#include <Box2D/Box2D.h>

#include "common/Vector.h"
#include "common/Object.h"

namespace d2d
{
	class BodyData;
	typedef ResourcesMgr<BodyData> BodyDataMgr;

	class BodyData : public Object
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
			std::vector<Vector> vertices;
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

		const wxString& getFilename() const;

		void insertFixture(FixtureData* fixtureData) { 
			m_fixtures.push_back(fixtureData); 
		}

	private:
		void clear();

		bool loadFromFile(const wxString& filename);

		void loadFromMeshFile(const wxString& filename);
		void loadFromPolygonFile(const wxString& filename);
		void loadFromCircleFile(const wxString& filename);
		void loadFromShapeFile(const wxString& filename);

	private:
		wxString m_filename;

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

	inline const wxString& BodyData::getFilename() const
	{
		return m_filename;
	}
}

