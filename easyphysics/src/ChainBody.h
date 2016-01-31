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
	ChainBody(b2World* world, const std::vector<ee::Vector>& vertices, bool isLoop, float scale = 1.0f);
	ChainBody(b2World* world, float scale = 1.0f);

	virtual void getRect(ee::Rect& rect, const ee::Vector& position, float angle) const;

}; // ChainBody

}

#endif // _EPHYSICS_CHAIN_BODY_H_