#ifndef _EASYEDITOR_CHAIN_BODY_H_
#define _EASYEDITOR_CHAIN_BODY_H_

#include "IBody.h"

#include <vector>

namespace ee
{

class BodyData;

class ChainBody : public IBody
{
public:
	ChainBody(b2World* world, const BodyData& bodyData, float scale = 1.0f);
	ChainBody(b2World* world, const std::vector<Vector>& vertices, bool isLoop, float scale = 1.0f);
	ChainBody(b2World* world, float scale = 1.0f);

	virtual void GetRect(Rect& rect, const Vector& position, float angle) const;

}; // ChainBody

}

#endif // _EASYEDITOR_CHAIN_BODY_H_