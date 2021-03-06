#ifndef _EASYEDITOR_SHAPES_BODY_H_
#define _EASYEDITOR_SHAPES_BODY_H_

#include "IBody.h"

namespace ee
{

class BodyData;

class ShapesBody : public IBody
{
public:
	ShapesBody(b2World* world, const BodyData& bodyData, float scale = 1.0f);

	virtual void GetRect(sm::rect& rect, const sm::vec2& position, float angle) const override;

}; // ShapesBody

}

#endif // _EASYEDITOR_SHAPES_BODY_H_