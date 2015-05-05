#ifndef _DRAG2D_ONE_FLOAT_VALUE_STATIC_H_
#define _DRAG2D_ONE_FLOAT_VALUE_STATIC_H_

#include "OneFloatValue.h"

namespace d2d
{
	
class OneFloatValueStatic : public OneFloatValue
{
public:
	OneFloatValueStatic(float value) : m_value(value) {}

	virtual float GetValue() const { return m_value; }

private:
	float m_value;

}; // OneFloatValueStatic

}

#endif // _DRAG2D_ONE_FLOAT_VALUE_STATIC_H_