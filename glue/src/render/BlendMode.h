#ifndef _GLUE_BLEND_MODE_H_
#define _GLUE_BLEND_MODE_H_

namespace glue
{

enum BlendMode
{
	// normal
	BM_NORMAL		= 0,
	BM_DISSOLVE,		// todo

	// darken modes
	BM_DARKEN		= 10,
	BM_MULTIPLY,
	BM_COLOR_BURN,
	BM_LINEAR_BURN,
	BM_DARKER_COLOR,	// todo

	// lighten modes
	BM_LIGHTEN		= 20,
	BM_SCREEN,
	BM_COLOR_DODGE,
	BM_LINEAR_DODGE,
	BM_LIGHTER_COLOR,	// todo

	// saturation modes
	BM_OVERLAY		= 30,
	BM_SOFT_LIGHT,
	BM_HARD_LIGHT,
	BM_VIVID_LIGHT,
	BM_LINEAR_LIGHT,
	BM_PIN_LIGHT,
	BM_HARD_MIX,

	// substraction modes
	BM_DIFFERENCE	= 40,
	BM_EXCLUSION,

	// color modes 
	BM_HUE = 50,		// todo
	BM_SATURATION,		// todo
	BM_COLOR,			// todo
	BM_LUMINOSITY,		// todo

	// others
	BM_UNKNOWN		= 99,
};

}

#endif // _GLUE_BLEND_MODE_H_