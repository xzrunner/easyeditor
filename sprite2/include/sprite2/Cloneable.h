#ifndef _SPRITE2_CLONEABLE_H_
#define _SPRITE2_CLONEABLE_H_

namespace s2
{

class Cloneable 
{
public:
	virtual Cloneable* Clone() const = 0;
	virtual ~Cloneable() {}
}; // Cloneable

}

#endif // _SPRITE2_CLONEABLE_H_
