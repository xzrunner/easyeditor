#pragma once

#include "IBody.h"

#include <vector>

namespace d2d
{
	class BodyData;

	class ChainBody : public IBody
	{
	public:
		ChainBody(b2World* world, const BodyData& bodyData, float scale = 1.0f);
		ChainBody(b2World* world, const std::vector<Vector>& vertices, bool isLoop, float scale = 1.0f);
		ChainBody(b2World* world, float scale = 1.0f);

		virtual void getRect(Rect& rect, const Vector& position, float angle) const;

	}; // ChainBody
}

