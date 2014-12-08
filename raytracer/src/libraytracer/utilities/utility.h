#ifndef _RT_UTILITIES_H_
#define _RT_UTILITIES_H_

namespace rt
{

template<class T>
struct DeletePointerFunctor
{
	void operator()(const T* pT) const
	{
		delete pT, pT = NULL;
	}
};

}

#endif // _RT_UTILITIES_H_