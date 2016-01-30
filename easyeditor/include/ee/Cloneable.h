#ifndef _EASYEDITOR_CLONEABLE_H_
#define _EASYEDITOR_CLONEABLE_H_

namespace ee
{

class Cloneable 
{
public:
	virtual Cloneable* Clone() const = 0;
	virtual ~Cloneable() {}
}; // Cloneable

}

#endif // _EASYEDITOR_CLONEABLE_H_
