#ifndef _EASYEDITOR_ONE_FLOAT_VALUE_STATIC_H_
#define _EASYEDITOR_ONE_FLOAT_VALUE_STATIC_H_

#include "OneFloatValue.h"

namespace ee
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

#endif // _EASYEDITOR_ONE_FLOAT_VALUE_STATIC_H_