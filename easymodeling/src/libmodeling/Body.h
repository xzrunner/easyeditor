
#ifndef LIBMODELING_BODY_H
#define LIBMODELING_BODY_H



#include <vector>

namespace libmodeling
{
	class Fixture;

	class Body : public ee::Object
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

		bool isContain(const ee::Vector& pos) const;
		bool isIntersect(const ee::Rect& rect) const;

		void draw(const ee::Matrix& mt, const ee::Colorf& cFace, 
			const ee::Colorf& cEdge) const;

	public:
		wxString name;

		Type type;

		float linearDamping;

		float angularDamping;

		bool allowSleep;

		bool bullet;

		bool active;

		float gravityScale;

		ee::Sprite* sprite;

		std::vector<Fixture*> fixtures;

	}; // Body
}

#endif // LIBMODELING_BODY_H
