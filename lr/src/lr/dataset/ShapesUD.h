#ifndef _LR_SHAPES_UD_H_
#define _LR_SHAPES_UD_H_

#include "UserData.h"

namespace lr
{

class ShapesUD : public UserData
{
public:
	ShapesUD()
		: UserData(UT_SHAPES)
	{}

public:
	int layer_id;
	std::vector<std::string> shape_names;

}; // ShapesUD

}

#endif // _LR_SHAPES_UD_H_