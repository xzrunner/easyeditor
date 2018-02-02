#include "painting2/FilterFactory.h"

#include "painting2/RFEdgeDetection.h"
#include "painting2/RFRelief.h"
#include "painting2/RFOutline.h"
#include "painting2/RFOuterGlow.h"

#include "painting2/RFGray.h"
#include "painting2/RFBlur.h"
#include "painting2/RFGaussianBlur.h"

#include "painting2/RFHeatHaze.h"
#include "painting2/RFShockWave.h"
#include "painting2/RFSwirl.h"
#include "painting2/RFBurningMap.h"

#include "painting2/RFColGrading.h"

#include <stddef.h>
#include <assert.h>
#include <string.h>

namespace pt2
{

CU_SINGLETON_DEFINITION(FilterFactory)

FilterFactory::FilterFactory() 
{
	memset(m_temps, 0, sizeof(m_temps));

	m_temps[pt2::FM_EDGE_DETECTION]	= new RFEdgeDetection;
	m_temps[pt2::FM_RELIEF]			= new RFRelief;
	m_temps[pt2::FM_OUTLINE]			= new RFOutline;
	m_temps[pt2::FM_OUTER_GLOW]		= new RFOuterGlow;

	m_temps[pt2::FM_GRAY]			= new RFGray;
	m_temps[pt2::FM_BLUR]			= new RFBlur;
	m_temps[pt2::FM_GAUSSIAN_BLUR]	= new RFGaussianBlur;

	m_temps[pt2::FM_HEAT_HAZE]		= new RFHeatHaze;
	m_temps[pt2::FM_SHOCK_WAVE]		= new RFShockWave;
	m_temps[pt2::FM_SWIRL]			= new RFSwirl;
	m_temps[pt2::FM_BURNING_MAP]		= new RFBurningMap;	

	m_temps[pt2::FM_COL_GRADING]		= new RFColGrading;
}

FilterFactory::~FilterFactory() 
{
	for (int i = 0; i < MAX_COUNT; ++i) {
		if (m_temps[i]) {
			delete m_temps[i];
		}
	}
}

RenderFilterPtr FilterFactory::Create(pt2::FilterMode mode)
{
	RenderFilter* filter = nullptr;
	switch (mode)
	{
	case pt2::FM_NULL:
		break;

	case pt2::FM_EDGE_DETECTION:
		filter = new (mm::AllocHelper::Allocate(sizeof(RFEdgeDetection))) RFEdgeDetection();
		break;
	case pt2::FM_RELIEF:
		filter = new (mm::AllocHelper::Allocate(sizeof(RFRelief))) RFRelief();
		break;
	case pt2::FM_OUTLINE:
		filter = new (mm::AllocHelper::Allocate(sizeof(RFOutline))) RFOutline();
		break;
	case pt2::FM_OUTER_GLOW:
		filter = new (mm::AllocHelper::Allocate(sizeof(RFOuterGlow))) RFOuterGlow();
		break;

	case pt2::FM_GRAY:
		filter = new (mm::AllocHelper::Allocate(sizeof(RFGray))) RFGray();
		break;
	case pt2::FM_BLUR:
		filter = new (mm::AllocHelper::Allocate(sizeof(RFBlur))) RFBlur();
		break;
	case pt2::FM_GAUSSIAN_BLUR:
		filter = new (mm::AllocHelper::Allocate(sizeof(RFGaussianBlur))) RFGaussianBlur();
		break;

	case pt2::FM_HEAT_HAZE:
		filter = new (mm::AllocHelper::Allocate(sizeof(RFHeatHaze))) RFHeatHaze();
		break;
	case pt2::FM_SHOCK_WAVE:
		filter = new (mm::AllocHelper::Allocate(sizeof(RFShockWave))) RFShockWave();
		break;
	case pt2::FM_SWIRL:
		filter = new (mm::AllocHelper::Allocate(sizeof(RFSwirl))) RFSwirl();
		break;
	case pt2::FM_BURNING_MAP:
		filter = new (mm::AllocHelper::Allocate(sizeof(RFBurningMap))) RFBurningMap();
		break;

	case pt2::FM_COL_GRADING:
		filter = new (mm::AllocHelper::Allocate(sizeof(RFColGrading))) RFColGrading();
		break;

	default:
		assert(0);
	}

	return RenderFilterPtr(filter, RenderFilter::Deleter);
}

}