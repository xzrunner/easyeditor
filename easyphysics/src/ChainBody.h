#ifndef _EPHYSICS_CHAIN_BODY_H_
#define _EPHYSICS_CHAIN_BODY_H_

#include "IBody.h"

#include <vector>

namespace ephysics
{

class BodyData;

class ChainBody : public IBody
{
public:
	ChainBody(b2World* world, const BodyData& bodyData, float scale = 1.0f);
	ChainBody(b2World* world, const std::vector<d2d::Vector>& vertices, bool isLoop, float scale = 1.0f);
	ChainBody(b2World* world, float scale = 1.0f);

	virtual void getRect(d2d::Rect& rect, const d2d::Vector& position, float angle) const;

}; // ChainBody

}

#endif // _EPHYSICS_CHAIN_BODY_H_