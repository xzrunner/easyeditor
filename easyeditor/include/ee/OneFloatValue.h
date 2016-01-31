#ifndef _EASYEDITOR_ONE_FLOAT_VALUE_H_
#define _EASYEDITOR_ONE_FLOAT_VALUE_H_

namespace ee
{

class OneFloatValue
{
public:
	virtual ~OneFloatValue() {}
	virtual float GetValue() const = 0;
}; // OneFloatValue

}

#endif // _EASYEDITOR_ONE_FLOAT_VALUE_H_