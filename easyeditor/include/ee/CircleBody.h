#ifndef _EASYEDITOR_CIRCLE_BODY_H_
#define _EASYEDITOR_CIRCLE_BODY_H_

#include "IBody.h"

namespace ee
{

class BodyData;

class CircleBody : public IBody
{
public:
	CircleBody(b2World* world, const BodyData& bodyData, float scale = 1.0f);
	CircleBody(b2World* world, float radius);

	virtual void GetRect(Rect& rect, const sm::vec2& position, float angle) const;

}; // CircleBody

}

#endif // _EASYEDITOR_CIRCLE_BODY_H_