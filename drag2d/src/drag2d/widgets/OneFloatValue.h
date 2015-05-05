#ifndef _DRAG2D_ONE_FLOAT_VALUE_H_
#define _DRAG2D_ONE_FLOAT_VALUE_H_

namespace d2d
{

class OneFloatValue
{
public:
	virtual ~OneFloatValue() {}
	virtual float GetScope() const = 0;
}; // OneFloatValue

}

#endif // _DRAG2D_ONE_FLOAT_VALUE_H_