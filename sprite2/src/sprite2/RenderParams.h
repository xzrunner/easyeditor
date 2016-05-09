#ifndef _SPRITE2_RENDER_PARAMS_H_
#define _SPRITE2_RENDER_PARAMS_H_

namespace s2
{

class RenderParams
{
public:
	Sprite*		root_spr;

	bool		set_shader;

	Matrix		mat;

	// color
	Color		col_mul, col_add;
	Color		col_rmap, col_gmap, col_bmap;

	// shader
	FilterMode	filter;
	BlendMode	blend;

	// camera
	CameraMode	camera;
	float		camera_base_y;	
	
public:
	RenderParams();

}; // RenderStyle

}

#include "RenderParams.inl"

#endif // _SPRITE2_RENDER_PARAMS_H_