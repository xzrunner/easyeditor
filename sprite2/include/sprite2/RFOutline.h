#ifndef _SPRITE2_RF_OUTLINE_H_
#define _SPRITE2_RF_OUTLINE_H_

#include "RenderFilter.h"

namespace s2
{

class RFOutline : public RenderFilter
{
public:
	RFOutline() 
		: RenderFilter(FM_OUTLINE) 
	{}

}; // RFOutline

}

#endif // _SPRITE2_RF_OUTLINE_H_