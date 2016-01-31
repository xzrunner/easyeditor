#ifndef _EPHYSICS_CIRCLE_BODY_H_
#define _EPHYSICS_CIRCLE_BODY_H_

#include "IBody.h"

namespace ephysics
{

class BodyData;

class CircleBody : public IBody
{
public:
	CircleBody(b2World* world, const BodyData& bodyData, float scale = 1.0f);
	CircleBody(b2World* world, float radius);

	virtual void getRect(ee::Rect& rect, const ee::Vector& position, float angle) const;

}; // CircleBody

}

#endif // _EPHYSICS_CIRCLE_BODY_H_