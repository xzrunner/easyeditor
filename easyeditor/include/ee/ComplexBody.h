#ifndef _EASYEDITOR_COMPLEX_BODY_H_
#define _EASYEDITOR_COMPLEX_BODY_H_

#include "IBody.h"
#include "FixtureData.h"

#include <vector>

namespace ee
{

class ComplexBody : public IBody
{
public:
	ComplexBody(b2World* world, const std::vector<FixtureDataInfo>& fixtures, 
		float scale = 1.0f);

	virtual void GetRect(Rect& rect, const Vector& position, float angle) const;

}; // ComplexBody

}

#endif // _EASYEDITOR_COMPLEX_BODY_H_