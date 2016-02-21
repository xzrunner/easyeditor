#ifndef _EASYMODELING_RESOLVE_TO_B2_H_
#define _EASYMODELING_RESOLVE_TO_B2_H_

#include <Box2D/Box2D.h>

#include <map>

namespace emodeling
{

class Body;
class Joint;

class ResolveToB2
{
public:
	static b2Body* CreateBody(const Body& data, b2World* world,
		std::map<Body*, b2Body*>& mapBody);
	static b2Joint* CreateJoint(const Joint& data, b2World* world,
		const std::map<Body*, b2Body*>& mapBody);
	static b2Joint* CreateJoint(const Joint& data, b2World* world,
		const std::map<Body*, b2Body*>& mapBody,
		const std::map<Joint*, b2Joint*>& mapJoint); 

}; // ResolveToB2

}

#endif // _EASYMODELING_RESOLVE_TO_B2_H_
