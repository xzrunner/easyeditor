#ifndef _CLASS_UTILITY_REF_COUNT_OBJ_H_
#define _CLASS_UTILITY_REF_COUNT_OBJ_H_

namespace cu
{

class RefCountObj
{
public:
	RefCountObj();
	RefCountObj(const RefCountObj& obj);
	RefCountObj& operator = (const RefCountObj& obj);
	virtual ~RefCountObj() = 0;

	void AddReference() const;
	void RemoveReference() const;

private:
	mutable int m_ref_count;

}; // RefCountObj

template<class T>
struct AddRefFonctor
{
	void operator()(const T* p) const {
		p->AddReference();
	}
}; // AddRefFonctor

template<class T>
struct RemoveRefFonctor
{
	void operator()(const T* p) const {
		p->RemoveReference();
	}
}; // RemoveRefFonctor

template<class T>
void RefCountObjAssign(T*& left, T*& right)
{
	if (left != right) {
		if (left) {
			left->RemoveReference();
		}
		left = right;
		if (right) {
			right->AddReference();
		}
	}
}

}

#include "CU_RefCountObj.inl"

#endif // _CLASS_UTILITY_REF_COUNT_OBJ_H_