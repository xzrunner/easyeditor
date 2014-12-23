#ifndef _EPBIN_TOOLS_H_
#define _EPBIN_TOOLS_H_

namespace epbin
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

#endif // _EPBIN_TOOLS_H_