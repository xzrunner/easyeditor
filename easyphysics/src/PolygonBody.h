#ifndef _EPHYSICS_POLYGON_BODY_H_
#define _EPHYSICS_POLYGON_BODY_H_

#include "IBody.h"

namespace ephysics
{

class BodyData;

class PolygonBody : public IBody
{
public:
	PolygonBody(b2World* world, const BodyData& bodyData, float scale = 1.0f);
	PolygonBody(b2World* world, float hWidth, float hHeight, bool isStatic, float scale = 1.0f);
	
	virtual void getRect(ee::Rect& rect, const ee::Vector& position, float angle) const;

}; // PolygonBody

}

#endif // _EPHYSICS_POLYGON_BODY_H_