#ifndef _EASYEDITOR_POLYGON_BODY_H_
#define _EASYEDITOR_POLYGON_BODY_H_

#include "IBody.h"

namespace ee
{

class BodyData;

class PolygonBody : public IBody
{
public:
	PolygonBody(b2World* world, const BodyData& bodyData, float scale = 1.0f);
	PolygonBody(b2World* world, float hWidth, float hHeight, bool isStatic, float scale = 1.0f);
	
	virtual void GetRect(sm::rect& rect, const sm::vec2& position, float angle) const;

}; // PolygonBody

}

#endif // _EASYEDITOR_POLYGON_BODY_H_