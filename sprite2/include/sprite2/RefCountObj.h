#ifndef _SPRITE2_REF_COUNT_OBJ_H_
#define _SPRITE2_REF_COUNT_OBJ_H_

namespace s2
{

class RefCountObj
{
public:
	RefCountObj();
	RefCountObj(const RefCountObj& obj);
	RefCountObj& operator = (const RefCountObj& obj);
	virtual ~RefCountObj() = 0;

	void AddReference();
	void RemoveReference();

private:
	int m_ref_count;

}; // RefCountObj

}

#include "RefCountObj.inl"

#endif // _SPRITE2_REF_COUNT_OBJ_H_