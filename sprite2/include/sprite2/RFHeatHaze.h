#ifndef _SPRITE2_RF_HEAT_HAZE_H_
#define _SPRITE2_RF_HEAT_HAZE_H_

#include "RenderFilter.h"

namespace s2
{

class RFHeatHaze : public RenderFilter
{
public:
	RFHeatHaze() 
		: RenderFilter(FM_HEAT_HAZE) 
	{}

}; // RFHeatHaze

}

#endif // _SPRITE2_RF_HEAT_HAZE_H_