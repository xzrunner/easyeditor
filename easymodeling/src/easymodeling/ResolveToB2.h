
#ifndef EMODELING_RESOLVE_TO_B2_H
#define EMODELING_RESOLVE_TO_B2_H

#include <drag2d.h>
#include <Box2D/Box2D.h>

namespace libmodeling
{
	class Body;
	class Joint;
}

namespace emodeling
{
	class ResolveToB2
	{
	public:
		static b2Body* createBody(const libmodeling::Body& data, b2World* world,
			std::map<libmodeling::Body*, b2Body*>& mapBody);
		static b2Joint* createJoint(const libmodeling::Joint& data, b2World* world,
			const std::map<libmodeling::Body*, b2Body*>& mapBody);
		static b2Joint* createJoint(const libmodeling::Joint& data, b2World* world,
			const std::map<libmodeling::Body*, b2Body*>& mapBody,
			const std::map<libmodeling::Joint*, b2Joint*>& mapJoint); 

	}; // ResolveToB2
}

#endif // EMODELING_RESOLVE_TO_B2_H
