#ifndef _EASYEDITOR_STD_FUNCTOR_H_
#define _EASYEDITOR_STD_FUNCTOR_H_

namespace ee
{

template<class T>
struct DeletePointerFunctor
{
	void operator()(const T* p) const
	{
		delete p, p = NULL;
	}
};

}

#endif // _EASYEDITOR_STD_FUNCTOR_H_
