
#ifndef LIBMODELING_FIXTURE_H
#define LIBMODELING_FIXTURE_H

#include <drag2d.h>

namespace libmodeling
{
	class Body;

	class Fixture
	{
	public:
		Fixture();
		~Fixture();

		bool isContain(const d2d::Vector& pos) const;
		bool isIntersect(const d2d::Rect& rect) const;

		void draw(const d2d::Colorf& cFace, const d2d::Colorf& cEdge) const;

	private:
		void transLocalToWorld(const std::vector<d2d::Vector>& local, std::vector<d2d::Vector>& world) const;

	public:
		wxString name;

		Body* body;

		d2d::IShape* shape;

		float density;

		float friction;
		float restitution;

		bool isSensor;

		unsigned short categoryBits;
		unsigned short maskBits;
		short groupIndex;

	}; // Fixture
}

#endif // LIBMODELING_FIXTURE_H
