#ifndef _CLASS_UTILITY_CLONEABLE_H_
#define _CLASS_UTILITY_CLONEABLE_H_

namespace cu
{

class Cloneable 
{
public:
	virtual Cloneable* Clone() const = 0;
	virtual ~Cloneable() {}
}; // Cloneable

}

#endif // _CLASS_UTILITY_CLONEABLE_H_
