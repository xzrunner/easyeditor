#ifndef _EASYSPRPACKER_CHECK_H_
#define _EASYSPRPACKER_CHECK_H_

#include <string>

namespace esprpacker
{

inline
bool is_name_valid(const std::string& name) {
	return !name.empty() && name[0] != '_';	
}

}

#endif // _EASYSPRPACKER_CHECK_H_