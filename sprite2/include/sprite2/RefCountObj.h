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

	void MarkUnshareable();
	bool IsShareable() const;
	bool IsSheared() const;

private:
	int m_ref_count;
	bool m_shareable;

}; // RefCountObj

}

#include "RefCountObj.inl"

#endif // _SPRITE2_REF_COUNT_OBJ_H_