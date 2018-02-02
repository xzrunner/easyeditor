#pragma once

namespace pt2
{

enum FilterMode
{
	FM_NULL					= 0,

	FM_EDGE_DETECTION		= 10,
	FM_RELIEF,
	FM_OUTLINE,
	FM_OUTER_GLOW,

	FM_GRAY					= 20,
	FM_BLUR,
	FM_GAUSSIAN_BLUR,

	FM_HEAT_HAZE			= 30,
	FM_SHOCK_WAVE,
	FM_SWIRL,
	FM_BURNING_MAP,

	FM_COL_GRADING			= 40,

}; // FilterMode

}
