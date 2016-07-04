#ifndef _LR_NEW_COMPLEX_UD_H_
#define _LR_NEW_COMPLEX_UD_H_

#include "UserData.h"

namespace lr
{

class NewComplexUD : public UserData
{
public:
	NewComplexUD()
		: UserData(UT_NEW_COMPLEX)
	{}

}; // NewComplexUD

}

#endif // _LR_NEW_COMPLEX_UD_H_