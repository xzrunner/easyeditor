#ifndef _LR_USER_DATA_H_
#define _LR_USER_DATA_H_

#include "ud_type.h"

namespace lr
{

class UserData
{
public:
	UserData(UDType type) : type(type) {}

	UDType type;

}; // UserData

}

#endif // _LR_USER_DATA_H_