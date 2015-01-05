#ifndef _EPBIN_TOOLS_H_
#define _EPBIN_TOOLS_H_

#include <string.h>

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

void str_replace(std::string& str, const std::string& old_value, const std::string& new_value)   
{
	for(std::string::size_type pos = 0; pos != std::string::npos; pos += new_value.length()) {   
		if((pos = str.find(old_value,pos)) != std::string::npos) {
			str.replace(pos, old_value.length(), new_value);
		} else {
			break;
		}
	}   
}

}

#endif // _EPBIN_TOOLS_H_