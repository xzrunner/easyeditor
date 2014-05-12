#pragma once

#include "IBody.h"

namespace d2d
{
	class BodyData;

	class ShapesBody : public IBody
	{
	public:
		ShapesBody(b2World* world, const BodyData& bodyData, float scale = 1.0f);

		virtual void getRect(Rect& rect, const Vector& position, float angle) const;

	}; // ShapesBody
}

