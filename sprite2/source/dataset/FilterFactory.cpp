#include "FilterFactory.h"

#include "RFNull.h"

#include "RFEdgeDetection.h"
#include "RFRelief.h"
#include "RFOutline.h"
#include "RFOuterGlow.h"

#include "RFGray.h"
#include "RFBlur.h"
#include "RFGaussianBlur.h"

#include "RFHeatHaze.h"
#include "RFShockWave.h"
#include "RFSwirl.h"
#include "RFBurningMap.h"

#include <stddef.h>
#include <assert.h>
#include <string.h>

namespace s2
{

SINGLETON_DEFINITION(FilterFactory)

FilterFactory::FilterFactory() 
{
	memset(m_temps, 0, sizeof(m_temps));

	m_temps[FM_NULL]			= new RFNull;

	m_temps[FM_EDGE_DETECTION]	= new RFEdgeDetection;
	m_temps[FM_RELIEF]			= new RFRelief;
	m_temps[FM_OUTLINE]			= new RFOutline;
	m_temps[FM_OUTER_GLOW]		= new RFOuterGlow;

	m_temps[FM_GRAY]			= new RFGray;
	m_temps[FM_BLUR]			= new RFBlur;
	m_temps[FM_GAUSSIAN_BLUR]	= new RFGaussianBlur;

	m_temps[FM_HEAT_HAZE]		= new RFHeatHaze;
	m_temps[FM_SHOCK_WAVE]		= new RFShockWave;
	m_temps[FM_SWIRL]			= new RFSwirl;
	m_temps[FM_BURNING_MAP]		= new RFBurningMap;	
}

FilterFactory::~FilterFactory() 
{
	for (int i = 0; i < MAX_COUNT; ++i) {
		if (m_temps[i]) {
			delete m_temps[i];
		}
	}
}

RenderFilter* FilterFactory::Create(FilterMode mode)
{
	RenderFilter* filter = NULL;	
	switch (mode)
	{
	case FM_NULL:
		filter = new RFNull;
		break;

	case FM_EDGE_DETECTION:
		filter = new RFEdgeDetection;
		break;
	case FM_RELIEF:
		filter = new RFRelief;
		break;
	case FM_OUTLINE:
		filter = new RFOutline;
		break;
	case FM_OUTER_GLOW:
		filter = new RFOuterGlow;
		break;

	case FM_GRAY:
		filter = new RFGray;
		break;
	case FM_BLUR:
		filter = new RFBlur;
		break;
	case FM_GAUSSIAN_BLUR:
		filter = new RFGaussianBlur;
		break;

	case FM_HEAT_HAZE:
		filter = new RFHeatHaze;
		break;
	case FM_SHOCK_WAVE:
		filter = new RFShockWave;
		break;
	case FM_SWIRL:
		filter = new RFSwirl;
		break;
	case FM_BURNING_MAP:
		filter = new RFBurningMap;
		break;

	default:
		assert(0);
	}
	return filter;
}

}