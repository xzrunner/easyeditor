#ifndef _EPHYSICS_SHAPES_BODY_H_
#define _EPHYSICS_SHAPES_BODY_H_

#include "IBody.h"

namespace ephysics
{

class BodyData;

class ShapesBody : public IBody
{
public:
	ShapesBody(b2World* world, const BodyData& bodyData, float scale = 1.0f);

	virtual void getRect(ee::Rect& rect, const ee::Vector& position, float angle) const;

}; // ShapesBody

}

#endif // _EPHYSICS_SHAPES_BODY_H_