#ifndef _SPRITE2_RF_NULL_H_
#define _SPRITE2_RF_NULL_H_

#include "RenderFilter.h"

namespace s2
{

class RFNull : public RenderFilter
{
public:
	RFNull() 
		: RenderFilter(FM_NULL) 
	{}

}; // RFNull

}

#endif // _SPRITE2_RF_NULL_H_