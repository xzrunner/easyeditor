#ifndef _SPRITE2_RF_SHOCK_WAVE_H_
#define _SPRITE2_RF_SHOCK_WAVE_H_

#include "RenderFilter.h"

namespace s2
{

class RFShockWave : public RenderFilter
{
public:
	RFShockWave() 
		: RenderFilter(FM_SHOCK_WAVE) 
	{}

}; // RFShockWave

}

#endif // _SPRITE2_RF_SHOCK_WAVE_H_