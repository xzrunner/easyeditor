#ifndef _RT_INTERFACES_H_
#define _RT_INTERFACES_H_

namespace rt
{

class ICloneable 
{
public:
	virtual ~ICloneable() {}
	virtual ICloneable* Clone() const = 0;
}; // ICloneable

}

#endif // _RT_INTERFACES_H_