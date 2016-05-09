#ifndef _SPRITE2_RENDER_PARAMS_INL_
#define _SPRITE2_RENDER_PARAMS_INL_

namespace s2
{

inline
RenderParams::RenderParams()
	: root_spr(NULL)
	, set_shader(true)
	, col_mul(0xffffffff)
	, col_add(0)
	, col_rmap(255, 0, 0, 0)
	, col_gmap(0, 255, 0, 0)
	, col_bmap(0, 0, 0, 255)
	, filter(FM_NULL)
	, blend(BM_NULL)
	, camera(CM_ORTHO)
	, camera_base_y(0)
{
}

}

#endif // _SPRITE2_RENDER_PARAMS_INL_