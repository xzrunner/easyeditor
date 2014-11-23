#ifndef _EPHYSICS_COMPLEX_BODY_H_
#define _EPHYSICS_COMPLEX_BODY_H_

#include "IBody.h"
#include "FixtureData.h"

#include <vector>

namespace ephysics
{

class ComplexBody : public IBody
{
public:
	ComplexBody(b2World* world, const std::vector<FixtureDataInfo>& fixtures, 
		float scale = 1.0f);

	virtual void getRect(d2d::Rect& rect, const d2d::Vector& position, float angle) const;

}; // ComplexBody

}

#endif // _EPHYSICS_COMPLEX_BODY_H_