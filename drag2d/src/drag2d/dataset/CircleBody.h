#pragma once

#include "IBody.h"

namespace d2d
{
	class BodyData;

	class CircleBody : public IBody
	{
	public:
		CircleBody(b2World* world, const BodyData& bodyData, float scale = 1.0f);
		CircleBody(b2World* world, float radius);

		virtual void getRect(Rect& rect, const Vector& position, float angle) const;

	}; // CircleBody
}

