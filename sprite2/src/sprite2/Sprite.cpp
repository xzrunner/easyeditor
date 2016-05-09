#include "Sprite.h"

namespace s2
{

Sprite::Sprite()
	: m_angle(0)
	, m_col_mul(0xffffffff)
	, m_col_add(0)
	, m_col_rmap(255, 0, 0, 0)
	, m_col_gmap(0, 255, 0, 0)
	, m_col_bmap(0, 0, 255, 0)
	, m_filter(FM_NULL)
	, m_blend(BM_NULL)
	, m_camera(CM_ORTHO)
	, m_camera_base_y(0)
{
}

}