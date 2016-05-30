#ifndef _SPRITE2_RF_EDGE_DETECTION_H_
#define _SPRITE2_RF_EDGE_DETECTION_H_

#include "RenderFilter.h"

namespace s2
{

class RFEdgeDetection : public RenderFilter
{
public:
	RFEdgeDetection() 
		: RenderFilter(FM_EDGE_DETECTION) 
		, m_blend(0.5f)
	{}

	float GetBlend() const { return m_blend; }
	void SetBlend(float blend) { m_blend = blend; }

private:
	float m_blend;

}; // RFEdgeDetection

}

#endif // _SPRITE2_RF_EDGE_DETECTION_H_