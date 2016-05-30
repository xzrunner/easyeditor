#ifndef _SPRITE2_RF_GRAY_H_
#define _SPRITE2_RF_GRAY_H_

#include "RenderFilter.h"

namespace s2
{

class RFGray : public RenderFilter
{
public:
	RFGray() 
		: RenderFilter(FM_GRAY) 
	{}

}; // RFGray

}

#endif // _SPRITE2_RF_GRAY_H_