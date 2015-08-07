#ifndef _LR_BASE_FILE_UD_H_
#define _LR_BASE_FILE_UD_H_

#include "UserData.h"

namespace lr
{

class BaseFileUD : public UserData
{
public:
	BaseFileUD(const std::string& path) 
		: UserData(UT_BASE_FILE)
		, base_path(path) 
	{}

	std::string base_path;

}; // BaseFileUD

}

#endif // _LR_BASE_FILE_UD_H_