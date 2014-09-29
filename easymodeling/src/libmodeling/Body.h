
#ifndef LIBMODELING_BODY_H
#define LIBMODELING_BODY_H

#include <drag2d.h>

#include <vector>

namespace libmodeling
{
	class Fixture;

	class Body : public d2d::Object
	{
	public:
		enum Type
		{
			e_static = 0,
			e_kinematic,
			e_dynamic
		};

	public:
		Body();
		~Body();

		bool isContain(const d2d::Vector& pos) const;
		bool isIntersect(const d2d::Rect& rect) const;

		void draw(const d2d::Colorf& cFace, const d2d::Colorf& cEdge) const;

	public:
		wxString name;

		Type type;

		float linearDamping;

		float angularDamping;

		bool allowSleep;

		bool bullet;

		bool active;

		float gravityScale;

		d2d::ISprite* sprite;

		std::vector<Fixture*> fixtures;

	}; // Body
}

#endif // LIBMODELING_BODY_H
