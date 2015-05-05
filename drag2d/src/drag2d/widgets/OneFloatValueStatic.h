#ifndef _DRAG2D_ONE_FLOAT_VALUE_STATIC_H_
#define _DRAG2D_ONE_FLOAT_VALUE_STATIC_H_

#include "OneFloatValue.h"

namespace d2d
{
	
class OneFloatValueStatic : public OneFloatValue
{
public:
	OneFloatValueStatic(float scope) : m_scope(scope) {}

	virtual float GetScope() const { return m_scope; }

private:
	float m_scope;

}; // OneFloatValueStatic

}

#endif // _DRAG2D_ONE_FLOAT_VALUE_STATIC_H_