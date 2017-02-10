#ifndef _POLYMESH_SKIN2_JOINT_H_
#define _POLYMESH_SKIN2_JOINT_H_

#include <SM_Vector.h>

namespace pm
{

class Skin2Joint
{
public:
	int   joint;
	float weight;
	sm::vec2 vertex, offset;

public:
	Skin2Joint() : joint(-1), weight(0) {}

}; // Skin2Joint

}

#endif // _POLYMESH_SKIN2_JOINT_H_